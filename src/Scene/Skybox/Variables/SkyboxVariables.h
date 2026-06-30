#pragma once
#include "../../../Includes.h"
#include "../Data/HDRToCubemapVertexShaderUniformVariables.h"
#include "../Data/SkyboxVertexShaderUniformVariables.h"


#define NUM_SKYBOX_SHADERS 2
#define HDRTOCUBEMAP 0
#define SKYBOX 1


const VkExtent2D bakingResolution = VkExtent2D{
    .width = 2048,
    .height = 2048
};

const std::string hdrToCubemapVertexShaderFilePath = "../Resources/shaders/Skybox/EquirectToCubemap/EquirectToCubemap_vs.spv";

const std::string hdrToCubemapFragmentShaderFilePath = "../Resources/shaders/Skybox/EquirectToCubemap/EquirectToCubemap_fs.spv";

const std::string skyboxVertexShaderFilePath = "../Resources/shaders/Skybox/Skybox/Skybox_vs.spv";

const std::string skyboxFragmentShaderFilePath = "../Resources/shaders/Skybox/Skybox/Skybox_fs.spv";

inline glm::mat4 SkyboxProjectionMatrix()
{
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 10.0f);
    projectionMatrix[1][1] *= -1;
    
    return projectionMatrix;
}

const glm::mat4 skyboxViewMatrices[6] = {
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,  1), glm::vec3(0, -1, 0)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))
} ;

inline std::vector<glm::vec3> skyboxVertices = {
    { -1.0f,  1.0f, -1.0f },
    { -1.0f, -1.0f, -1.0f },
    {  1.0f, -1.0f, -1.0f },
    {  1.0f, -1.0f, -1.0f },
    {  1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f },
    { -1.0f, -1.0f,  1.0f },
    { -1.0f, -1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f,  1.0f },
    { -1.0f, -1.0f,  1.0f },
    {  1.0f, -1.0f, -1.0f },
    {  1.0f, -1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    {  1.0f,  1.0f, -1.0f },
    {  1.0f, -1.0f, -1.0f },
    { -1.0f, -1.0f,  1.0f },
    { -1.0f,  1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    {  1.0f, -1.0f,  1.0f },
    { -1.0f, -1.0f,  1.0f },
    { -1.0f,  1.0f, -1.0f },
    {  1.0f,  1.0f, -1.0f },
    {  1.0f,  1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    { -1.0f,  1.0f,  1.0f },
    { -1.0f,  1.0f, -1.0f },
    { -1.0f, -1.0f, -1.0f },
    { -1.0f, -1.0f,  1.0f },
    {  1.0f, -1.0f, -1.0f },
    {  1.0f, -1.0f, -1.0f },
    { -1.0f, -1.0f,  1.0f },
    {  1.0f, -1.0f,  1.0f }
};
