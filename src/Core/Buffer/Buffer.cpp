#include "Buffer.h"




BufferPack Buffer::BuildRawBuffer(BuildRawBufferConfiguration configuration)
{
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    
    VkBufferCreateInfo bufferCreateInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = configuration.size,
        .usage = configuration.usage,
        .sharingMode = configuration.sharingMode
    };

    VkResult result = vkCreateBuffer(configuration.logicalDevice, &bufferCreateInfo, NULL, &buffer);
    
    if( result != VK_SUCCESS )
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE BUFFER\033[0m");

    VkMemoryRequirements memoryRequirements ;
    vkGetBufferMemoryRequirements(configuration.logicalDevice, buffer, &memoryRequirements);
    bufferMemory = Memory::AllocateMemory(MemoryAllocateInfo{
        .logicalDevice = configuration.logicalDevice,
        .physicalDevice = configuration.physicalDevice,
        .properties = configuration.properties,
        .memRequirements = memoryRequirements
    });
    vkBindBufferMemory(configuration.logicalDevice, buffer, bufferMemory, 0);
    
    return BufferPack{
        .buffer = buffer,
        .bufferMemory = bufferMemory
    } ;
}

BufferPack Buffer::BuildBufferWithData(BuildBufferWithDataConfiguration configuration)
{
    BufferPack stageBuffer = BuildRawBuffer(BuildRawBufferConfiguration{
        .size = configuration.bufferSize,
        .logicalDevice = configuration.logicalDevice,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = configuration.physicalDevice,
        .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    });

    UploadDataToBuffer(UploadDataToBufferConfiguration{
        .offset = 0,
        .memoryMapFlags = 0,
        .data = configuration.data,
        .dataSize = configuration.bufferSize,
        .bufferMemory = stageBuffer.bufferMemory,
        .logicalDevice = configuration.logicalDevice,
    });
    
    BufferPack dataBuffer = BuildRawBuffer(BuildRawBufferConfiguration{
        .size = configuration.bufferSize,
        .logicalDevice = configuration.logicalDevice,
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = configuration.physicalDevice,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    });
    
    CopyBuffer(CopyBufferConfiguration{
        .dstBuffer = dataBuffer.buffer,
        .srcBuffer = stageBuffer.buffer,
        .commandPool = configuration.commandPool,
        .graphicsQueue = configuration.graphicsQueue,
        .logicalDevice = configuration.logicalDevice,
        .copyRegions = std::vector<VkBufferCopy>{
            VkBufferCopy{
                .srcOffset = 0,
                .dstOffset = 0,
                .size = configuration.bufferSize
            }
        }
    });
    
    DestroyBuffer(DestroyBufferConfiguration{
        .logicalDevice = configuration.logicalDevice,
        .buffer = stageBuffer
    });
    
    return dataBuffer ;
}

BufferPack Buffer::BuildWithData(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, void * data, VkDeviceSize bufferSize)
{
    BufferPack stageBuffer = BuildRawBuffer(BuildRawBufferConfiguration{
        .size = bufferSize,
        .logicalDevice = logicalDevice,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = physicalDevice,
        .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    });

    UploadDataToBuffer(UploadDataToBufferConfiguration{
        .offset = 0,
        .memoryMapFlags = 0,
        .data = data,
        .dataSize = bufferSize,
        .bufferMemory = stageBuffer.bufferMemory,
        .logicalDevice = logicalDevice,
    });
    
    BufferPack dataBuffer = BuildRawBuffer(BuildRawBufferConfiguration{
        .size = bufferSize,
        .logicalDevice = logicalDevice,
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = physicalDevice,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    });
    
    CopyBuffer(CopyBufferConfiguration{
        .dstBuffer = dataBuffer.buffer,
        .srcBuffer = stageBuffer.buffer,
        .commandPool = commandPool,
        .graphicsQueue = graphicsQueue,
        .logicalDevice = logicalDevice,
        .copyRegions = std::vector<VkBufferCopy>{
            VkBufferCopy{
                .srcOffset = 0,
                .dstOffset = 0,
                .size = bufferSize
            }
        }
    });
    
    DestroyBuffer(DestroyBufferConfiguration{
        .logicalDevice = logicalDevice,
        .buffer = stageBuffer
    });
    
    return dataBuffer ;
}

HostVisibleBuffer Buffer::BuildHostVisibleBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usage)
{
    BufferPack bufferPack = BuildRawBuffer(BuildRawBufferConfiguration{
        .size = bufferSize,
        .logicalDevice = logicalDevice,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = physicalDevice,
        .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    });
        
    void * memoryPointer = MapMemory(MapMemoryConfiguration{
        .offset = 0,
        .dataSize = bufferSize,
        .bufferMemory = bufferPack.bufferMemory,
        .logicalDevice = logicalDevice,
        .memoryMapFlags = 0,
    });

    return HostVisibleBuffer{
        .bufferPack = bufferPack,
        .memoryPointer = memoryPointer
    };
}

