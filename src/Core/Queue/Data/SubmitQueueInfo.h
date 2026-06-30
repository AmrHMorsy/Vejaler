#pragma once
#include "../../../Includes.h"



struct SubmitQueueInfo{
    std::vector<VkSemaphore> waitSemaphores;
    std::vector<VkSemaphore> signalSemaphores;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkPipelineStageFlags> waitStages;
};
