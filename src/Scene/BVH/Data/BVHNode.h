#pragma once
#include "../../BoundingVolume/BoundingBox/Data/BoundingBoxInfo.h"


struct BVHNode {
    bool isLeaf;
    int leftIndex;
    int rightIndex;
    BoundingBoxInfo boundingBox;
};
