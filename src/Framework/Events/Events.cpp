#include "Events.h"
#include "Data/EventConstants.h"



void Events::Handle(ContextInfo& context, GLFWwindow * window, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, std::vector<VariableInfo>& variables)
{
    glfwPollEvents();
    
    StateInfo& state = statePack.states[statePack.active];
    
    for(size_t i = 0; i < state.events.size(); i++)
        eventHandlers[state.events[i].type](context, scenePack, statePack, ui, textPack, window, variables, state.events[i]);
}

void Events::Load(nlohmann::json e, std::vector<EventInfo>& events)
{
    if(!e.contains("events"))
        return;
    
    for(size_t i = 0; i < e["events"].size(); i++){
        EventInfo event;
        
        if(e["events"][i].contains("soundEffect")){
            event.soundEffect = true;
            event.soundEffectName = e["events"][i]["soundEffect"];
        }
        
        if(e["events"][i].contains("key")){
            event.key = e["events"][i]["key"];
            event.glfwKey = glfwKeys[event.key];
            event.type = KEYBOARD;
            if(e["events"][i]["type"] == "PRESS")
                event.keyPressType = PRESS;
            else if(e["events"][i]["type"] == "HOLD")
                event.keyPressType = HOLD;
        }
        else if(e["events"][i].contains("uiElement")){
            event.target = e["events"][i]["uiElement"];
            if(e["events"][i]["eventType"] == "FLAG_CHANGE")
                event.type = FLAG_CHANGE;
            event.oldValue = GetValue(e["events"][i]["oldValue"]);
            event.newValue = GetValue(e["events"][i]["newValue"]);
        }
        else if(e["events"][i].contains("text")){
            event.target = e["events"][i]["text"];
            if(e["events"][i]["eventType"] == "CLICK")
                event.type = TEXT_CLICK;
            else if(e["events"][i]["eventType"] == "ENTER")
                event.type = TEXT_ENTER;
        }
        
        for(size_t k = 0; k < e["events"][i]["actions"].size(); k++){
            ActionInfo action;
            
            if(e["events"][i]["actions"][k].contains("entity"))
                action.entity = actionEntities[e["events"][i]["actions"][k]["entity"]];
            
            if(e["events"][i]["actions"][k].contains("type"))
                action.type = actionTypes[e["events"][i]["actions"][k]["type"]];
            
            if(e["events"][i]["actions"][k].contains("subType"))
                action.subType = actionTypes[e["events"][i]["actions"][k]["subType"]];
            
            if(e["events"][i]["actions"][k].contains("value"))
                action.value = GetValue(e["events"][i]["actions"][k]["value"]);
            
            if(e["events"][i]["actions"][k].contains("target"))
                action.target = e["events"][i]["actions"][k]["target"];
            
            event.actions.push_back(action);
        }
        
        events.push_back(event);
    }
}

std::variant<float, int, std::string, bool, glm::vec3> Events::GetValue(nlohmann::json j)
{
    if(j.is_number_integer())
        return j.get<int>();
    
    if(j.is_string())
        return j.get<std::string>();
    
    if(j.is_boolean())
        return j.get<bool>();
    
    if(j.is_number_float())
        return j.get<float>();
    
    if(j.is_array() && j.size() == 3 &&
       j[0].is_number() && j[1].is_number() && j[2].is_number())
        return glm::vec3(j[0].get<float>(), j[1].get<float>(), j[2].get<float>());
    
    throw std::runtime_error("Unsupported std::variant type");
}
