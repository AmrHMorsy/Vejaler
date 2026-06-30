#pragma once
#include "Data/ImageViewBuildInfo.h"


class ImageView{
    
public:
    
    static VkImageView Build(ImageViewBuildInfo info) ;
    static void Destroy(VkDevice logicalDevice, VkImageView imageView);
} ;
