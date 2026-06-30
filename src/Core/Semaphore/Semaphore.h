#pragma once
#include "../../Includes.h"





class Semaphore{
    
public:
    
    static VkSemaphore Build(VkDevice logicalDevice);
    static void Destroy(VkDevice logicalDevice, VkSemaphore semaphore);    
};
