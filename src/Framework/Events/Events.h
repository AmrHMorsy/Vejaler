#pragma once
#include "../../Ui/Manager/Manager.h"
#include "../State/State.h"
#include "../../Scene/Scene/Scene.h"




class Events{
    
private:
    
    static std::variant<float, int, std::string, bool, glm::vec3> GetValue(nlohmann::json j);
    
public:
    
    static void Load(nlohmann::json e, std::vector<EventInfo>& events);
    static void Handle(ContextInfo& context, GLFWwindow * window, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, std::vector<VariableInfo>& variables);
};
