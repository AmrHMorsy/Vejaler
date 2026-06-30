#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Scene/Light/Data/LightPack.h"
#include "../../../Scene/Model/Data/ModelPack.h"
#include "../../../Scene/Skybox/Data/SkyboxPack.h"
#include "../../../Application/Data/ApplicationInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"


namespace Ui{
    class ModelPanel{
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, ModelPack& modelPack, SkyboxPack& skyboxPack, LightPack& lightPack, int windowWidth, int windowHeight, ApplicationInfo& application);
    };
}
