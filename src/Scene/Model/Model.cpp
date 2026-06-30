#include "Model.h"
#include <filesystem>
#include "Data/ModelConstants.h"




void Model::Build(ContextInfo& context, SceneInfo& scene)
{
    scene.modelPack.shader = Core::Shader::Build(context.logicalDevice, modelShaderConfig);
    
    scene.modelPack.vertexInput = VertexInput::Build(modelShaderConfig.filePaths[0]);
    
    scene.modelPack.descriptorSetLayouts.resize(modelDescriptorBindingSchematics.size());
    for(int i = 0; i < modelDescriptorBindingSchematics.size(); i++)
        scene.modelPack.descriptorSetLayouts[i] = DescriptorSetLayout::Create(context.logicalDevice, modelDescriptorBindingSchematics[i]);
    
    scene.modelPack.pipelineLayout = PipelineLayout::Build(context.logicalDevice, scene.modelPack.descriptorSetLayouts);
    
    scene.modelPack.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, graphicsPipelineConfig, scene.renderPass, scene.modelPack.pipelineLayout, scene.modelPack.shader, scene.modelPack.vertexInput);
    
    scene.modelPack.numVisibleDrawCommands.resize(MAX_FRAMES_IN_FLIGHT);
    scene.modelPack.boundingSphereBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    scene.modelPack.boundingSphereDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    scene.modelPack.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    scene.modelPack.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, modelDescriptorBindingSchematics);
    
    scene.modelPack.vertexShaderUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        scene.modelPack.vertexShaderUniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(ModelShaderUniform::Vertex)
        });
        scene.modelPack.descriptorBindings[i][0][0].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = scene.modelPack.vertexShaderUniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(ModelShaderUniform::Vertex)
        }};
    }
    
    scene.modelPack.fragmentShaderUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        scene.modelPack.fragmentShaderUniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(ModelShaderUniform::Fragment)
        });
        scene.modelPack.descriptorBindings[i][0][1].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = scene.modelPack.fragmentShaderUniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(ModelShaderUniform::Fragment)
        }};
    }
        
    scene.modelPack.descriptorPools.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        scene.modelPack.descriptorPools[i] = DescriptorPool::Build2(context.logicalDevice, modelDescriptorBindingSchematics);
    
    scene.modelPack.samplers.resize(NUM_SAMPLERS);
    scene.modelPack.descriptorSamplers.resize(NUM_SAMPLERS);
    for(int i = 0; i < NUM_SAMPLERS; i++){
        scene.modelPack.samplers[i] = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, samplerConfigs[i]);
        scene.modelPack.descriptorSamplers[i] = VkDescriptorImageInfo{
            .sampler = scene.modelPack.samplers[i],
            .imageView = VK_NULL_HANDLE,
        };
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        scene.modelPack.descriptorBindings[i][3][2].descriptorImages = scene.modelPack.descriptorSamplers;
        
    scene.modelPack.modelMatricesBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    scene.modelPack.maxScalesBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    scene.modelPack.maxScalesDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    scene.modelPack.modelMatricesDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        scene.modelPack.modelMatricesDescriptorBuffers[i].resize(1);
    
    scene.modelPack.modelSettingsBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    scene.modelPack.descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    
    Material::Build(context, scene.modelPack.defaultMaterial, scene.modelPack.defaultMaterialConfig);
    
    scene.modelPack.fsData.brightness = scene.brightness;
    scene.modelPack.fsData.screenResolution = glm::vec2(scene.screenResolution.width, scene.screenResolution.height);
}

