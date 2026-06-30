#pragma once
#include "../../../Includes.h"




struct InsertImageMemoryBarrierConfiguration{
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkCommandPool commandPool;
    VkDependencyFlags dependencyFlags;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    const std::vector<ImageMemoryBarrierConfiguration> imageMemoryBarrierConfigurations;
};
