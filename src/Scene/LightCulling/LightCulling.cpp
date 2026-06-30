#include "LightCulling.h"
#include "../Model/Model.h"
#include "Data/LightCullingConstants.h"




void LightCulling::Build(ContextInfo& context, LightCullingInfo& lightCulling, DepthPrepassInfo& depthPrepass, ModelPack& modelPack, LightPack& lightPack, CameraInfo& camera)
{
    lightCulling.numTiles2D.x = static_cast<unsigned int>(std::ceil(context.swapChain.extent.width/tileResolution.x));
    lightCulling.numTiles2D.y = static_cast<unsigned int>(std::ceil(context.swapChain.extent.height/tileResolution.y));
    
    lightCulling.uniformData.numTiles2D = lightCulling.numTiles2D;
    
    lightCulling.uniformData.numCellsPerTile = numCellsPerTile;
    lightCulling.uniformData.tileResolution = tileResolution;
    
    lightCulling.totalNumTiles = lightCulling.numTiles2D.x * lightCulling.numTiles2D.y;
    
    lightCulling.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {lightCullingComputeShaderFilePath},
        .stageFlags = {VK_SHADER_STAGE_COMPUTE_BIT}
    });
    
    lightCulling.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, lightCullingDescriptorBindingsSchematic);
    
    lightCulling.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {lightCulling.descriptorSetLayout});
    
    lightCulling.computePipeline = ComputePipeline::Create(context.logicalDevice, lightCulling.pipelineLayout, lightCulling.shader.pipelineShaderStages[0], 1)[0];
    
    lightCulling.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    lightCulling.primaryCommandBuffers = CommandBuffer::Allocate(context.logicalDevice, lightCulling.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);    
    
    lightCulling.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        lightCulling.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    lightCulling.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        lightCulling.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    lightCulling.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, lightCullingDescriptorBindingsSchematic);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        lightCulling.descriptorBindings[i][0].descriptorImages = {VkDescriptorImageInfo{
            .sampler = depthPrepass.sampler,
            .imageView = depthPrepass.imageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    
    lightCulling.tileLightCountBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        lightCulling.tileLightCountBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = lightCulling.totalNumTiles * sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
        lightCulling.descriptorBindings[i][2].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.tileLightCountBuffers[i].buffer,
            .offset = 0,
            .range = lightCulling.totalNumTiles * sizeof(unsigned int)
        }};
        modelPack.descriptorBindings[i][2][0].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.tileLightCountBuffers[i].buffer,
            .offset = 0,
            .range = lightCulling.totalNumTiles * sizeof(unsigned int)
        }};
    }

    
    lightCulling.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        lightCulling.uniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(LightCullingUniformVariables)
        });
        lightCulling.descriptorBindings[i][4].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.uniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(LightCullingUniformVariables)
        }};
    }

    lightCulling.tilesFrustumPlanesBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        lightCulling.tilesFrustumPlanesBuffers[i] = Buffer::BuildHostVisibleBuffer(context.logicalDevice, context.physicalDevicePack.physicalDevice, lightCulling.totalNumTiles * 4 * sizeof(glm::vec4), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        lightCulling.descriptorBindings[i][5].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.tilesFrustumPlanesBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = lightCulling.totalNumTiles * 4 * sizeof(glm::vec4)
        }};
    }
    
    lightCulling.maxLightCountBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        lightCulling.maxLightCountBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
        lightCulling.descriptorBindings[i][6].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.maxLightCountBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(unsigned int)
        }};
        modelPack.descriptorBindings[i][2][2].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.maxLightCountBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(unsigned int)
        }};
    }
    
    lightCulling.descriptorPool = DescriptorPool::Build(context.logicalDevice, lightCullingDescriptorBindingsSchematic, MAX_FRAMES_IN_FLIGHT);
    
    lightCulling.descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        lightCulling.descriptorSets[i] = DescriptorSet::Build2(context.logicalDevice, {lightCulling.descriptorSetLayout}, lightCulling.descriptorPool, {lightCulling.descriptorBindings[i]})[0];
    
    lightCulling.lightBoundingSphereViewSpaceBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    lightCulling.tileLightIndicesBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    lightCulling.uniformData.numTiles2D = lightCulling.numTiles2D;
    
    modelPack.fsData.numTiles2D = lightCulling.numTiles2D;
}

