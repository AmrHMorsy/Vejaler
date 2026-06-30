#pragma once
#include "../../../Includes.h"



struct ModelShaderUniform{
    struct Vertex {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 projection;
    };
    struct alignas(16) Fragment {
        uint32_t numLights;
        float nearClippingPlane;
        float farClippingPlane;
        float brightness;
        glm::vec2 screenResolution;
        glm::uvec2 numTiles2D;
        glm::vec4 cameraPosition;
        std::array<glm::vec4, MAX_NUM_LIGHTS> lightPositions;
        std::array<glm::vec4, MAX_NUM_LIGHTS> lightColors;
        std::array<glm::vec4, MAX_NUM_LIGHTS> lightDatas;
        std::array<glm::vec4, maxNumShadowSamples> sampleOffsetDirections;
    };
};
