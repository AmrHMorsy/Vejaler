#pragma once
#include "SceneInfo.h"



struct ScenePack{
    int selected = 0;
    int nextSelected = 0;
    std::vector<SceneInfo> scenes;
    std::string savedFilePath = "";
    std::vector<std::string> names;
    std::vector<const char *> namesPtr;
};
