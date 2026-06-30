#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"




namespace Ui{
    class LightPanel{
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, int windowWidth, int windowHeight);
    };
}
