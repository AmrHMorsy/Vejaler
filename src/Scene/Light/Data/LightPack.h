#pragma once
#include "LightInfo.h"
#include "LightConfig.h"
#include "../../../Includes.h"
#include "../../LightCulling/Data/LightCullingInfo.h"
#include "../../../Core/Sampler/Data/SamplerConfig.h"


struct LightPack{
    uint32_t numLights;
    std::vector<LightConfig> configs;
    std::vector<VkDescriptorImageInfo> shadowDescriptorImages;

    
    
    
    
    
    

    
    
    
    
    
    int selected = 0;
    LightConfig config;
    VkSampler shadowSampler;
    VkCommandPool primaryCommandPool;
    SamplerConfig shadowSamplerConfig;
    LightCullingInfo lightCulling;
    std::vector<LightInfo> lights;
    std::vector<std::string> names;
    std::vector<const char *> namesPtr;
        
    std::array<glm::vec4, maxNumShadowSamples> sampleOffsetDirections;
    
    
    
    
    BufferPack shadowIndexBuffer;
    BufferPack shadowVertexBuffer;
    uint32_t shadowIndexOffset = 0;
    uint32_t shadowVertexOffset = 0;
    std::vector<uint32_t> shadowIndices;
    BufferPack shadowDrawCommandsBuffer;
    std::vector<glm::vec3> shadowVertices;
    std::vector<VkDrawIndexedIndirectCommand> shadowDrawCommands;
};
