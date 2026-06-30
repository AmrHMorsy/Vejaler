#pragma once
#include "../../../Includes.h"



struct BindIndexBufferConfiguration{
    VkIndexType type;
    VkDeviceSize offset;
    VkBuffer indexBuffer;
    VkCommandBuffer commandBuffer;
};
