#include "Fence.h"



VkFence Fence::Create(VkDevice logicalDevice, VkFenceCreateFlags flags)
{
    VkFenceCreateInfo fenceCreateInfo{};
    
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = flags;
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    VkFence fence;
    VkResult result = vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fence);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE FENCES\n\033[0m");
    
    return fence;
}

void Fence::Wait(VkDevice logicalDevice, const std::vector<VkFence>& fences, VkBool32 waitAll, uint64_t timeout)
{
    vkWaitForFences(logicalDevice, static_cast<uint32_t>(fences.size()), fences.data(), waitAll, timeout);
}

void Fence::Reset(VkDevice logicalDevice, const std::vector<VkFence>& fences)
{
    vkResetFences(logicalDevice, static_cast<uint32_t>(fences.size()), fences.data()) ;
}

void Fence::Destroy(VkDevice logicalDevice, VkFence fence)
{
    vkDestroyFence(logicalDevice, fence, NULL) ;
}
