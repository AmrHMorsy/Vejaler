#include "CommandBuffer.h"




std::vector<VkCommandBuffer> CommandBuffer::Allocate(VkDevice logicalDevice, VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t numCommandBuffers)
{
    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = level;
    commandBufferAllocateInfo.commandBufferCount = numCommandBuffers;
    
    std::vector<VkCommandBuffer> commandBuffers(numCommandBuffers);
    
    VkResult result = vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffers.data());
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO ALLOCATE COMMAND BUFFERS\n\033[0m");
    
    return commandBuffers;
}

void CommandBuffer::Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags flag, const VkCommandBufferInheritanceInfo * pInheritanceInfo)
{
    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags = flag;
    commandBufferBeginInfo.pInheritanceInfo = pInheritanceInfo;
    
    VkResult result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO BEGIN RECORDING COMMAND BUFFER\033[0m\n");
}

void CommandBuffer::End(VkCommandBuffer commandBuffer)
{
    VkResult result = vkEndCommandBuffer(commandBuffer);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO END RECORDING COMMAND BUFFER\033[0m\n" ) ;
}

void CommandBuffer::Reset(VkCommandBuffer commandBuffer)
{
    VkResult result = vkResetCommandBuffer(commandBuffer, 0);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO RESET COMMAND BUFFER\033[0m\n");
}

void CommandBuffer::Free(VkDevice logicalDevice, VkCommandPool commandPool, std::vector<VkCommandBuffer> commandBuffers)
{
    vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data()) ;
}

void CommandBuffer::Execute(VkCommandBuffer primaryCommandBuffer, std::vector<VkCommandBuffer> secondaryCommandBuffers)
{
    vkCmdExecuteCommands(primaryCommandBuffer, static_cast<uint32_t>(secondaryCommandBuffers.size()), secondaryCommandBuffers.data());
}
