#pragma once
#include "../../../../Includes.h"
#include "../../../Texture/Data/TexturePack.h"



struct PreFilteredEnvironmentConfig{
    VkExtent2D resolution;
    TexturePack skyboxTexture;
};
