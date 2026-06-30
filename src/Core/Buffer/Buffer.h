#pragma once
#include "../Queue/Queue.h"
#include "Data/BufferPack.h"
#include "Data/UniformBufferPack.h"
#include "../Fence/Fence.h"
#include "Data/MapMemoryConfiguration.h"
#include "Data/CopyBufferConfiguration.h"
#include "Data/CopyMemoryConfiguration.h"
#include "../Memory/Memory.h"
#include "Data/UnMapMemoryConfiguration.h"
#include "Data/DestroyBufferConfiguration.h"
#include "Data/BuildRawBufferConfiguration.h"
#include "Data/BindIndexBufferConfiguration.h"
#include "Data/BindVertexBufferConfiguration.h"
#include "Data/CopyBufferToImageConfiguration.h"
#include "Data/BuildUniformBufferConfiguration.h"
#include "Data/UploadDataToBufferConfiguration.h"
#include "Data/BuildBufferWithDataConfiguration.h"
#include "Data/HostVisibleBuffer.h"
#include "../CommandBuffer/CommandBuffer.h"



class Buffer {
    
public:
    
    Buffer();
    static BufferPack BuildRawBuffer(BuildRawBufferConfiguration configuration);
    static BufferPack BuildBufferWithData(BuildBufferWithDataConfiguration configuration);
    static UniformBufferPack BuildUniformBuffer(BuildUniformBufferConfiguration configuration);
    static void BindVertexBuffer(BindVertexBufferConfiguration configuration);
    static void BindIndexBuffer(BindIndexBufferConfiguration configuration) ;
    static void CopyBuffer(CopyBufferConfiguration configuration);
    static void CopyBufferToImage(CopyBufferToImageConfiguration configuration);
    static void * MapMemory(MapMemoryConfiguration configuration);
    static void CopyMemory(CopyMemoryConfiguration configuration);
    static void UnMapMemory(UnMapMemoryConfiguration configuration);
    static void UploadDataToBuffer(UploadDataToBufferConfiguration configuration);
    static void DestroyBuffer(DestroyBufferConfiguration configuration) ;
    static HostVisibleBuffer BuildHostVisibleBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usage);
    
    
    
    static BufferPack BuildWithData(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, void * data, VkDeviceSize bufferSize);
    static void Destroy(VkDevice logicalDevice, BufferPack bufferPack);

    
    ~Buffer();
} ;
