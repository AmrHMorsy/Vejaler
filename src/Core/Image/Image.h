#pragma once
#include "../Memory/Memory.h"
#include "../Buffer/Buffer.h"
#include "../PipelineBarrier/PipelineBarrier.h"

#include "Data/ImageInfo.h"
#include "Data/ImageBlitConfiguration.h"
#include "Data/BuildRawImageConfiguration.h"
#include "Data/BuildImageWithDataConfiguration.h"


class Image{
    
private:
    
    static void FillImageBlit(const ImageBlitRegionConfiguration& configuration, VkImageBlit& imageBlitRegion);

public:
    
    static ImageInfo BuildRawImage(BuildRawImageConfiguration configuration) ;
    static ImageInfo BuildImageWithData(BuildImageWithDataConfiguration configuration);
    static void BlitImage(ImageBlitConfiguration configuration);
    static void Destroy(VkDevice logicalDevice, ImageInfo image);
} ;
