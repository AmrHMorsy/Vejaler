#pragma once
#include "../../../Includes.h"
#include "../../../../Resources/external/include/miniaudio/miniaudio.h"


#define SOUNDS_POOL_SIZE 10


struct AudioInfo{
    int selected = 0;
    std::array<ma_sound*, SOUNDS_POOL_SIZE> pool;
};
