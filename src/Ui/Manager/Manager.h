#pragma once
#include "../../Includes.h"
#include "Data/ManagerInfo.h"
#include "../../Scene/Scene/Data/ScenePack.h"
#include "../../Core/Window/Data/WindowInfo.h"
#include "../../Application/Data/ApplicationInfo.h"
#include "../../Framework/Context/Data/ContextInfo.h"



namespace Ui{
    class  Manager{
    public:
        static void Build(Ui::ManagerInfo& ui, ContextInfo& context, WindowInfo& window);
        static void Init(ContextInfo& context, WindowInfo& window, Ui::ManagerInfo& ui);
        static void Load(WindowInfo& window, Ui::ManagerInfo& ui, nlohmann::json j);
        static void HandleWindowResize(ContextInfo& context, Ui::ManagerInfo& ui);
        static void Record(ApplicationInfo& application);
        static void Submit(Ui::ManagerInfo& ui, ScenePack& scenePack, WindowInfo& window, uint32_t frame, ContextInfo& context);
        static void Destroy();
    }; 
}
