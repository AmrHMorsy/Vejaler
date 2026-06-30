#include "PipelineLayout.h"



VkPipelineLayout PipelineLayout::Build(VkDevice logicalDevice, std::vector<VkDescriptorSetLayout> descriptorSetLayouts)
{
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    
    if(descriptorSetLayouts.size() == 0){
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    }
    else{
        pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
    }
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout pipelineLayout;
    
    VkResult result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE PIPELINE LAYOUT\033[0m\n");
    
    return pipelineLayout;
}

void PipelineLayout::Destroy(VkDevice logicalDevice, VkPipelineLayout& pipelineLayout)
{
    vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
}
