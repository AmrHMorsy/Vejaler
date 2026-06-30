#pragma once
#include "../../../Includes.h"
#include "../../../Scene/Model/Data/ModelPack.h"
#include "../../../Scene/Light/Data/LightPack.h"
#include "../../../Scene/Skybox/Data/SkyboxPack.h"
#include "../../../Framework/Context/Data/ContextInfo.h"




namespace Ui{
    class TransformPanel{
    public:
        static void Render(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, SkyboxPack& skyboxPack);
    };
}