void LightCulling::Load(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack, CameraInfo& camera)
{
    LightCullingInfo& lightCulling = lightPack.lightCulling;
    
    lightCulling.lightBoundingSpheresViewSpace.resize(lightPack.numLights);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = lightCulling.lightBoundingSphereViewSpaceBuffers[i].bufferPack
        });
        lightCulling.lightBoundingSphereViewSpaceBuffers[i] = Buffer::BuildHostVisibleBuffer(context.logicalDevice, context.physicalDevicePack.physicalDevice, sizeof(lightCulling.lightBoundingSpheresViewSpace[0]) * lightCulling.lightBoundingSpheresViewSpace.size(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        lightCulling.descriptorBindings[i][1].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.lightBoundingSphereViewSpaceBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(lightCulling.lightBoundingSpheresViewSpace[0]) * lightCulling.lightBoundingSpheresViewSpace.size()
        }};
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = lightCulling.tileLightIndicesBuffers[i]
        });
        lightCulling.tileLightIndicesBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = lightCulling.totalNumTiles * lightPack.numLights * sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
        lightCulling.descriptorBindings[i][3].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.tileLightIndicesBuffers[i].buffer,
            .offset = 0,
            .range = lightCulling.totalNumTiles * lightPack.numLights * sizeof(unsigned int)
        }};
        modelPack.descriptorBindings[i][2][1].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = lightCulling.tileLightIndicesBuffers[i].buffer,
            .offset = 0,
            .range = lightCulling.totalNumTiles * lightPack.numLights * sizeof(unsigned int)
        }};
    }
    
    for(size_t i = 0; i < lightPack.numLights; i++)
        lightPack.lightCulling.lightBoundingSpheresViewSpace[i] = glm::vec4(glm::vec3(camera.viewMatrix * glm::vec4(lightPack.lights[i].boundingSphere.x, lightPack.lights[i].boundingSphere.y, lightPack.lights[i].boundingSphere.z, 1.0f)), lightPack.lights[i].boundingSphere.w);
    
    DescriptorPool::Reset(context.logicalDevice, lightCulling.descriptorPool, 0);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        lightCulling.descriptorSets[i] = DescriptorSet::Build2(context.logicalDevice, {lightCulling.descriptorSetLayout}, lightCulling.descriptorPool, {lightCulling.descriptorBindings[i]})[0];
}

void LightCulling::UpdateLightBoundingSpheres(LightPack& lightPack, CameraInfo& camera)
{
    for(size_t i = 0; i < lightPack.numLights; i++)
        lightPack.lightCulling.lightBoundingSpheresViewSpace[i] = glm::vec4(glm::vec3(camera.viewMatrix * glm::vec4(lightPack.lights[i].boundingSphere.x, lightPack.lights[i].boundingSphere.y, lightPack.lights[i].boundingSphere.z, 1.0f)), lightPack.lights[i].boundingSphere.w);
}

void LightCulling::Record(ContextInfo& context, LightPack& lightPack, CameraInfo& camera, uint32_t frame)
{
    LightCullingInfo& lightCulling = lightPack.lightCulling;
    
    Fence::Wait(context.logicalDevice, {lightCulling.fences[frame]}, VK_TRUE, UINT64_MAX);
    
    Fence::Reset(context.logicalDevice, {lightCulling.fences[frame]});
    
    CommandBuffer::Reset(lightCulling.primaryCommandBuffers[frame]);
    
    CommandBuffer::Begin(lightCulling.primaryCommandBuffers[frame], 0, nullptr);
        
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = lightCulling.lightBoundingSpheresViewSpace.data(),
        .dataSize = sizeof(lightCulling.lightBoundingSpheresViewSpace[0]) * lightCulling.lightBoundingSpheresViewSpace.size(),
        .memoryPointer = lightCulling.lightBoundingSphereViewSpaceBuffers[frame].memoryPointer
    });
    
    lightCulling.uniformData.nearClippingPlane = camera.config.nearClippingPlane;
    lightCulling.uniformData.farClippingPlane = camera.config.farClippingPlane;
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &lightCulling.uniformData,
        .dataSize = sizeof(LightCullingUniformVariables),
        .memoryPointer = lightCulling.uniformBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = lightCulling.tilesFrustumPlanes.data(),
        .dataSize = lightCulling.totalNumTiles * 4 * sizeof(glm::vec4),
        .memoryPointer = lightCulling.tilesFrustumPlanesBuffers[frame].memoryPointer
    });

    ComputePipeline::Bind(lightCulling.primaryCommandBuffers[frame], lightCulling.computePipeline);

    DescriptorSet::Bind(lightCulling.primaryCommandBuffers[frame], VK_PIPELINE_BIND_POINT_COMPUTE, lightCulling.pipelineLayout, {lightCulling.descriptorSets[frame]});
    
    vkCmdDispatch(lightCulling.primaryCommandBuffers[frame], lightCulling.numTiles2D.x, lightCulling.numTiles2D.y, 1);
    
    CommandBuffer::End(lightCulling.primaryCommandBuffers[frame]);
}

