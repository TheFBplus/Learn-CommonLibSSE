#include "include/SDFix_Events.h"
#include "include/SDFix_Functions.h"
#include "include/SDFix_DataHandler.h"

namespace SDFix
{


	bool  EffectStartEventHandler::RegisterEffectStartEvent()
	{
		static  EffectStartEventHandler g_effectstarthandler;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

		if (!ScriptEventSource)
		{
			logger::error("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&g_effectstarthandler);

		logger::info("Register Effect Start Event Handler!");

		return true;
	}



	EventResult  EffectStartEventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource)
		{
			logger::error("Event Source Not Found!");
			return EventResult::kContinue;
		}

		auto SDF_datahandler = DataHandler::GetSingleton();

		if (!SDF_datahandler)
		{
			logger::error("SDF_datahandler Not Found!");
			return EventResult::kContinue;

		}

		//----------------------Check Hit Causer-----------------------------------------------

		auto hit_causer = a_event->caster.get();

		if (!hit_causer)
		{
			logger::error("Hit Causer Not Found!");
			return EventResult::kContinue;
		}

		if (!hit_causer->Is3DLoaded())
		{
			logger::debug("Hit Causer Not 3D Loaded!");
			return EventResult::kContinue;
		}
		//-------------------------------------------------------------------------------------




		//----------------------Check Hit Target-----------------------------------------------

		auto hit_target = a_event->target.get();

		if (!hit_target)
		{
			logger::error("Hit Target Not Found!");
			return EventResult::kContinue;
		}

		if (!hit_target->Is3DLoaded())
		{
			logger::debug("Hit Target Not 3D Loaded!");
			return EventResult::kContinue;
		}


		if (hit_target->formType != RE::FormType::ActorCharacter)
		{
			logger::debug("Hit Target Not Actor!");
			return EventResult::kContinue;
		}

		if (!hit_target->As<RE::Actor>()->CalculateCachedOwnerIsNPC())
		{
			logger::debug("Hit Target Not Humandio Character!");
			return EventResult::kContinue;
		}

		if (hit_target->IsDead())
		{
			logger::debug("Hit Target is Dead!");
			return EventResult::kContinue;
		}


		if (!hit_target->HasGraphVariableFloat("staggerDirection"))
		{
			logger::debug("Hit Target Has Not staggerDirection!");
			return EventResult::kContinue;
		}

		//-------------------------------------------------------------------------------------




		//----------------------Check Magic Effect-----------------------------------------------

		auto this_effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);

		if (!this_effect)
		{
			logger::error("Effect Not Found!");
			return EventResult::kContinue;
		}


		if (this_effect->GetArchetype() == RE::EffectArchetypes::ArchetypeID::kStagger)
		{
			logger::debug("Effect Is Stagger!");
			logger::debug(FMT_STRING("Caster Name is \"{}\", ID is \"{:x}\""), hit_causer->GetName(), hit_causer->GetFormID());
			logger::debug(FMT_STRING("Hit Target Name is \"{}\", ID is \"{:x}\""), hit_target->GetName(), hit_target->GetFormID());

			if (hit_causer->GetFormID() == hit_target->GetFormID())
			{
				logger::debug("Effect Is Self Deliverey!");
				return EventResult::kContinue;
			}

			auto headangle = hit_target->GetHeadingAngle(hit_causer->GetPosition(), false);

			if (SDF_datahandler->AddHitAngleRecord(hit_target->GetFormID(), headangle))
				if (RegiterAnimEvent(hit_target->As<RE::Actor>(), AnimationGraphEventHanlder::GetSingleton()))
					logger::debug("Register Stagger Event Successfully!");
				else
					SDF_datahandler->RemoveHitAngleRecord(hit_target->GetFormID());
		}
		//---------------------------------------------------------------------------------------

		return EventResult::kContinue;
	}







	EventResult AnimationGraphEventHanlder::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource)
		{
			logger::error("Event Source Not Found!");
			return EventResult::kContinue;
		}

		a_eventSource->RemoveEventSink(AnimationGraphEventHanlder::GetSingleton());

		auto SDF_datahandler = DataHandler::GetSingleton();

		if (!SDF_datahandler)
		{
			logger::error("SDF_datahandler Not Found!");
			return EventResult::kContinue;

		}

		logger::debug("Animation Event Trigger!");

		logger::debug(FMT_STRING("Animation tag: {}, payload : {}."), a_event->tag, a_event->payload);

		auto this_target = a_event->holder;

		if (!this_target)
		{
			logger::error("Animation Event Holder Not Found!");
			return EventResult::kContinue;
		}


		if (!this_target->As<RE::Actor>()->IsStaggering())
		{
			logger::debug("Hit Target is Not Staggering!");
			return EventResult::kContinue;
		}


		float Cur_AnimVarFloat = 0.f;
		float headingAng = 0.f;

		if (SDF_datahandler->GetHitAngle(this_target->GetFormID(), headingAng))
		{
			SDF_datahandler->RemoveHitAngleRecord(this_target->GetFormID());

			headingAng = headingAng >= 0.f ? headingAng / 360.f : (360.f + headingAng) / 360.f;		//Convert the heading angle value to "staggerDirection" graph variable float

			if (!this_target->GetGraphVariableFloat("staggerDirection", Cur_AnimVarFloat))
			{
				logger::error("Can't Find Stagger Direction Value!");
				return EventResult::kContinue;
			}

			if (abs(Cur_AnimVarFloat) <= 1e-6)		//Check if the Graph Variable Float value equal to zero. The game system will reset this value to zero once a spell-cause-stagger took place.
			{
				logger::debug(FMT_STRING("Direction Float is {}"), headingAng);
				this_target->SetGraphVariableFloat("staggerDirection", headingAng);
			}
			else
				logger::debug("stagger Direction Float is not reset to zero");

			logger::debug(FMT_STRING("Map Size is {}"), SDF_datahandler->GetMapSize());
		}

		return EventResult::kContinue;
	}






	bool ObjLoadHandler::RegisterObjLoadEvent()
	{
		static  ObjLoadHandler g_ObjLoadhandler;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

		if (!ScriptEventSource)
		{
			logger::error("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&g_ObjLoadhandler);

		logger::info("Register Object Load Event Handler!");

		return true;
	}



	EventResult ObjLoadHandler::ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource)
		{
			logger::error("Event Source Not Found!");
			return EventResult::kContinue;
		}

		if (!a_event->loaded)
		{
			auto SDF_datahandler = DataHandler::GetSingleton();

			if (!SDF_datahandler)
			{
				logger::error("SDF_datahandler Not Found!");
				return EventResult::kContinue;

			}

			if (SDF_datahandler->GetMapSize() > 0)	 //Check If the Map Need to be Clear.
			{
				if (SDF_datahandler->HasHitAngleRecord(a_event->formID))
				{
					SDF_datahandler->RemoveHitAngleRecord(a_event->formID);
					logger::info(FMT_STRING("Hit Angle Record \"{}\" is Forced Removed!"), a_event->formID);
				}
			}
		}

		return EventResult::kContinue;
	}




}