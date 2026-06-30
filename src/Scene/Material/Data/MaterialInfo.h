#pragma once
#include "../../../Includes.h"




struct MaterialInfo{
    TexturePack albedo;
    TexturePack normal;
    TexturePack opacity;
    TexturePack metallic;
    TexturePack roughness;
    std::string name = "";
    TexturePack roughnessMetallicOpacity;
};
