#pragma once
#include "../../../../Includes.h"
#define EXCLUSIVE 0
#define INCLUSIVE 1


struct BoundingBoxInfo{
    glm::vec3 center;
    std::array<glm::vec3, 8> corners;
    glm::vec4 min = glm::vec4(INFINITY, INFINITY, INFINITY, 1) ;
    glm::vec4 max = glm::vec4(-INFINITY, -INFINITY, -INFINITY, 1);
} ;
