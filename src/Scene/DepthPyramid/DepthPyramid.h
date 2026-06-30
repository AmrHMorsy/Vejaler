#pragma once
#include "../../Includes.h"
#include "../../Core/Core.h"

#include "Data/DepthPyramidInfo.h"
#include "Data/DepthPyramidConstants.h"
#include "Data/DepthPyramidShaderUniform.h"

#include "../DepthPrepass/Data/DepthPrepassInfo.h"
#include "../FrustumCulling/Data/FrustumCullingInfo.h"




class DepthPyramid{
    
public:
    
    static void Build(ContextInfo& context, DepthPyramidInfo& depthPyramid, DepthPrepassInfo& depthPrepass);
    static void Record(ContextInfo& context, DepthPyramidInfo& depthPyramid, uint32_t frame);
    static void Submit(ContextInfo& context, DepthPyramidInfo& depthPyramid, FrustumCullingInfo& frustumCulling, uint32_t frame);
    static void HandleWindowResize(ContextInfo& context, DepthPrepassInfo& depthPrepass, DepthPyramidInfo& depthPyramid);
};
