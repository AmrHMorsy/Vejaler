#pragma once
#include "../../Core/Core.h"
#include "Data/FrustumCullingInfo.h"
#include "../Model/Data/ModelPack.h"
#include "../Camera/Data/CameraInfo.h"




class FrustumCulling{
    
public:
    
    static void Build(ContextInfo& context, FrustumCullingInfo& frustumCulling, bool& frustumCullingEnable);
    static void Load(ContextInfo& context, FrustumCullingInfo& frustumCulling, ModelPack& modelPack);
    static void Record(ContextInfo& context, FrustumCullingInfo& frustumCulling, ModelPack& modelPack, CameraInfo& camera, uint32_t frame);
    static void Submit(ContextInfo& context, FrustumCullingInfo& frustumCulling, uint32_t frame);
};
