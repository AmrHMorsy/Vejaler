#include "BoundingBox.h"




BoundingBoxInfo BoundingBox::Build(const std::vector<uint32_t>& indices, const std::vector<glm::vec3>& vertices)
{
    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);
    std::array<glm::vec3, 8> corners;
    
    for(size_t i = 0; i < indices.size(); i++){
        min = glm::min(min, vertices[indices[i]]) ;
        max = glm::max(max, vertices[indices[i]]) ;
    }
    
    corners[0] = glm::vec3(min.x, min.y, max.z);
    corners[1] = glm::vec3(min.x, max.y, min.z);
    corners[2] = glm::vec3(min.x, max.y, max.z);
    corners[3] = glm::vec3(max.x, min.y, min.z);
    corners[4] = glm::vec3(max.x, min.y, max.z);
    corners[5] = glm::vec3(max.x, max.y, min.z);
    corners[6] = glm::vec3(max.x, max.y, max.z);
    
    glm::vec3 center = (min + max) * 0.5f;
    
    return BoundingBoxInfo{
        .corners = corners,
        .center = center,
        .min = glm::vec4(min, 1.0f),
        .max = glm::vec4(max, 1.0f),
    };
}

void BoundingBox::Transform(BoundingBoxInfo& boundingBox, glm::mat4 modelMatrix)
{
    boundingBox.min = modelMatrix * boundingBox.min;
    boundingBox.max = modelMatrix * boundingBox.max;
    boundingBox.center = (boundingBox.min + boundingBox.max) * 0.5f;
}

bool BoundingBox::IsIntersect(glm::vec4 &position, BoundingBoxInfo boundingBox)
{
    if(position.x > boundingBox.min.x && 
       position.y > boundingBox.min.y &&
       position.z > boundingBox.min.z &&
       position.x < boundingBox.max.x &&
       position.y < boundingBox.max.y &&
       position.z < boundingBox.max.z)
        return true;
    
    return false;
}

void BoundingBox::HandleIntersection(glm::vec4 &position, BoundingBoxInfo boundingBox)
{
    if(!IsIntersect(position, boundingBox))
        return;
    
    float dxMin = fabs(position.x - boundingBox.min.x);
    float dxMax = fabs(position.x - boundingBox.max.x);
    float dyMin = fabs(position.y - boundingBox.min.y);
    float dyMax = fabs(position.y - boundingBox.max.y);
    float dzMin = fabs(position.z - boundingBox.min.z);
    float dzMax = fabs(position.z - boundingBox.max.z);
    float minDist = std::min({dxMin, dxMax, dyMin, dyMax, dzMin, dzMax});
    
    if (minDist == dxMin)
        position.x = boundingBox.min.x;
    else if (minDist == dxMax)
        position.x = boundingBox.max.x;
    else if (minDist == dyMin)
        position.y = boundingBox.min.y;
    else if (minDist == dyMax)
        position.y = boundingBox.max.y;
    else if (minDist == dzMin)
        position.z = boundingBox.min.z;
    else if (minDist == dzMax)
        position.z = boundingBox.max.z;
}

void BoundingBox::HandleInclusiveIntersection(glm::vec4 &position, BoundingBoxInfo boundingBox)
{
    position = glm::min(position, boundingBox.max);
    position = glm::max(position, boundingBox.min);
}
