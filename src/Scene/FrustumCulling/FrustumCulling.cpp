#include "FrustumCulling.h"
#include "Data/FrustumCullingConstants.h"
#include "Data/FrustumCullingUniformVariables.h"




void FrustumCulling::Build(ContextInfo& context, FrustumCullingInfo& frustumCulling, bool& frustumCullingEnable)
{
    frustumCulling.shader = Core::Shader::Build(context.logicalDevice, frustumCullingShaderConfig);
    
    frustumCulling.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, frustumCullingDescriptorBindings);
        
    frustumCulling.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {frustumCulling.descriptorSetLayout});
    
    frustumCulling.computePipeline = ComputePipeline::Create(context.logicalDevice, frustumCulling.pipelineLayout, frustumCulling.shader.pipelineShaderStages[0], 1)[0];
    
    frustumCulling.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    frustumCulling.primaryCommandBuffers = CommandBuffer::Allocate(context.logicalDevice, frustumCulling.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);
    
    frustumCulling.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        frustumCulling.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    frustumCulling.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        frustumCulling.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    frustumCulling.descriptorPool = DescriptorPool::Build(context.logicalDevice, frustumCullingDescriptorBindings, MAX_FRAMES_IN_FLIGHT);
    
    frustumCulling.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, frustumCullingDescriptorBindings);
    
    frustumCulling.visibleDrawCommandsCountStageBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.visibleDrawCommandsCountStageBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        });
    
    frustumCulling.visibleDrawCommandsCountBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        frustumCulling.visibleDrawCommandsCountBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = sizeof(unsigned int),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
        frustumCulling.descriptorBindings[i][4].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = frustumCulling.visibleDrawCommandsCountBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(unsigned int)
        }};
    }
    
    frustumCulling.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        frustumCulling.uniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(FrustumCullingUniformVariables)
        });
        frustumCulling.descriptorBindings[i][3].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = frustumCulling.uniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(FrustumCullingUniformVariables)
        }};
    }
    
    frustumCulling.descriptorSets = DescriptorSet::Build(context.logicalDevice, frustumCulling.descriptorSetLayout, frustumCulling.descriptorPool, frustumCulling.descriptorBindings);
                
    frustumCulling.allDrawCommandsBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    frustumCulling.visibleDrawCommandsBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    frustumCulling.visibleDrawCommandsDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    frustumCulling.uniformData.parameters = glm::vec4(static_cast<float>(0.0f), static_cast<float>(frustumCullingEnable), 0.0f, 0.0f);
}

void FrustumCulling::Load(ContextInfo& context, FrustumCullingInfo& frustumCulling, ModelPack& modelPack)
{
    DescriptorPool::Reset(context.logicalDevice, frustumCulling.descriptorPool, 0);
            
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = frustumCulling.allDrawCommandsBuffers[i]
        });
        frustumCulling.allDrawCommandsBuffers[i] = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .graphicsQueue = context.queuePack.graphicsQueue,
            .commandPool = modelPack.primaryCommandPool,
            .data = modelPack.drawCommands.data(),
            .bufferSize = sizeof(modelPack.drawCommands[0]) * modelPack.drawCommands.size()
        }) ;
        frustumCulling.descriptorBindings[i][1].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = frustumCulling.allDrawCommandsBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(VkDrawIndexedIndirectCommand) * modelPack.drawCommands.size()
        }};
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = frustumCulling.visibleDrawCommandsBuffers[i]
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.visibleDrawCommandsBuffers[i] = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
            .size = modelPack.drawCommands.size() * sizeof(VkDrawIndexedIndirectCommand),
            .logicalDevice = context.logicalDevice,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.visibleDrawCommandsDescriptorBuffers[i] = {VkDescriptorBufferInfo{
            .buffer = frustumCulling.visibleDrawCommandsBuffers[i].buffer,
            .offset = 0,
            .range = modelPack.drawCommands.size() * sizeof(VkDrawIndexedIndirectCommand)
        }};
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.descriptorBindings[i][2].descriptorBuffers = frustumCulling.visibleDrawCommandsDescriptorBuffers[i];

    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.descriptorBindings[i][0].descriptorBuffers = modelPack.boundingSphereDescriptorBuffers[i];
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.descriptorBindings[i][5].descriptorBuffers = modelPack.modelMatricesDescriptorBuffers[i];
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        frustumCulling.descriptorBindings[i][6].descriptorBuffers = modelPack.maxScalesDescriptorBuffers[i];
    
    frustumCulling.descriptorSets = DescriptorSet::Build(context.logicalDevice, frustumCulling.descriptorSetLayout, frustumCulling.descriptorPool, frustumCulling.descriptorBindings);
}

