#pragma once
#include "../../../Includes.h"



struct VariableInfo{
    int type;
    std::string name;
    std::variant<int, std::string, float> value;
};
