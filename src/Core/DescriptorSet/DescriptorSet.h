#pragma once
#include "../DescriptorBinding/Data/DescriptorBinding.h"



class DescriptorSet {
    
public:
    
    static std::vector<VkDescriptorSet> Build2(VkDevice logicalDevice, const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkDescriptorPool& descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings);
    static std::vector<VkDescriptorSet> Allocate2(VkDevice logicalDevice, const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkDescriptorPool& descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings, uint32_t numSets);
    static void Write2(VkDevice logicalDevice, const std::vector<VkDescriptorSet>& descriptorSets, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings, uint32_t numSets);
    static void Write2(VkDevice logicalDevice, VkDescriptorSet descriptorSet, const std::vector<DescriptorBinding>& descriptorBindings);
    
    
    
    static std::vector<VkDescriptorSet> Build(VkDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings);
    static std::vector<VkDescriptorSet> Allocate(VkDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings);
    static void Write(VkDevice logicalDevice, std::vector<VkDescriptorSet> &descriptorSets, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings);
    static void Bind(VkCommandBuffer commandBuffer, VkPipelineBindPoint bindPoint, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet> descriptorSets);
};
