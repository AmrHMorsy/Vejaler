#pragma once
#include "../../../Includes.h"




struct ModelSettings{
    uint shadingFlags = (1u << 0) | (1u << 1) | (1u << 2) | (1u << 3);
    int albedoMipLevel = 0;
    int roughnessMipLevel = 0;
    int metallicMipLevel = 0;
    float gammaExponent = 2.2;
};
