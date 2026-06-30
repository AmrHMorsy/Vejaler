#pragma once
#include "ActionInfo.h"
#include "../../../Includes.h"



struct EventInfo{
    int type;
    int target;
    int glfwKey;
    std::string key;
    int keyPressType;
    bool soundEffect = false;
    std::string soundEffectName;
    bool previousKeyState = false;
    std::vector<ActionInfo> actions;
    std::variant<float, int, std::string, bool, glm::vec3> oldValue;
    std::variant<float, int, std::string, bool, glm::vec3> newValue;
};
