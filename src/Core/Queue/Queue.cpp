#include "Queue.h"



QueuePack Queue::GetDeviceQueue(GetDeviceQueueConfiguration configuration)
{
    VkQueue graphicsQueue;
    vkGetDeviceQueue(configuration.logicalDevice, configuration.queueIndex, 0, &graphicsQueue);
    
    VkQueue presentQueue;
    vkGetDeviceQueue(configuration.logicalDevice, configuration.queueIndex, 0, &presentQueue);

    return QueuePack{
        .presentQueue = presentQueue,
        .graphicsQueue = graphicsQueue
    };
}

void Queue::SubmitQueue(SubmitQueueConfiguration configuration)
{
    std::vector<VkSubmitInfo> submitInfos(configuration.submissionInfos.size());
    
    for(size_t i = 0 ; i < configuration.submissionInfos.size() ; i++){
        submitInfos[i] = VkSubmitInfo{};
        submitInfos[i].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfos[i].commandBufferCount = static_cast<uint32_t>(configuration.submissionInfos[i].commandBuffers.size());
        submitInfos[i].pCommandBuffers = configuration.submissionInfos[i].commandBuffers.data();
        submitInfos[i].waitSemaphoreCount = static_cast<uint32_t>(configuration.submissionInfos[i].waitSemaphores.size());
        
        if(submitInfos[i].waitSemaphoreCount > 0){
            submitInfos[i].pWaitSemaphores = configuration.submissionInfos[i].waitSemaphores.data();
            submitInfos[i].pWaitDstStageMask = configuration.submissionInfos[i].waitStages.data();
        }
        else{
            submitInfos[i].pWaitSemaphores = nullptr;
            submitInfos[i].pWaitDstStageMask = nullptr;
        }
        
        submitInfos[i].signalSemaphoreCount = static_cast<uint32_t>(configuration.submissionInfos[i].signalSemaphores.size());

        if(submitInfos[i].signalSemaphoreCount > 0)
            submitInfos[i].pSignalSemaphores = configuration.submissionInfos[i].signalSemaphores.data();
        else
            submitInfos[i].pSignalSemaphores = nullptr;
    }

    VkResult result = vkQueueSubmit(configuration.graphicsQueue, static_cast<uint32_t>(submitInfos.size()), submitInfos.data(), configuration.fence) ;
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO SUBMIT QUEUE\033[0m\n");
}

void Queue::WaitIdle(VkQueue graphicsQueue)
{
    vkQueueWaitIdle(graphicsQueue) ;
}
