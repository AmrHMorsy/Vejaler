#pragma once
#include "../../../Includes.h"
#include "StateInfo.h"



struct StatePack{
    int active = 0;
    std::vector<StateInfo> states;
};
