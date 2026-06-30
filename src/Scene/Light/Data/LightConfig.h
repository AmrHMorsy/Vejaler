#pragma once
#include "../../../Includes.h"
#include "../../Shadow/Data/ShadowConfig.h"




struct LightConfig{
    int type = 0;
    float speed = 3.0f;
    float shadowBias = 0.1;
    ShadowConfig shadowConfig;
    int numShadowSamples = 1;
    float intensity = 10000.0f;
    float minShadowBias = 0.0f;
    float maxShadowBias = 1.0f;
    float nearClippingPlane = 0.5f;
    float farClippingPlane = 3000.0f;
    float minLightIntensityPercentage = 0.001f;
    float minDistancePoissonSampling = 0.01f;
    float softShadowSampleDirectionsOffset = 0.03f;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 position = glm::vec4(30.0f, 30.0f, 40.0f, 1.0f);
};
