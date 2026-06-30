#pragma once
#include "Data/MemoryAllocateInfo.h"
#include "Data/MemoryTypeFindInfo.h"


class Memory{
    
private:
    
    static uint32_t FindMemoryType(MemoryTypeFindInfo info) ;

public:
    
    Memory() ;
    static VkDeviceMemory AllocateMemory(MemoryAllocateInfo info) ;
    ~Memory() ;
} ;