void FrustumCulling::Record(ContextInfo& context, FrustumCullingInfo& frustumCulling, ModelPack& modelPack, CameraInfo& camera, uint32_t frame)
{
    Fence::Wait(context.logicalDevice, {frustumCulling.fences[frame]}, VK_TRUE, UINT64_MAX);
    
    void* data;
    vkMapMemory(context.logicalDevice, frustumCulling.visibleDrawCommandsCountStageBuffers[frame].bufferMemory, 0, sizeof(uint32_t), 0, &data);
    memcpy(&modelPack.numVisibleDrawCommands[frame], data, sizeof(uint32_t));
    vkUnmapMemory(context.logicalDevice, frustumCulling.visibleDrawCommandsCountStageBuffers[frame].bufferMemory);
    
    Fence::Reset(context.logicalDevice, {frustumCulling.fences[frame]});
    
    CommandBuffer::Reset(frustumCulling.primaryCommandBuffers[frame]);

    CommandBuffer::Begin(frustumCulling.primaryCommandBuffers[frame], 0, nullptr);
        
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = modelPack.maxScales.data(),
        .dataSize = sizeof(modelPack.maxScales[0]) * modelPack.maxScales.size(),
        .memoryPointer = modelPack.maxScalesBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = modelPack.modelMatrices.data(),
        .dataSize = sizeof(modelPack.modelMatrices[0]) * modelPack.modelMatrices.size(),
        .memoryPointer = modelPack.modelMatricesBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &frustumCulling.uniformData,
        .dataSize = sizeof(FrustumCullingUniformVariables),
        .memoryPointer = frustumCulling.uniformBuffers[frame].memoryPointer
    });
    
    ComputePipeline::Bind(frustumCulling.primaryCommandBuffers[frame], frustumCulling.computePipeline);
    
    DescriptorSet::Bind(frustumCulling.primaryCommandBuffers[frame], VK_PIPELINE_BIND_POINT_COMPUTE, frustumCulling.pipelineLayout, {frustumCulling.descriptorSets[frame]});
    
    vkCmdFillBuffer(frustumCulling.primaryCommandBuffers[frame], frustumCulling.visibleDrawCommandsCountBuffers[frame].buffer, 0, sizeof(uint32_t), 0);
    
    if(!modelPack.drawCommands.empty()){
        uint32_t numGroups = (modelPack.drawCommands.size() + 63) / 64 ;
        vkCmdDispatch(frustumCulling.primaryCommandBuffers[frame], numGroups, 1, 1);
    }
        
    Buffer::CopyBuffer(CopyBufferConfiguration{
        .dstBuffer = frustumCulling.visibleDrawCommandsCountStageBuffers[frame].buffer,
        .srcBuffer = frustumCulling.visibleDrawCommandsCountBuffers[frame].buffer,
        .commandPool = frustumCulling.primaryCommandPool,
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
    
    CommandBuffer::End(frustumCulling.primaryCommandBuffers[frame]);
}

void FrustumCulling::Submit(ContextInfo& context, FrustumCullingInfo& frustumCulling, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = frustumCulling.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {},
                .signalSemaphores = {frustumCulling.finishedSemaphores[frame]},
                .commandBuffers = {frustumCulling.primaryCommandBuffers[frame]},
                .waitStages = {}
            }
        }
    });
}
