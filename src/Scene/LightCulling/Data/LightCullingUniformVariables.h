#pragma once
#include "../../../Includes.h"



struct LightCullingUniformVariables{
    float nearClippingPlane;
    float farClippingPlane;
    glm::uvec2 numTiles2D;
    uint numCellsPerTile;
    uint flags;
    glm::uvec2 tileResolution;
};
