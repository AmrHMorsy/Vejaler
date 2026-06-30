#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"



namespace Ui{
    class LightAddPanel {
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, float windowWidth, float windowHeight);
    };
}
