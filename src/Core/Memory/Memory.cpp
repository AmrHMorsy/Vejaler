#include "Memory.h"




VkDeviceMemory Memory::AllocateMemory(MemoryAllocateInfo info)
{
    VkMemoryAllocateInfo allocInfo{} ;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = info.memRequirements.size ;
    allocInfo.memoryTypeIndex = FindMemoryType(MemoryTypeFindInfo{
        .physicalDevice = info.physicalDevice,
        .properties = info.properties,
        .typeFilter = info.memRequirements.memoryTypeBits
    }) ;

    VkDeviceMemory memory ;
    VkResult result = vkAllocateMemory(info.logicalDevice, &allocInfo, nullptr, &memory) ;
    
    if(result != VK_SUCCESS)
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO ALLOCATE IMAGE MEMORY \033[0m" ) ;

    return memory ;
}

uint32_t Memory::FindMemoryType(MemoryTypeFindInfo info)
{
    VkPhysicalDeviceMemoryProperties memoryProperties ;
    vkGetPhysicalDeviceMemoryProperties( info.physicalDevice, &memoryProperties ) ;
    
    for( uint32_t i = 0 ; i < memoryProperties.memoryTypeCount; i++ ){
        if( (info.typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & info.properties) == info.properties )
            return i ;
    }
    throw std::runtime_error( "\033[31m\nERROR::FAILURE TO FIND SUITABLE MEMORY TYPE\033[0m" ) ;
}
