#pragma once
#include "../../Model/Data/ModelPack.h"
#include "../../Light/Data/LightPack.h"
#include "../../Camera/Data/CameraPack.h"
#include "../../Skybox/Data/SkyboxPack.h"
#include "../../../Core/Core.h"
#include "../../FrustumCulling/FrustumCulling.h"
#include "../../../../Resources/external/include/miniaudio/miniaudio.h"
#include "../../NormalPass/Data/NormalPassInfo.h"
#include "../../DepthPrepass/Data/DepthPrepassInfo.h"
#include "../../SSAO/Data/SSAOInfo.h"
#include "../../FrustumCulling/Data/FrustumCullingInfo.h"
#include "../../OcclusionCulling/Data/OcclusionCullingInfo.h"
#include "../../DepthPyramid/Data/DepthPyramidInfo.h"



struct SceneInfo{
    SSAOInfo ssao;
    LightPack lightPack;
    ModelPack modelPack;
    CameraPack cameraPack;
    SkyboxPack skyboxPack;
    NormalPassInfo normalPass;
    DepthPrepassInfo depthPrepass;
    FrustumCullingInfo frustumCulling;
    OcclusionCullingInfo occlusionCulling;
    LightCullingInfo lightCulling;
    DepthPyramidInfo depthPyramid;

    
    
    
    
    float gammaExponent = 2.2f;
    VkClearColorValue backgroundColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
    bool enableRendering = true;
    float brightness = 1.0f;
    VkRenderPass renderPass;
    VkExtent2D screenResolution;
    VkCommandPool primaryCommandPool;
    std::vector<VkFence> inflightFences;
    std::vector<VkFramebuffer> frameBuffers;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    bool frustumCullingEnable = true;
    std::string editingMode = "Camera Mode";
};
