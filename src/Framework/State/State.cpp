#include "State.h"




void State::Load(nlohmann::json j, ScenePack& scenePack, StatePack& statePack)
{
    if(!j.contains("states"))
        throw std::runtime_error("\033[31m\nERROR::NO STATES IN JSON FILE\033[0m");
    
    for(size_t i = 0; i < j["states"].size(); i++){
        StateInfo state;
        state.ID = j["states"][i]["ID"];
        state.scene = j["states"][i]["scene"];
        Events::Load(j["states"][i], state.events);
        statePack.states.push_back(state);
    }
    
    if(statePack.states.empty())
        throw std::runtime_error("\033[31m\nERROR::NO STATES IN JSON FILE\033[0m");
    
    scenePack.nextSelected = statePack.states[statePack.active].scene;
}

void State::Transition(StatePack& statePack, ScenePack& scenePack, int newSelected)
{
    statePack.active = newSelected;
    scenePack.nextSelected = statePack.states[statePack.active].scene;
}
