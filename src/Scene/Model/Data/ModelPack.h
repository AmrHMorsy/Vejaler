#pragma once
#include "ModelConfig.h"
#include "ModelInfo.h"
#include "../../Transform/Data/TransformInfo.h"
#include "../../../Includes.h"
#include "../../../Core/Core.h"
#include "../../BVH/Data/BVHInfo.h"
#include "../../Texture/Data/TexturePack.h"
#include "../../Texture/Data/TexturePack.h"
#include "../../Parser/Data/Mesh.h"
#include "../../Texture/Data/TextureConfig.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"
#include "../../BoundingVolume/BoundingBox/Data/BoundingBoxInfo.h"
#include "../../BoundingVolume/BoundingBox/Data/BoundingBoxInfo.h"
#include "../../Material/Data/MaterialInfo.h"
#include "../../Material/Data/MaterialConfig.h"


struct ModelPack{
    glm::vec3 max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);

    
    
    
    
    BVHInfo bvh;
    ShaderInfo shader;
    VertexInputInfo vertexInput;
    VkPipelineLayout pipelineLayout;
    ModelShaderUniform::Vertex vsData;
    ModelShaderUniform::Fragment fsData;
    GraphicsPipelineInfo graphicsPipeline;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    
    
    uint32_t indexOffset = 0;
    uint32_t vertexOffset = 0;
    uint32_t firstInstance = 0;
    
    uint32_t totalNumVertices = 0;
    uint32_t totalNumTriangles = 0;
    uint32_t totalNumDrawCommands = 0;
    std::vector<uint32_t> numVisibleDrawCommands;
    
    std::vector<uint32_t> indices;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals, vertices;
    std::vector<VkDrawIndexedIndirectCommand> drawCommands;
    BufferPack vertexBuffer, indexBuffer, normalBuffer, drawCommandsBuffer, textureCoordinatesBuffer;
    
    std::vector<ModelInfo> models;
    std::vector<BoundingBoxInfo> boundingBoxes;
    std::vector<BoundingBoxInfo> inclusiveBoundingBoxes;


    std::vector<VkSampler> samplers;
    VkCommandPool primaryCommandPool;
    std::vector<VkDescriptorPool> descriptorPools;
    std::vector<glm::vec4> boundingSpheresWorldSpace;
    std::vector<VkDescriptorImageInfo> descriptorSamplers;
    std::vector<std::vector<VkDescriptorSet>> descriptorSets;
    std::vector<UniformBufferPack> vertexShaderUniformBuffers;
    std::vector<UniformBufferPack> fragmentShaderUniformBuffers;
    std::vector<std::vector<std::vector<DescriptorBinding>>> descriptorBindings;    


    int selected;
    bool applyToAllModels = false;
    std::vector<std::string> names;
    std::vector<const char*> namesPtr;

    
    
    std::vector<glm::mat4> modelMatrices;
    std::vector<float> maxScales;
    
    std::vector<ModelSettings> modelSettings;
    
    std::vector<HostVisibleBuffer> modelMatricesBuffers, modelSettingsBuffers;
    std::vector<HostVisibleBuffer> maxScalesBuffers;
    std::vector<BufferPack> boundingSphereBuffers;
    
    std::vector<std::vector<VkDescriptorBufferInfo>> modelMatricesDescriptorBuffers;
    std::vector<std::vector<VkDescriptorBufferInfo>> maxScalesDescriptorBuffers;
    std::vector<std::vector<VkDescriptorBufferInfo>> boundingSphereDescriptorBuffers;
    
    
    
    
    std::vector<VkDescriptorImageInfo> descriptorImages;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    MaterialInfo defaultMaterial;
    MaterialConfig defaultMaterialConfig;
};
