#pragma once
#include "../../../Includes.h"
#include "../../../Core/Core.h"
#include "../../Texture/Data/TexturePack.h"
#include "../../IBL/IBL/Data/IBLInfo.h"
#include "HDRToCubemapVertexShaderUniformVariables.h"
#include "SkyboxVertexShaderUniformVariables.h"


struct SkyboxInfo{
    std::string filePath;
    SkyboxVertexShaderUniformVariables vsData;

    bool isHDRTonemappingEnabled = true;
    bool isGammaCorrectEnabled = true;
    
    bool ready = false;
    IBLInfo ibl;
    std::string name;
    int selectedResolution = 0;
    std::vector<std::string> resolutionStrings;
    std::vector<const char *> resolutionStringsPtr;
    
    
    TexturePack skyboxTexturePack;
    BufferPack skyboxVertexBufferInfo;
    VkDescriptorPool skyboxDescriptorPool;
    std::vector<VkImageView> skyboxImageViews;
    std::vector<VkDescriptorSet> skyboxDescriptorSets;
    std::vector<UniformBufferPack> skyboxUniformBufferInfo;
    std::vector<std::vector<DescriptorBinding>> skyboxDescriptorBindings;
    
    VkFence hdrToCubemapFence;
    TexturePack hdrToCubemapTextureInfo;
    VkDescriptorPool hdrToCubemapDescriptorPool;
    std::vector<UniformBufferPack> hdrToCubemapUniformBuffers;
    std::vector<VkExtent2D> hdrToCubemapResolutions;
    std::vector<VkDescriptorSet> hdrToCubemapDescriptorSets;
    std::vector<std::vector<VkFramebuffer>> hdrToCubemapFrameBuffers;
    std::vector<std::vector<DescriptorBinding>> hdrToCubemapDescriptorBindings;
    std::vector<HDRToCubemapVertexShaderUniformVariables> hdrToCubemapVertexShaderUniformDatas;
    VkSemaphore hdrToCubemapPreFilteredEnvironmentSemaphore;
    VkSemaphore hdrToCubemapIrradianceSemaphore;
} ;
