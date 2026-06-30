#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"



namespace Ui{
    class CameraAddPanel{
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, VkExtent2D swapChainExtent, float windowWidth, float windowHeight);
    };
}
