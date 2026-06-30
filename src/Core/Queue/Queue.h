#pragma once
#include "Data/QueuePack.h"
#include "Data/SubmitQueueInfo.h"
#include "Data/SubmitQueueConfiguration.h"
#include "Data/GetDeviceQueueConfiguration.h"



class Queue{
    
public:
    
    static QueuePack GetDeviceQueue(GetDeviceQueueConfiguration configuration);
    static void SubmitQueue(SubmitQueueConfiguration configuration);
    static void WaitIdle(VkQueue graphicsQueue);
};
