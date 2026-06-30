#pragma once
#include "Data/StatePack.h"
#include "../Events/Events.h"


class State{
    
public:
    
    static void Load(nlohmann::json j, ScenePack& scenePack, StatePack& statePack);
    static void Transition(StatePack& statePack, ScenePack& scenePack, int newSelected);
};
