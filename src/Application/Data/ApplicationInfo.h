#pragma once
#include "../../Includes.h"
#include "../../Ui/Text/Data/TextPack.h"
#include "../../Scene/Scene/Data/ScenePack.h"
#include "../../Ui/Manager/Data/ManagerInfo.h"
#include "../../Core/Window/Data/WindowInfo.h"
#include "../../Framework/State/Data/StatePack.h"
#include "../../Framework/Music/Data/MusicPack.h"
#include "../../Framework/Context/Data/ContextInfo.h"
#include "../../Framework/Variable/Data/VariableInfo.h"




struct ApplicationInfo{
    WindowInfo window;
    uint32_t frame = 0;
    Ui::ManagerInfo ui;
    ScenePack scenePack;
    MusicPack musicPack;
    StatePack statePack;
    ContextInfo context;
    Ui::TextPack textPack;
    nlohmann::json gameJSON;
    std::vector<VariableInfo> variables;
};
