#pragma once
#include "../../Includes.h"
#include "Data/VariableInfo.h"
#include "../Events/Data/ActionInfo.h"




class Variable{
    
public:
    
    static void ChangeValue(VariableInfo& variable, ActionInfo& action);
    static void Load(nlohmann::json j, std::vector<VariableInfo>& variables);
    static std::variant<int, std::string, float> GetValue(nlohmann::json j);
};
