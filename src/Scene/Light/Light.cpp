#include "Light.h"
#include "Variables/LightVariables.h"
#include "../Model/Data/ModelConstants.h"






void Light::Build(ContextInfo& context, LightPack& lightPack, DepthPrepassInfo& depthPrepass, LightCullingInfo& lightCulling, ModelPack& modelPack, CameraInfo& camera)
{
    GenerateSoftShadowSampleDirections(lightPack);
    
    LightCulling::Build(context, lightPack.lightCulling, depthPrepass, modelPack, lightPack, camera);
    
    lightPack.shadowSamplerConfig = SamplerConfig{
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .anisotropyEnable = VK_FALSE,
        .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_TRUE,
        .compareOp = VK_COMPARE_OP_LESS,
        .mipMapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = 0.0f
    };
    lightPack.shadowSampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, lightPack.shadowSamplerConfig);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.descriptorBindings[i][1][0].descriptorImages = {VkDescriptorImageInfo{
            .sampler = lightPack.shadowSampler,
            .imageView = VK_NULL_HANDLE,
        }};
    
    lightPack.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    modelPack.fsData.sampleOffsetDirections = lightPack.sampleOffsetDirections;
}

void Light::GenerateSoftShadowSampleDirections(LightPack& lightPack)
{
    std::vector<std::array<float, 3>> softShadowSampleDirections = PoissonDisk<3>::Generate(maxNumShadowSamples, 30, lightPack.config.minDistancePoissonSampling, minPoissonSampling, maxPoissonSampling);
    
    if(softShadowSampleDirections.size() >= maxNumShadowSamples){
        for(int i = 0; i < maxNumShadowSamples; i++)
            lightPack.sampleOffsetDirections[i] = glm::vec4(softShadowSampleDirections[i][0], softShadowSampleDirections[i][1], softShadowSampleDirections[i][2], 0.0f);
    }
    else{
        for(int i = 0; i < maxNumShadowSamples; i++)
            lightPack.sampleOffsetDirections[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void Light::Load(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack, CameraInfo& camera)
{
    if(lightPack.numLights > MAX_NUM_LIGHTS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO LOAD LIGHTS::NUM_LIGHTS > MAX_NUM_LIGHTS\033[0m");
    
    lightPack.names.resize(lightPack.numLights);
    lightPack.namesPtr.resize(lightPack.numLights);
    for(uint32_t i = 0; i < lightPack.numLights; i++){
        lightPack.names[i] = "Light " + std::to_string(i + 1);
        lightPack.namesPtr[i] = lightPack.names[i].c_str();
    }
    
    Generate(lightPack, modelPack);
    
    for(uint32_t i = 0; i < lightPack.numLights; i++){
        ComputeProjectionMatrix(lightPack.lights[i]);
        ComputeViewMatrices(lightPack.lights[i]);
        ComputeBoundingSphere(lightPack.lights[i]);
    }
    
    lightPack.shadowDescriptorImages.resize(lightPack.numLights);
    for(uint32_t i = 0; i < lightPack.numLights; i++){
        Shadow::Build(context, modelPack, lightPack.lights[i]);
        Shadow::Bake(context, modelPack, lightPack, i);
        lightPack.shadowDescriptorImages[i] = VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = lightPack.lights[i].shadow.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
        };
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        modelPack.descriptorBindings[i][1][1].descriptorImages = lightPack.shadowDescriptorImages;
        modelPack.descriptorBindings[i][1][1].descriptorActualCount = lightPack.shadowDescriptorImages.size();
    }
        
    LightCulling::Load(context, lightPack, modelPack, camera);
    
    modelPack.fsData.numLights = lightPack.numLights;
    for(uint32_t i = 0; i < lightPack.numLights; i++){
        modelPack.fsData.lightPositions[i] = lightPack.lights[i].config.position;
        modelPack.fsData.lightColors[i] = lightPack.lights[i].config.intensity * lightPack.lights[i].config.color;
        modelPack.fsData.lightDatas[i] = glm::vec4(lightPack.lights[i].config.farClippingPlane, lightPack.lights[i].config.shadowBias, lightPack.lights[i].config.numShadowSamples, lightPack.lights[i].config.softShadowSampleDirectionsOffset);
    }
    
    Model::UpdateDescriptorSets(context, modelPack);
}

void Light::ComputeProjectionMatrix(LightInfo& light)
{
    light.projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, light.config.nearClippingPlane, light.config.farClippingPlane);
}

void Light::ComputeViewMatrices(LightInfo& light)
{
    for(int j = 0; j < 6; j++)
        light.cubemapViewMatrices[j] = glm::lookAt(glm::vec3(light.config.position), glm::vec3(light.config.position) + cubemapDirections[j], cubemapUpVectors[j]);
}

void Light::ComputeBoundingSphere(LightInfo& light)
{
    glm::vec3 center = light.config.position;
    float radius = sqrt(100.0f/light.config.minLightIntensityPercentage);
    
    light.boundingSphere = glm::vec4(center, radius);
}

void Light::Generate(LightPack& lightPack, ModelPack& modelPack)
{
    if(modelPack.max == glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN))
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO GENERATE LIGHTS::SCENE MAX IS INVALID\033[0m");
    
    if(modelPack.min == glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX))
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO GENERATE LIGHTS::SCENE MIN IS INVALID\033[0m");
    
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distX(modelPack.min.x, modelPack.max.x);
    std::uniform_real_distribution<float> distY(modelPack.min.y, modelPack.max.y);
    std::uniform_real_distribution<float> distZ(modelPack.min.z, modelPack.max.z);
    
    lightPack.lights.resize(lightPack.numLights);
    for(uint32_t i = 0; i < lightPack.numLights; i++)
        lightPack.lights[i].config.position = glm::vec4(glm::vec3(distX(rng), distY(rng), distZ(rng)), 1.0f);
}
