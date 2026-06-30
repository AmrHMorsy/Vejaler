#pragma once
#include "../../../Includes.h"


#define NUM_SAMPLES 16


struct SSAOShaderUniform{
    
    struct Fragment{
        alignas(16) glm::mat4 cameraViewMatrix;
        alignas(16) glm::mat4 cameraProjectionMatrix;
        alignas(16) glm::mat4 cameraInverseProjectionMatrix;
        alignas(16) glm::vec4 screenResolution_nearClippingPlane_farClippingPlane;
        alignas(16) std::array<glm::vec4, NUM_SAMPLES> samplingKernel;
        alignas(16) glm::vec4 noiseTextureResolution;
    };
};
