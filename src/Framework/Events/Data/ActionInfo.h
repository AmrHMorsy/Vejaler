#pragma once
#include "../../../Includes.h"



struct ActionInfo{
    int type;
    int subType;
    int entity;
    uint target;
    std::variant<float, int, std::string, bool, glm::vec3> value;
};
