#include "Variable.h"
#include "Data/VariableConstants.h"



void Variable::Load(nlohmann::json j, std::vector<VariableInfo>& variables)
{
    if(!j.contains("variables"))
        return;
    
    for(size_t i = 0; i < j["variables"].size(); i++){
        VariableInfo v;
        v.name = j["variables"][i]["name"];
        v.type = VariableTypes[j["variables"][i]["type"]];
        v.value = GetValue(j["variables"][i]["value"]);
        variables.push_back(v);
    }
}

std::variant<int, std::string, float> Variable::GetValue(nlohmann::json j)
{
    if(j.is_number_integer())
        return j.get<int>();
    
    if(j.is_string())
        return j.get<std::string>();
    
    if(j.is_number_float())
        return j.get<float>();
    
    throw std::runtime_error("Unsupported std::variant type");
}

void Variable::ChangeValue(VariableInfo& variable, ActionInfo& action)
{
    if(variable.type == INT)
        variable.value = std::get<int>(action.value);
    
    else if(variable.type == STRING)
        variable.value = std::get<std::string>(action.value);
}
