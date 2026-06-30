#include "BVH.h"




void BVH::Build(std::vector<BoundingBoxInfo>& boundingBoxes, BVHInfo& bvh)
{
    std::vector<unsigned int> indices(boundingBoxes.size()) ;
    for(size_t i = 0; i < boundingBoxes.size(); i++)
        indices[i] = i;
    
    bvh.bvhNodes.clear();
    
    bvh.rootIndex = BuildRecursive(bvh.bvhNodes, boundingBoxes, indices, 0, indices.size()) ;
}

int BVH::BuildRecursive(std::vector<BVHNode> &bvhNodes, const std::vector<BoundingBoxInfo>& boundingBoxes, std::vector<unsigned int>& indices, int start, int end)
{
    BoundingBoxInfo unionBoundingBox ;
    for( int i = start ; i < end ; i++ ){
        unionBoundingBox.min = glm::min(unionBoundingBox.min, boundingBoxes[indices[i]].min);
        unionBoundingBox.max = glm::max(unionBoundingBox.max, boundingBoxes[indices[i]].max);
    }
    
    BVHNode bvhNode ;
    bvhNode.boundingBox = unionBoundingBox ;

    if( (end-start) == 1 ){
        bvhNode.isLeaf = true ;
        bvhNode.leftIndex = -1 ;
        bvhNode.rightIndex = -1 ;
        int nodeIndex = (int)bvhNodes.size();
        bvhNodes.push_back(bvhNode);
        return nodeIndex;
    }
    
    bvhNode.isLeaf = false ;

    glm::vec3 extent = unionBoundingBox.max - unionBoundingBox.min ;
    int axis = 0 ;
    if(extent.y > extent.x && extent.y > extent.z)
        axis = 1 ;
    else if(extent.z > extent.x)
        axis = 2 ;

    std::sort(indices.begin()+start, indices.begin()+end, [&](int a, int b) {
        glm::vec3 boxCenter1 = glm::vec3(0.5f * (boundingBoxes[a].min + boundingBoxes[a].max));
        glm::vec3 boxCenter2 = glm::vec3(0.5f * (boundingBoxes[b].min + boundingBoxes[b].max));
        return boxCenter1[axis] < boxCenter2[axis];
    });

    int mid = start + (end - start) / 2;

    int left = BuildRecursive(bvhNodes, boundingBoxes, indices, start, mid);
    int right = BuildRecursive(bvhNodes, boundingBoxes, indices, mid, end);

    bvhNode.leftIndex = left;
    bvhNode.rightIndex = right;

    int nodeIndex = (int)bvhNodes.size();
    bvhNodes.push_back(bvhNode);
    
    return nodeIndex ;
}

void BVH::CheckIntersection(glm::vec4 &position, const std::vector<BVHNode>& bvhNodes, int index)
{
    if(bvhNodes.size() == 0)
        return;
    
    const BVHNode& node = bvhNodes[index];
    
    if(!BoundingBox::IsIntersect(position, node.boundingBox))
        return;
    
    if(node.isLeaf)
        BoundingBox::HandleIntersection(position, node.boundingBox);
    else{
        if(node.leftIndex != -1)
            CheckIntersection(position, bvhNodes, node.leftIndex);
        if(node.rightIndex != -1)
            CheckIntersection(position, bvhNodes, node.rightIndex);
    }
}
