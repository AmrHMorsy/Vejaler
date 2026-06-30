#include "CommandPool.h"



VkCommandPool CommandPool::Create(VkDevice logicalDevice, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = flags;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
    
    VkCommandPool commandPool;
    VkResult result = vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE COMMAND POOL\n\033[0m");

    return commandPool;
}

void CommandPool::Destroy(VkDevice logicalDevice, VkCommandPool commandPool)
{
    vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
}
