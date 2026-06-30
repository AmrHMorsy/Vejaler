#pragma once
#include "../../Includes.h"
#include "../../Core/Core.h"
#include "Data/OcclusionCullingInfo.h"
#include "../Model/Data/ModelPack.h"
#include "Data/OcclusionCullingConstants.h"
#include "Data/OcclusionCullingShaderUniform.h"
#include "../DepthPyramid/Data/DepthPyramidInfo.h"




class OcclusionCulling{
    
public:
    
    static void Build(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, DepthPyramidInfo& depthPyramid);
    static void Load(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, ModelPack& modelPack, FrustumCullingInfo& frustumCulling);
    static void Record(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, ModelPack& modelPack, uint32_t frame);
    static void Submit(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, DepthPyramidInfo& depthPyramid, uint32_t frame);
};
