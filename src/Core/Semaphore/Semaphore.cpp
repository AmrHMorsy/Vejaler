#include "Semaphore.h"




VkSemaphore Semaphore::Build(VkDevice logicalDevice)
{
    VkSemaphoreCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
    VkSemaphore semaphore ;
    VkResult result = vkCreateSemaphore(logicalDevice, &info, nullptr, &semaphore);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE SEMAPHORES\033[0m");
    
    return semaphore;
}

void Semaphore::Destroy(VkDevice logicalDevice, VkSemaphore semaphore)
{
    vkDestroySemaphore(logicalDevice, semaphore, NULL);
}
