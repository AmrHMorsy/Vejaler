#pragma once
#include "Data/BVHInfo.h"
#include "Data/BVHNode.h"

#include "../BoundingVolume/BoundingBox/BoundingBox.h"
#include "../BoundingVolume/BoundingSphere/BoundingSphere.h"


class BVH{
    
private:
    
    static int BuildRecursive(std::vector<BVHNode> &bvhNodes, const std::vector<BoundingBoxInfo>& boundingBoxes, std::vector<unsigned int>& indices, int start, int end) ;
    
public:
    
    static void Build(std::vector<BoundingBoxInfo>& boundingBoxes, BVHInfo& bvh);
    static void CheckIntersection(glm::vec4 &position, const std::vector<BVHNode>& bvhNodes, int index);
};