void LightCulling::Submit(ContextInfo& context, LightCullingInfo& lightCulling, DepthPrepassInfo& depthPrepass, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = lightCulling.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {depthPrepass.finishedSemaphores[frame]},
                .signalSemaphores = {lightCulling.finishedSemaphores[frame]},
                .commandBuffers = {lightCulling.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}
            }
        }
    });
}

void LightCulling::HandleWindowResize(ContextInfo& context, LightCullingInfo& lightCulling, ModelPack& modelPack, CameraInfo& camera)
{
    lightCulling.numTiles2D.x = static_cast<unsigned int>(std::ceil(context.swapChain.extent.width/tileResolution.x));
    
    lightCulling.numTiles2D.y = static_cast<unsigned int>(std::ceil(context.swapChain.extent.height/tileResolution.y));
    
    lightCulling.totalNumTiles = lightCulling.numTiles2D.x * lightCulling.numTiles2D.y;
    
    lightCulling.uniformData.numTiles2D = lightCulling.numTiles2D;
    
    modelPack.fsData.numTiles2D = lightCulling.numTiles2D;
    
    lightCulling.tilesFrustumPlanes = ExtractTilesFrustumPlanes(lightCulling.numTiles2D, camera.projectionMatrix);
}

std::vector<glm::vec4> LightCulling::ExtractTilesFrustumPlanes(glm::uvec2 numTiles2D, glm::mat4 cameraProjectionMatrix)
{
    std::vector<glm::vec4> tilesFrustumPlanes(numTiles2D.x * numTiles2D.y * 4);
    
    glm::vec4 row1 = glm::vec4(cameraProjectionMatrix[0][0], cameraProjectionMatrix[1][0], cameraProjectionMatrix[2][0], cameraProjectionMatrix[3][0]);
    glm::vec4 row2 = glm::vec4(cameraProjectionMatrix[0][1], cameraProjectionMatrix[1][1], cameraProjectionMatrix[2][1], cameraProjectionMatrix[3][1]);
    glm::vec4 row4 = glm::vec4(cameraProjectionMatrix[0][3], cameraProjectionMatrix[1][3], cameraProjectionMatrix[2][3], cameraProjectionMatrix[3][3]);
    
    float stepX = 2.0f / (float)numTiles2D.x;
    float stepY = 2.0f / (float)numTiles2D.y;
    
    for(int x = 0; x < numTiles2D.x; x++){
        float offsetX = (float)x * stepX;
        float xMin = -1.0f + offsetX;
        float xMax = xMin + stepX;
        for(int y = 0; y < numTiles2D.y; y++){
            std::array<glm::vec4, 4> tileFrustumPlanes;
            tileFrustumPlanes[0] = row1 - (row4 * xMin);
            tileFrustumPlanes[1] = (xMax * row4) - row1;
            
            float offsetY = (float)y * stepY;
            float yMin = -1.0f + offsetY;
            float yMax = yMin + stepY;
            tileFrustumPlanes[2] = row2 - (row4 * yMin);
            tileFrustumPlanes[3] = (yMax * row4) - row2;
            
            for(int i = 0; i < 4; i++)
                tileFrustumPlanes[i] /= glm::length(glm::vec3(tileFrustumPlanes[i]));

            int baseIndex = (y * numTiles2D.x * 4) + (x * 4);
            for(int i = 0; i < 4; i++)
                tilesFrustumPlanes[baseIndex + i] = tileFrustumPlanes[i];
        }
    }
    
    return tilesFrustumPlanes;
}
