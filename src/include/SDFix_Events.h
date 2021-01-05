#pragma once

namespace SDFix
{
	using EventResult = RE::BSEventNotifyControl;


	class  EffectStartEventHandler : public RE::BSTEventSink <RE::TESMagicEffectApplyEvent>
	{
		
	public:

		virtual EventResult ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource);

		static bool RegisterEffectStartEvent();


	private:

		  EffectStartEventHandler() = default;

		 ~ EffectStartEventHandler() = default;

		  EffectStartEventHandler(const EffectStartEventHandler&) = delete;

		  EffectStartEventHandler(EffectStartEventHandler&&) = delete;

		  EffectStartEventHandler& operator= (const EffectStartEventHandler&) = delete;

		  EffectStartEventHandler& operator= (EffectStartEventHandler&&) = delete;
	};




	class AnimationGraphEventHanlder : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
	{

	public:

		virtual EventResult ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource);

		static AnimationGraphEventHanlder* GetSingleton()
		{
			static AnimationGraphEventHanlder thishandler;

			return &thishandler;
		}


	private:

		AnimationGraphEventHanlder() = default;

		~AnimationGraphEventHanlder() = default;
	};



	class ObjLoadHandler : public RE::BSTEventSink<RE::TESObjectLoadedEvent>
	{
	public:

		virtual EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource);

		static bool RegisterObjLoadEvent();


	private:

		ObjLoadHandler() = default;

		~ObjLoadHandler() = default;

		ObjLoadHandler(const ObjLoadHandler&) = delete;

		ObjLoadHandler(ObjLoadHandler&&) = delete;

		ObjLoadHandler& operator= (const ObjLoadHandler&) = delete;

		ObjLoadHandler& operator= (ObjLoadHandler&&) = delete;
	};


}