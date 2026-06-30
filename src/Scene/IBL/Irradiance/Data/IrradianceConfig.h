#pragma once
#include "../../../../Includes.h"
#include "../../../Texture/Data/TexturePack.h"



struct IrradianceConfig{
    VkExtent2D resolution;
    TexturePack skyboxTexturePack;
};