void Buffer::CopyBuffer(CopyBufferConfiguration configuration)
{
    VkCommandBuffer commandBuffer = CommandBuffer::Allocate(configuration.logicalDevice, configuration.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
    
    CommandBuffer::Begin(commandBuffer, 0, nullptr);
    
    vkCmdCopyBuffer(commandBuffer, configuration.srcBuffer, configuration.dstBuffer, static_cast<uint32_t>(configuration.copyRegions.size()), configuration.copyRegions.data()) ;
    
    CommandBuffer::End(commandBuffer) ;

    VkFence fence = Fence::Create(configuration.logicalDevice, 0);
    
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = fence,
        .graphicsQueue = configuration.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {VK_NULL_HANDLE},
                .signalSemaphores = {VK_NULL_HANDLE},
                .commandBuffers = {commandBuffer},
                .waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}
            }
        }
    }) ;
    
    Fence::Wait(configuration.logicalDevice, {fence}, VK_TRUE, UINT64_MAX);

    Fence::Destroy(configuration.logicalDevice, fence);
    
    CommandBuffer::Free(configuration.logicalDevice, configuration.commandPool, {commandBuffer});
}

UniformBufferPack Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration configuration)
{
    BufferPack bufferPack = BuildRawBuffer(BuildRawBufferConfiguration{
        .size = configuration.bufferSize,
        .logicalDevice = configuration.logicalDevice,
        .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = configuration.physicalDevice,
        .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    });
    
    void * memoryPointer = MapMemory(MapMemoryConfiguration{
        .offset = 0,
        .dataSize = configuration.bufferSize,
        .bufferMemory = bufferPack.bufferMemory,
        .logicalDevice = configuration.logicalDevice,
        .memoryMapFlags = 0,
    });
    
    return UniformBufferPack{
        .bufferPack = bufferPack,
        .memoryPointer = memoryPointer
    };
}

void Buffer::BindVertexBuffer(BindVertexBufferConfiguration configuration)
{
    if(configuration.vertexBuffers.size() == 0)
        return;
    
    for(int i = 0; i < configuration.vertexBuffers.size(); i++){
        if(configuration.vertexBuffers[i] == VK_NULL_HANDLE)
            return;
    }
    
    vkCmdBindVertexBuffers(configuration.commandBuffer, configuration.firstBinding, static_cast<uint32_t>(configuration.vertexBuffers.size()), configuration.vertexBuffers.data(), configuration.offsets.data());
}

void Buffer::BindIndexBuffer(BindIndexBufferConfiguration configuration)
{
    if(configuration.indexBuffer == VK_NULL_HANDLE)
        return;
    
    vkCmdBindIndexBuffer(configuration.commandBuffer, configuration.indexBuffer, configuration.offset, configuration.type);
}

void Buffer::CopyBufferToImage(CopyBufferToImageConfiguration configuration)
{
    VkCommandBuffer commandBuffer = CommandBuffer::Allocate(configuration.logicalDevice, configuration.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
    
    CommandBuffer::Begin(commandBuffer, 0, nullptr);
    
    vkCmdCopyBufferToImage(commandBuffer, configuration.buffer, configuration.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(configuration.copyRegions.size()), configuration.copyRegions.data()) ;

    CommandBuffer::End(commandBuffer) ;
    
    VkFence fence = Fence::Create(configuration.logicalDevice, 0);
    
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = fence,
        .graphicsQueue = configuration.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {VK_NULL_HANDLE},
                .signalSemaphores = {VK_NULL_HANDLE},
                .commandBuffers = {commandBuffer},
                .waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}
            }
        }
    }) ;
    
    Fence::Wait(configuration.logicalDevice, {fence}, VK_TRUE, UINT64_MAX);

    Fence::Destroy(configuration.logicalDevice, fence);

    CommandBuffer::Free(configuration.logicalDevice, configuration.commandPool, {commandBuffer});
}

void Buffer::UploadDataToBuffer(UploadDataToBufferConfiguration configuration)
{
    void * memoryPointer = MapMemory(MapMemoryConfiguration{
        .offset = configuration.offset,
        .dataSize = configuration.dataSize,
        .bufferMemory = configuration.bufferMemory,
        .logicalDevice = configuration.logicalDevice,
        .memoryMapFlags = configuration.memoryMapFlags,
    });
    
    CopyMemory(CopyMemoryConfiguration{
        .data = configuration.data,
        .memoryPointer = memoryPointer,
        .dataSize = static_cast<size_t>(configuration.dataSize)
    });
    
    UnMapMemory(UnMapMemoryConfiguration{
        .bufferMemory = configuration.bufferMemory,
        .logicalDevice = configuration.logicalDevice
    });
}

void * Buffer::MapMemory(MapMemoryConfiguration configuration)
{
    void * memoryPointer ;
    vkMapMemory(configuration.logicalDevice, configuration.bufferMemory, configuration.offset, configuration.dataSize, configuration.memoryMapFlags, &memoryPointer);

    return memoryPointer;
}

void Buffer::CopyMemory(CopyMemoryConfiguration configuration)
{
    memcpy(configuration.memoryPointer, configuration.data, configuration.dataSize);
}

void Buffer::UnMapMemory(UnMapMemoryConfiguration configuration)
{
    vkUnmapMemory(configuration.logicalDevice, configuration.bufferMemory) ;
}

void Buffer::DestroyBuffer(DestroyBufferConfiguration configuration)
{
    if(configuration.buffer.buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(configuration.logicalDevice, configuration.buffer.buffer, NULL);
    if(configuration.buffer.bufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(configuration.logicalDevice, configuration.buffer.bufferMemory, NULL);
}

void Buffer::Destroy(VkDevice logicalDevice, BufferPack bufferPack)
{
    if(bufferPack.buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(logicalDevice, bufferPack.buffer, NULL);
    
    if(bufferPack.bufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(logicalDevice, bufferPack.bufferMemory, NULL);
}
