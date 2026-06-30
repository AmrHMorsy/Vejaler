#pragma once
#include "CameraConfig.h"



struct CameraInfo{
    float yaw;
    float roll;
    float pitch;
    glm::vec4 up;
    glm::vec4 right;
    std::string name;
    float fovRadians;
    float angle = 0.0f;
    bool crouch = false;
    CameraConfig config;
    float aspectRatio;
    glm::mat4 viewMatrix;
    VkExtent2D resolution;
    glm::mat4 projectionMatrix;
    glm::mat4 projectionViewMatrix;
    glm::mat4 inverseProjectionMatrix;
    glm::mat4 rotation = glm::mat4(1.0f);
    std::array<glm::vec4, 6> viewFrustumPlanes;
    glm::mat4 lookAroundRotation = glm::mat4(1.0f);
};
