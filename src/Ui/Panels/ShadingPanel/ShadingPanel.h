#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Application/Data/ApplicationInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"





namespace Ui{
    class ShadingPanel{
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, int windowWidth, ApplicationInfo& application);
    };
}