void Model::Load(ContextInfo& context, SceneInfo& scene, const nlohmann::json& modelsJSON)
{
    scene.modelPack.selected = 0;
    
    Assimp::Importer importer;
    const aiScene * assimpScene = importer.ReadFile("../Resources/assets/sponza-gltf-pbr/Sponza.glb", aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
    if(!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
        return;
    
    Material::Load(context, scene.modelPack, "../Resources/assets/sponza-gltf-pbr/", assimpScene);
    
    for(uint32_t i = 0; i < scene.modelPack.models.size(); i++)
        scene.modelPack.names.push_back(scene.modelPack.models[i].config.name);
    
    for(uint32_t i = 0; i < scene.modelPack.models.size(); i++)
        scene.modelPack.modelMatrices.push_back(scene.modelPack.models[i].transform.modelMatrix);
    
    for(uint32_t i = 0; i < scene.modelPack.models.size(); i++)
        scene.modelPack.maxScales.push_back(scene.modelPack.models[i].transform.maxScale);
    
    for(uint32_t i = 0; i < scene.modelPack.models.size(); i++)
        scene.modelPack.modelSettings.push_back(scene.modelPack.models[i].config.modelSettings);
    
    Loader::Load(context, scene.modelPack, assimpScene);
    
    Shadow::Load(context, scene.lightPack, scene.modelPack);
    
    UpdateModelMatricesBuffers(context, scene.modelPack);
    UpdateModelMaxScaleBuffers(context, scene.modelPack);
    UpdateBoundingSphereBuffers(context, scene.modelPack);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = scene.modelPack.modelSettingsBuffers[i].bufferPack
        });
        scene.modelPack.modelSettingsBuffers[i] = Buffer::BuildHostVisibleBuffer(context.logicalDevice, context.physicalDevicePack.physicalDevice, sizeof(scene.modelPack.modelSettings[0]) * scene.modelPack.modelSettings.size(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        scene.modelPack.descriptorBindings[i][5][1].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = scene.modelPack.modelSettingsBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = scene.modelPack.modelSettings.size() * sizeof(scene.modelPack.modelSettings[0])
        }};
    }
    
    scene.modelPack.namesPtr.resize(scene.modelPack.names.size());
    for(int i = 0; i < scene.modelPack.names.size(); i++)
        scene.modelPack.namesPtr[i] = scene.modelPack.names[i].c_str();
    
    scene.modelPack.descriptorImages.clear();
    for(int j = 0; j < scene.modelPack.models.size(); j++){
        scene.modelPack.descriptorImages.push_back(VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = scene.modelPack.models[j].material.albedo.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        });
        scene.modelPack.descriptorImages.push_back(VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = scene.modelPack.models[j].material.roughnessMetallicOpacity.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        });
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        scene.modelPack.descriptorBindings[i][0][2].descriptorImages = scene.modelPack.descriptorImages;
        scene.modelPack.descriptorBindings[i][0][2].descriptorActualCount = scene.modelPack.descriptorImages.size();
    }
    
    UpdateDescriptorSets(context, scene.modelPack);
    
    DepthPrepass::Load(context, scene.depthPrepass, scene.modelPack);
    
    FrustumCulling::Load(context, scene.frustumCulling, scene.modelPack);
    
    OcclusionCulling::Load(context, scene.occlusionCulling, scene.modelPack, scene.frustumCulling);
    
    NormalPass::Load(context, scene.normalPass, scene.modelPack);
    
    BVH::Build(scene.modelPack.boundingBoxes, scene.modelPack.bvh);
}

void Model::Render(VkDevice logicalDevice, ModelPack& modelPack, FrustumCullingInfo& frustumCulling, OcclusionCullingInfo& occlusionCulling, VkCommandBuffer& commandBuffer, uint32_t frame)
{
    if(modelPack.models.empty())
        return;
    
    GraphicsPipeline::Bind(commandBuffer, modelPack.graphicsPipeline.handle);
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = modelPack.modelMatrices.data(),
        .dataSize = sizeof(modelPack.modelMatrices[0]) * modelPack.modelMatrices.size(),
        .memoryPointer = modelPack.modelMatricesBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = modelPack.modelSettings.data(),
        .dataSize = sizeof(modelPack.modelSettings[0]) * modelPack.modelSettings.size(),
        .memoryPointer = modelPack.modelSettingsBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &modelPack.vsData,
        .dataSize = sizeof(ModelShaderUniform::Vertex),
        .memoryPointer = modelPack.vertexShaderUniformBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &modelPack.fsData,
        .dataSize = sizeof(ModelShaderUniform::Fragment),
        .memoryPointer = modelPack.fragmentShaderUniformBuffers[frame].memoryPointer
    });
    
    DescriptorSet::Bind(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, modelPack.pipelineLayout, modelPack.descriptorSets[frame]);
    
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0, 0, 0},
        .firstBinding = 0,
        .commandBuffer = commandBuffer,
        .vertexBuffers = {
            modelPack.vertexBuffer.buffer,
            modelPack.normalBuffer.buffer,
            modelPack.textureCoordinatesBuffer.buffer
        }
    });
    
    Buffer::BindIndexBuffer(BindIndexBufferConfiguration{
        .commandBuffer = commandBuffer,
        .indexBuffer = modelPack.indexBuffer.buffer,
        .type = VK_INDEX_TYPE_UINT32,
        .offset = 0
    });
    
    Drawer::DrawIndexedIndirect(commandBuffer, occlusionCulling.outputDrawCommandsBuffers[frame].buffer, 0, modelPack.drawCommands.size(), sizeof(VkDrawIndexedIndirectCommand));
}

