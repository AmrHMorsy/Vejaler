#pragma once
#include "Data/IntersectionInfo.h"
#include "../../Transform/Data/TransformInfo.h"



class BoundingSphere{

public:
    
    static glm::vec4 Build(const std::vector<uint32_t>& indices, const std::vector<glm::vec3>& vertices);
    static glm::vec4 Transform(const glm::vec4& boundingSphere, const TransformInfo& transform);
    static IntersectionInfo IsIntersect(glm::vec4 &position, glm::vec4 boundingSphere);
    static void HandleIntersection(glm::vec4 &position, glm::vec4 boundingSphere, IntersectionInfo intersectionInfo);
} ;
