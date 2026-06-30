#pragma once
#include "../Model/Model.h".
#include "../../Core/Core.h"
#include "Data/DepthPrepassInfo.h"
#include "../Scene/Data/SceneInfo.h"
#include "../Model/Data/ModelPack.h"
#include "Data/DepthPrepassConstants.h"



class DepthPrepass{
    
public:
    
    static void Build(ContextInfo& context, DepthPrepassInfo& depthPrepass);
    static void Record(ContextInfo& context, DepthPrepassInfo& depthPrepass, OcclusionCullingInfo& occlusionCulling, ModelPack& modelPack, CameraPack& cameraPack, uint32_t frame);
    static void Load(ContextInfo& context, DepthPrepassInfo& depthPrepass, ModelPack& modelPack);
    static void UpdateDescriptorSets(ContextInfo& context, DepthPrepassInfo& depthPrepass);
    static void Submit(DepthPrepassInfo& depthPrepass, OcclusionCullingInfo& occlusionCulling, ContextInfo& context, uint32_t frame);
    static void HandleWindowResize(ContextInfo& context, DepthPrepassInfo& depthPrepass);
};