void Model::UpdateDescriptorSets(ContextInfo& context, ModelPack& modelPack)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        DescriptorPool::Reset(context.logicalDevice, modelPack.descriptorPools[i], 0);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.descriptorSets[i] = DescriptorSet::Build2(context.logicalDevice, modelPack.descriptorSetLayouts, modelPack.descriptorPools[i], modelPack.descriptorBindings[i]);
}

void Model::UpdateModelMatricesBuffers(ContextInfo& context, ModelPack& modelPack)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = modelPack.modelMatricesBuffers[i].bufferPack
        });
        
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.modelMatricesBuffers[i] = Buffer::BuildHostVisibleBuffer(context.logicalDevice, context.physicalDevicePack.physicalDevice, sizeof(modelPack.modelMatrices[0]) * modelPack.modelMatrices.size(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.modelMatricesDescriptorBuffers[i][0] = VkDescriptorBufferInfo{
            .buffer = modelPack.modelMatricesBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = modelPack.modelMatrices.size() * sizeof(modelPack.modelMatrices[0])
        };
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.descriptorBindings[i][5][0].descriptorBuffers = modelPack.modelMatricesDescriptorBuffers[i];
}

void Model::UpdateModelMaxScaleBuffers(ContextInfo& context, ModelPack& modelPack)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = modelPack.maxScalesBuffers[i].bufferPack
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.maxScalesBuffers[i] = Buffer::BuildHostVisibleBuffer(context.logicalDevice, context.physicalDevicePack.physicalDevice, sizeof(modelPack.maxScales[0]) * modelPack.maxScales.size(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.maxScalesDescriptorBuffers[i] = {VkDescriptorBufferInfo{
                .buffer = modelPack.maxScalesBuffers[i].bufferPack.buffer,
                .offset = 0,
                .range = modelPack.maxScales.size() * sizeof(modelPack.maxScales[0])
            }};    
}

void Model::UpdateBoundingSphereBuffers(ContextInfo& context, ModelPack& modelPack)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        Buffer::DestroyBuffer(DestroyBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .buffer = modelPack.boundingSphereBuffers[i]
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.boundingSphereBuffers[i] =  Buffer::BuildBufferWithData(BuildBufferWithDataConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .graphicsQueue = context.queuePack.graphicsQueue,
            .commandPool = modelPack.primaryCommandPool,
            .data = modelPack.boundingSpheresWorldSpace.data(),
            .bufferSize = sizeof(modelPack.boundingSpheresWorldSpace[0]) * modelPack.boundingSpheresWorldSpace.size()
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        modelPack.boundingSphereDescriptorBuffers[i] = {VkDescriptorBufferInfo{
            .buffer = modelPack.boundingSphereBuffers[i].buffer,
            .offset = 0,
            .range = modelPack.boundingSpheresWorldSpace.size() * sizeof(glm::vec4)
        }};
}

void Model::Delete(ContextInfo& context, SceneInfo& scene, uint32_t modelIndex)
{
    if(scene.modelPack.models.size() == 0)
        return;
    
    ModelConfig& config = scene.modelPack.models[modelIndex].config;
    
    scene.modelPack.totalNumVertices -= scene.modelPack.vertices.size();
    scene.modelPack.totalNumTriangles -= (scene.modelPack.indices.size() / 3);
    scene.modelPack.totalNumDrawCommands -= scene.modelPack.drawCommands.size();
    
    if(scene.modelPack.models[modelIndex].config.boundingBoxType == EXCLUSIVE){
        scene.modelPack.boundingBoxes.erase(scene.modelPack.boundingBoxes.begin() + modelIndex);
        BVH::Build(scene.modelPack.boundingBoxes, scene.modelPack.bvh);
    }
    
    scene.modelPack.names.resize(scene.modelPack.names.size()-1);
    
    scene.modelPack.models.erase(scene.modelPack.models.begin() + modelIndex);
        
    for(size_t i = 0; i < scene.lightPack.lights.size(); i++)
        Shadow::Bake(context, scene.modelPack, scene.lightPack, i);
}
