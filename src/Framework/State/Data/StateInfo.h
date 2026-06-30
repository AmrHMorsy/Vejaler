#pragma once
#include "../../../Includes.h"
#include "../../Events/Data/EventInfo.h"



struct StateInfo{
    uint ID;
    uint scene;
    bool isAcceptingEvents = true;
    std::vector<EventInfo> events;
};
