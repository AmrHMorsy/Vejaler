#pragma once
#include "../../../Includes.h"



struct CameraConfig{
    float speed = 0.2f;
    float fovDegrees = 45.0f;
    bool lockedMotion = false;
    bool lockedPanning = false;
    float lookAroundSpeed = 0.04;
    float nearClippingPlane = 0.1f;
    float farClippingPlane = 1000.0f;
    bool enableCameraCollision = false;
    glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 originalPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    glm::vec4 originalDirection = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    
    glm::vec3 minBoundaries = glm::vec3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
    glm::vec3 maxBoundaries = glm::vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
};
