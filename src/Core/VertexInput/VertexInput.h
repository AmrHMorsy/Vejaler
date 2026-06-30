#pragma once
#include "../../Includes.h"
#include "Data/VertexInputInfo.h"




class VertexInput{
    
private:
    
    static size_t GetAttributeSize(VkFormat format);
    static std::vector<uint32_t> ReadFile(std::string filepath);
    static VkFormat GetAttributeFormat(spirv_cross::SPIRType type);
    static void BuildVertexBindingDescription(std::vector<VkVertexInputAttributeDescription> attributes, std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions);
    static void BuildVertexAttributeDescription(std::vector<uint32_t> vsFileData, std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions);

public:
        
    static VertexInputInfo Build(std::string vsFilePath);
};
