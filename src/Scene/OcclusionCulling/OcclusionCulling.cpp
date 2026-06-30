#include "OcclusionCulling.h"





void OcclusionCulling::Build(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, DepthPyramidInfo& depthPyramid)
{
    occlusionCulling.uniformData.screenResolution = glm::vec4(context.swapChain.extent.width, context.swapChain.extent.height, 0.0f, 0.0f);
    
    occlusionCulling.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {occlusionCullingComputeShaderFilePath},
        .stageFlags = {VK_SHADER_STAGE_COMPUTE_BIT}
    });
    
    occlusionCulling.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, occlusionCullingDescriptorBindingsSchematic);
    
    occlusionCulling.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {occlusionCulling.descriptorSetLayout});
    
    occlusionCulling.computePipeline = ComputePipeline::Create(context.logicalDevice, occlusionCulling.pipelineLayout, occlusionCulling.shader.pipelineShaderStages[0], 1)[0];
    
    occlusionCulling.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    occlusionCulling.primaryCommandBuffers = CommandBuffer::Allocate(context.logicalDevice, occlusionCulling.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);
    
    occlusionCulling.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    occlusionCulling.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
        
    occlusionCulling.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, occlusionCullingDescriptorBindingsSchematic);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][DEPTH_PYRAMID_DESCRIPTOR_BINDING_INDEX].descriptorImages = {VkDescriptorImageInfo{
            .sampler = depthPyramid.sampler,
            .imageView = depthPyramid.fullImageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    
    occlusionCulling.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.uniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(OcclusionCullingShaderUniform)
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][UNIFORM_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = occlusionCulling.uniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(OcclusionCullingShaderUniform)
        }};
    
    occlusionCulling.outputDrawCommandsCountBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.outputDrawCommandsCountBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
    
    occlusionCulling.outputDrawCommandsCountDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.outputDrawCommandsCountDescriptorBuffers[i] = {VkDescriptorBufferInfo{
            .buffer = occlusionCulling.outputDrawCommandsCountBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(unsigned int)
        }};
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][OUTPUT_DRAW_COMMANDS_COUNT_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = occlusionCulling.outputDrawCommandsCountDescriptorBuffers[i];
    
    occlusionCulling.outputDrawCommandsDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    occlusionCulling.outputDrawCommandsBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    occlusionCulling.descriptorPool = DescriptorPool::Build(context.logicalDevice, occlusionCullingDescriptorBindingsSchematic, MAX_FRAMES_IN_FLIGHT);
}

void OcclusionCulling::Load(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, ModelPack& modelPack, FrustumCullingInfo& frustumCulling)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][MODEL_MATRICES_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = modelPack.modelMatricesDescriptorBuffers[i];
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][MAX_SCALES_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = modelPack.maxScalesDescriptorBuffers[i];
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][BOUNDING_SPHERES_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = modelPack.boundingSphereDescriptorBuffers[i];
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][INPUT_DRAW_COMMANDS_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = frustumCulling.visibleDrawCommandsDescriptorBuffers[i];
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = occlusionCulling.outputDrawCommandsBuffers[i]
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.outputDrawCommandsBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = modelPack.drawCommands.size() * sizeof(VkDrawIndexedIndirectCommand),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.outputDrawCommandsDescriptorBuffers[i] = {VkDescriptorBufferInfo{
            .buffer = occlusionCulling.outputDrawCommandsBuffers[i].buffer,
            .offset = 0,
            .range = modelPack.drawCommands.size() * sizeof(VkDrawIndexedIndirectCommand)
        }};
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.descriptorBindings[i][OUTPUT_DRAW_COMMANDS_DESCRIPTOR_BINDING_INDEX].descriptorBuffers = occlusionCulling.outputDrawCommandsDescriptorBuffers[i];
    
    occlusionCulling.outputDrawCommandsCountStageBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        occlusionCulling.outputDrawCommandsCountStageBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        });
    
    DescriptorPool::Reset(context.logicalDevice, occlusionCulling.descriptorPool, 0);
    
    occlusionCulling.descriptorSets = DescriptorSet::Build(context.logicalDevice, occlusionCulling.descriptorSetLayout, occlusionCulling.descriptorPool, occlusionCulling.descriptorBindings);
}

void OcclusionCulling::Record(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, ModelPack& modelPack, uint32_t frame)
{
    Fence::Wait(context.logicalDevice, {occlusionCulling.fences[frame]}, VK_TRUE, UINT64_MAX);
    
    void* data;
    vkMapMemory(context.logicalDevice, occlusionCulling.outputDrawCommandsCountStageBuffers[frame].bufferMemory, 0, sizeof(uint32_t), 0, &data);
    memcpy(&modelPack.numVisibleDrawCommands[frame], data, sizeof(uint32_t));
    vkUnmapMemory(context.logicalDevice, occlusionCulling.outputDrawCommandsCountStageBuffers[frame].bufferMemory);
    
    Fence::Reset(context.logicalDevice, {occlusionCulling.fences[frame]});
    
    CommandBuffer::Reset(occlusionCulling.primaryCommandBuffers[frame]);
    
    CommandBuffer::Begin(occlusionCulling.primaryCommandBuffers[frame], 0, nullptr);
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &occlusionCulling.uniformData,
        .dataSize = sizeof(OcclusionCullingShaderUniform),
        .memoryPointer = occlusionCulling.uniformBuffers[frame].memoryPointer
    });
    
    ComputePipeline::Bind(occlusionCulling.primaryCommandBuffers[frame], occlusionCulling.computePipeline);
    
    DescriptorSet::Bind(occlusionCulling.primaryCommandBuffers[frame], VK_PIPELINE_BIND_POINT_COMPUTE, occlusionCulling.pipelineLayout, {occlusionCulling.descriptorSets[frame]});
        
    
    vkCmdFillBuffer(occlusionCulling.primaryCommandBuffers[frame], occlusionCulling.outputDrawCommandsCountBuffers[frame].buffer, 0, sizeof(uint32_t), 0);
        
    uint32_t numGroups = (modelPack.drawCommands.size() + 63) / 64;
    vkCmdDispatch(occlusionCulling.primaryCommandBuffers[frame], numGroups, 1, 1);
    
    Buffer::CopyBuffer(CopyBufferConfiguration{
        .dstBuffer = occlusionCulling.outputDrawCommandsCountStageBuffers[frame].buffer,
        .srcBuffer = occlusionCulling.outputDrawCommandsCountBuffers[frame].buffer,
        .commandPool = occlusionCulling.primaryCommandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .copyRegions = std::vector<VkBufferCopy>{
            VkBufferCopy{
                .srcOffset = 0,
                .dstOffset = 0,
                .size = sizeof(unsigned int)
            }
        }
    });
    
    CommandBuffer::End(occlusionCulling.primaryCommandBuffers[frame]);
}

void OcclusionCulling::Submit(ContextInfo& context, OcclusionCullingInfo& occlusionCulling, DepthPyramidInfo& depthPyramid, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = occlusionCulling.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {depthPyramid.finishedSemaphores[frame]},
                .signalSemaphores = {occlusionCulling.finishedSemaphores[frame]},
                .commandBuffers = {occlusionCulling.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}
            }
        }
    });
}
