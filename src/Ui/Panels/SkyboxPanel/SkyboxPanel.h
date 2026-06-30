#pragma once
#include "../../../Includes.h"
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"




namespace Ui{
    class SkyboxPanel{
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, int windowWidth, int windowHeight);
    };
}
