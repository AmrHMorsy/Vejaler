#pragma once
#include "Data/ShaderInfo.h"
#include "Data/ShaderConfig.h"




class Shader{
  
private:
    
    static VkPipelineShaderStageCreateInfo CreatePipelineShaderStage(VkShaderModule shaderModule, VkShaderStageFlagBits stageFlag ) ;
    static VkShaderModule CreateShaderModule(VkDevice logicalDevice, std::vector<uint32_t> shaderFileData);
    static std::vector<uint32_t> ReadFile(std::string filepath) ;

public:
    
    static ShaderInfo Build(VkDevice logicalDevice, ShaderConfig config);
    static void Destroy(VkDevice logicalDevice, ShaderInfo& shader);
};
