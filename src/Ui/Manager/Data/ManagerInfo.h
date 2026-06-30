#pragma once
#include "../../../Includes.h"
#include "../../Element/Data/ElementInfo.h"



namespace Ui{
    struct ManagerInfo{
        VkRenderPass renderPass;
        VkCommandPool commandPool;
        ImFont * font, * boldFont;
        std::vector<ElementInfo> elements;
        std::vector<VkFence> inflightFences;
        std::vector<VkSemaphore> waitSemaphores;
        std::vector<VkFramebuffer> frameBuffers;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkPipelineStageFlags> waitStages;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
    }; 
}
