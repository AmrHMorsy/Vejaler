#pragma once
#include "Data/BoundingBoxInfo.h"
#include "../../Transform/Data/TransformInfo.h"


class BoundingBox{

public:
    
    static BoundingBoxInfo Build(const std::vector<uint32_t>& indices, const std::vector<glm::vec3>& vertices);
    static bool IsIntersect(glm::vec4 &position, BoundingBoxInfo boundingBox);
    static void HandleIntersection(glm::vec4 &position, BoundingBoxInfo boundingBox);
    static void Transform(BoundingBoxInfo& boundingBox, glm::mat4 modelMatrix);
    static void HandleInclusiveIntersection(glm::vec4 &position, BoundingBoxInfo boundingBox);
};
