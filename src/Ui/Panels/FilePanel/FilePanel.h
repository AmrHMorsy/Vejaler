#pragma once
#include "../../../Scene/Scene/Data/ScenePack.h"
#include "../../../Application/Data/ApplicationInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"



namespace Ui{
    class FilePanel{
    public:
        static void Render(ScenePack& scenePack, ContextInfo& context, int windowWidth, int windowHeight, ApplicationInfo& application);
    };
}
