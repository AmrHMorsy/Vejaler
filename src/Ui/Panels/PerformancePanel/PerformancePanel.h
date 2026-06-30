#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"



namespace Ui{
    class PerformancePanel {
    public:
        static void Render(SceneInfo& scene, int windowWidth, int windowHeight, uint32_t frame);
    };
}
