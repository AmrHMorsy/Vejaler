#pragma once
#include "../../../Includes.h"



struct TextureConfig{
    bool load;
    bool mipMapped;
    int resolution;
    int bindingIndex;
    std::string name;
    std::string filePath;
    uint32_t arrayLayers;
    glm::ivec4 channelMap;
    std::string uiButtonID;
    VkFormat textureFormat;
    glm::ivec4 mergeIndices;
    VkImageViewType viewType;
    VkImageLayout finalLayout;
    VkImageCreateFlags imageFlag;
    std::string uiResolutionButtonID;
    VkSamplerAddressMode addressMode;
    VkImageLayout intermediateLayout;
};
