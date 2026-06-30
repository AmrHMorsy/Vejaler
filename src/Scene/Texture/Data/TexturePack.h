#pragma once
#include "../../../Includes.h"
#include "../../../Core/Image/Data/ImageInfo.h"
#include "TextureConfig.h"
#include "../../SourceImage/Data/SourceImageInfo.h"



struct TexturePack{
    VkSampler sampler;
    ImageInfo image;
    TextureConfig config;
    VkImageView imageView;
    ImTextureID uiTextureID;
    SourceImageInfo sourceImage;
    std::vector<std::string> resolutionStrings;
    std::vector<const char *> resolutionStringsPtr;
};
