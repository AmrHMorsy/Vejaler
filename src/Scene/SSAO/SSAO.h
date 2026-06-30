#pragma once
#include "Data/SSAOInfo.h"
#include "../../Includes.h"
#include "../Texture/Texture.h"
#include "../../Core/Core.h"
#include "../Scene/Data/SceneInfo.h"



class SSAO{
    
private:
    
    static float Random(float min, float max);
    static uint16_t FloatToHalf(float f);
    
public:
    
    static void Build(ContextInfo& context, SSAOInfo& ssao, DepthPrepassInfo& depthPrepass, NormalPassInfo& normalPass, ModelPack& modelPack, VkSampler& nearestClampIntOpaqueBlackSampler);
    static void Record(ContextInfo& context, SSAOInfo& ssao, CameraPack& cameraPack, uint32_t frame);
    static void Submit(SSAOInfo& ssao, NormalPassInfo& normalPass, ContextInfo& context, uint32_t frame);
    static void HandleWindowResize(ContextInfo& context, SSAOInfo& ssao, NormalPassInfo& normalPass, DepthPrepassInfo& depthPrepass, ModelPack& modelPack, VkSampler& nearestClampIntOpaqueBlackSampler);
};
