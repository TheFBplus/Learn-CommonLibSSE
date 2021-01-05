#pragma once
#include "include/SDFix_Events.h"

namespace SDFix
{

	inline bool RegiterAnimEvent(RE::Actor* thisactor, RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_eventSink)
	{
		auto  middlehighprocess = thisactor->currentProcess->middleHigh;

		if (!middlehighprocess)
		{
			logger::error("middlehighprocess Not Found!");
			return false;

		}

		auto thisAGmanager = middlehighprocess->animationGraphManager.get();

		if (!thisAGmanager)
		{
			logger::error("thisAGmanager Not Found!");
			return false;

		}

		auto thisgraph = thisAGmanager->graphs.begin()->get();;

		if (thisgraph)
		{
			thisgraph->AddEventSink(a_eventSink);
			return true;
		}

		return false;
	}

}