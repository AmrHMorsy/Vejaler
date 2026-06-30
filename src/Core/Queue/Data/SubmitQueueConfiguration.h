#pragma once
#include "SubmitQueueInfo.h"
#include "../../../Includes.h"



struct SubmitQueueConfiguration{
    VkFence fence;
    VkQueue graphicsQueue;
    std::vector<SubmitQueueInfo> submissionInfos;
};
