#include "BoundingSphere.h"



glm::vec4 BoundingSphere::Build(const std::vector<uint32_t>& indices, const std::vector<glm::vec3>& vertices)
{
    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);
    
    for(size_t i = 0; i < indices.size(); i++){
        min = glm::min(min, vertices[indices[i]]);
        max = glm::max(max, vertices[indices[i]]);
    }
    
    glm::vec3 center = (min + max) * 0.5f;
    
    float radius = 0.0f;
    
    for(size_t i = 0; i < indices.size(); i++)
        radius = glm::max(radius, glm::length(vertices[indices[i]]-center));
    
    return glm::vec4(center, radius);
}

glm::vec4 BoundingSphere::Transform(const glm::vec4& boundingSphere, const TransformInfo& transform)
{
    float transformedRadius = boundingSphere.w * transform.maxScale;
    glm::vec3 transformedCenter = glm::vec3(transform.modelMatrix * glm::vec4(glm::vec3(boundingSphere.x, boundingSphere.y, boundingSphere.z), 1.0f));
    
    return glm::vec4(transformedCenter, transformedRadius);
}

IntersectionInfo BoundingSphere::IsIntersect(glm::vec4 &position, glm::vec4 boundingSphere)
{
    glm::vec4 center = glm::vec4(boundingSphere.r, boundingSphere.g, boundingSphere.b, 1.0f);
    float radius = boundingSphere.w;
    glm::vec4 direction = position - center;
    float distance = glm::length(direction);
    
    bool isIntersect;
    if(distance < radius)
        isIntersect = true;
    
    isIntersect = false;
    
    return IntersectionInfo{
        .distance = distance,
        .direction = direction,
        .isIntersect = isIntersect
    };
}

void BoundingSphere::HandleIntersection(glm::vec4 &position, glm::vec4 boundingSphere, IntersectionInfo info)
{
    glm::vec4 normalizedDirection = (info.distance >= 1e-5f) ? (info.direction / info.distance): glm::vec4(1,0,0,0);
    position = glm::vec4(boundingSphere.r, boundingSphere.g, boundingSphere.b, 1.0f) + (normalizedDirection * boundingSphere.w);
}
