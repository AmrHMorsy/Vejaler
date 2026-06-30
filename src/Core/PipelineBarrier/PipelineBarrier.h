#pragma once
#include "../Queue/Queue.h"
#include "../Fence/Fence.h"
#include "../CommandBuffer/CommandBuffer.h"
#include "Data/ImageMemoryBarrierConfiguration.h"
#include "Data/InsertPipelineBarrierConfiguration.h"
#include "Data/InsertImageMemoryBarrierConfiguration.h"



class PipelineBarrier{
      
private:
    
    static void GetAccessFlags(VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags& srcAccessMask, VkAccessFlags& dstAccessMask);
    static VkImageMemoryBarrier FillImageMemoryBarrier(ImageMemoryBarrierConfiguration configuration);
    static void GetPipelineStageFlags(VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags& srcStageMask, VkPipelineStageFlags& dstStageMask);
    static void InsertPipelineBarrier(InsertPipelineBarrierConfiguration configuration) ;

public:
        
    static void InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration configuration);
};
