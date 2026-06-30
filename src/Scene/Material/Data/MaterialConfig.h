#pragma once
#include "../../../Includes.h"



struct MaterialConfig{
    
    TextureConfig albedo{
        .resolution = 64,
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = true,
        .name = "Albedo",
        .load = true,
        .uiButtonID = "##Albedo",
        .uiResolutionButtonID = "##AlbedoTexture",
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .textureFormat = VK_FORMAT_R8G8B8A8_SRGB,
        .filePath = "../Resources/assets/Default/Albedo.png",
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    
    TextureConfig normal{
        .resolution = 64,
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = true,
        .name = "Normal",
        .load = true,
        .uiButtonID = "##Normal",
        .uiResolutionButtonID = "##NormalTexture",
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .filePath = "../Resources/assets/Default/Normal.png",
        .textureFormat = VK_FORMAT_R8G8B8A8_UNORM,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    
    TextureConfig roughness{
        .resolution = 64,
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = true,
        .name = "Roughness",
        .load = true,
        .uiButtonID = "##Roughness",
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .uiResolutionButtonID = "##RoughnessTexture",
        .filePath = "../Resources/assets/Default/Roughness.png",
        .textureFormat = VK_FORMAT_R8G8B8A8_UNORM,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    
    TextureConfig opacity{
        .resolution = 64,
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = true,
        .name = "Opacity",
        .load = true,
        .uiButtonID = "##Opacity",
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .uiResolutionButtonID = "##OpacityTexture",
        .filePath = "../Resources/assets/Default/Opacity.png",
        .textureFormat = VK_FORMAT_R8G8B8A8_UNORM,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    
    TextureConfig metallic{
        .resolution = 64,
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = true,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .name = "Metallic",
        .load = true,
        .textureFormat = VK_FORMAT_R8G8B8A8_UNORM,
        .uiButtonID = "##Metallic",
        .uiResolutionButtonID = "##MetallicTexture",
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .filePath = "../Resources/assets/Default/Metallic.png",
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    
    TextureConfig roughnessMetallicOpacity{
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = true,
        .bindingIndex = 3,
        .load = true,
        .channelMap = glm::ivec4(1, 2, 0, 0),
        .mergeIndices = glm::ivec4(2, 3, -1, -1),
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .name = "RoughnessMetallicOpacity",
        .textureFormat = VK_FORMAT_R8G8B8A8_UNORM,
        .uiButtonID = "##RoughnessMetallicOpacity",
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
};
