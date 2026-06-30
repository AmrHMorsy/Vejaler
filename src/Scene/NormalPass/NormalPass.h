#pragma once
#include "../../Includes.h"
#include "Data/NormalPassInfo.h"
#include "../../Core/Core.h"
#include "../Scene/Data/SceneInfo.h"



class NormalPass{
    
public:
    
    static void Build(ContextInfo& context, NormalPassInfo& normalPass, DepthPrepassInfo& depthPrepass, ModelPack& modelPack);
    static void Load(ContextInfo& context, NormalPassInfo& normalPass, ModelPack& modelPack);
    static void Record(SceneInfo& scene, ContextInfo& context, uint32_t frame);
    static void Submit(NormalPassInfo& normalPass, LightCullingInfo& lightCulling, ContextInfo& context, uint32_t frame);
    static void HandleWindowResize(ContextInfo& context, NormalPassInfo& normalPass, DepthPrepassInfo& depthPrepass, ModelPack& modelPack);
};
