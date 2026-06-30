#include "DescriptorSet.h"




std::vector<VkDescriptorSet> DescriptorSet::Build2(VkDevice logicalDevice, const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkDescriptorPool& descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings)
{
    if(descriptorSetLayouts.size() != descriptorBindings.size())
        throw std::runtime_error("ERROR::FAILURE TO BUILD DESCRIPTOR SETS");
    
    uint32_t numSets = descriptorSetLayouts.size();

    std::vector<VkDescriptorSet> descriptorSets = Allocate2(logicalDevice, descriptorSetLayouts, descriptorPool, descriptorBindings, numSets);
            
    Write2(logicalDevice, descriptorSets, descriptorBindings, numSets);
    
    return descriptorSets;
}

std::vector<VkDescriptorSet> DescriptorSet::Allocate2(VkDevice logicalDevice, const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkDescriptorPool& descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings, uint32_t numSets)
{
    bool hasVariableDescriptorCount = false;
    std::vector<uint32_t> descriptorVariableCounts(numSets, 0);
    for(size_t i = 0; i < numSets; i++){
        
        uint32_t numBindingsWithVariableCount = 0;
        for(size_t k = 0; k < descriptorBindings[i].size(); k++){
            if(descriptorBindings[i][k].descriptorBindingFlags & VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT)
                numBindingsWithVariableCount++;
        }
        if(numBindingsWithVariableCount > 1)
            throw std::runtime_error("ERROR::ONLY ONE DESCRIPTOR BINDING CAN BE BINDLESS/HAVE VARIABLE COUNT");
        
        uint32_t j = 0, maxBindingIndex = std::numeric_limits<uint32_t>::min();;
        for(size_t k = 0; k < descriptorBindings[i].size(); k++){
            if(descriptorBindings[i][k].binding >= maxBindingIndex){
                maxBindingIndex = descriptorBindings[i][k].binding;
                j = k;
            }
        }
        
        for(size_t k = 0; k < descriptorBindings[i].size(); k++){
            if((descriptorBindings[i][k].descriptorBindingFlags & VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT) && (j != k))
                    throw std::runtime_error("ERROR::ONLY THE HIGHEST DESCRIPTOR BINDING INDEX CAN BE BINDLESS/HAVE VARIABLE COUNT");
        }
        
        if(descriptorBindings[i][j].descriptorBindingFlags & VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT){
            descriptorVariableCounts[i] = descriptorBindings[i][j].descriptorActualCount;
            hasVariableDescriptorCount = true;
        }
    }
    
    VkDescriptorSetVariableDescriptorCountAllocateInfo descriptorSetVariableDescriptorCountAllocateInfo{};
    descriptorSetVariableDescriptorCountAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    descriptorSetVariableDescriptorCountAllocateInfo.descriptorSetCount = numSets;
    descriptorSetVariableDescriptorCountAllocateInfo.pDescriptorCounts = descriptorVariableCounts.data();

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = numSets;
    descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();
    if(hasVariableDescriptorCount)
        descriptorSetAllocateInfo.pNext = &descriptorSetVariableDescriptorCountAllocateInfo;
    else
        descriptorSetAllocateInfo.pNext = nullptr;
    
    std::vector<VkDescriptorSet> descriptorSets(numSets) ;
    VkResult result = vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, descriptorSets.data());
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("ERROR::FAILURE TO ALLOCATE DESCRIPTOR SETS22");
    
    return descriptorSets;
}

void DescriptorSet::Write2(VkDevice logicalDevice, const std::vector<VkDescriptorSet>& descriptorSets, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings, uint32_t numSets)
{
    for(size_t i = 0; i < numSets; i++)
        DescriptorSet::Write2(logicalDevice, descriptorSets[i], descriptorBindings[i]);
}

void DescriptorSet::Write2(VkDevice logicalDevice, VkDescriptorSet descriptorSet, const std::vector<DescriptorBinding>& descriptorBindings)
{
    uint32_t numBindings = descriptorBindings.size();
    
    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
        
    for(size_t j = 0; j < numBindings; j++){
        
        VkWriteDescriptorSet writeDescriptorSet{} ;
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet = descriptorSet;
        writeDescriptorSet.dstBinding = descriptorBindings[j].binding;
        
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = descriptorBindings[j].descriptorType;
        writeDescriptorSet.descriptorCount = descriptorBindings[j].descriptorActualCount;
        
        switch(descriptorBindings[j].descriptorType){
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                writeDescriptorSet.pBufferInfo = descriptorBindings[j].descriptorBuffers.data();
                writeDescriptorSet.pImageInfo = nullptr;
                break;
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            case VK_DESCRIPTOR_TYPE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                writeDescriptorSet.pImageInfo = descriptorBindings[j].descriptorImages.data();
                writeDescriptorSet.pBufferInfo = nullptr;
                break;
            default:
                throw std::runtime_error("\033[31m\nERROR::UNSUPPORTED DESCRIPTOR TYPE\033[0m");
        };
        
        writeDescriptorSets.push_back(writeDescriptorSet);
    }
    
    vkUpdateDescriptorSets(logicalDevice, numBindings, writeDescriptorSets.data(), 0, NULL);
}









































std::vector<VkDescriptorSet> DescriptorSet::Build(VkDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings)
{
    std::vector<VkDescriptorSet> descriptorSets = Allocate(logicalDevice, descriptorSetLayout, descriptorPool, descriptorBindings);
        
    Write(logicalDevice, descriptorSets, descriptorBindings);
    
    return descriptorSets;
}

void DescriptorSet::Write(VkDevice logicalDevice, std::vector<VkDescriptorSet> &descriptorSets, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings)
{
    uint32_t numBindings, numSets = descriptorBindings.size();
    
    std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptorSets(numSets);
    
    for(size_t i = 0; i < numSets; i++){
        numBindings = descriptorBindings[i].size();
        writeDescriptorSets[i].resize(numBindings);
        
        for(size_t j = 0; j < numBindings; j++){
            VkWriteDescriptorSet writeDescriptorSet{} ;
            writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet = descriptorSets[i];
            writeDescriptorSet.dstBinding = descriptorBindings[i][j].binding;
            writeDescriptorSet.dstArrayElement = 0;
            writeDescriptorSet.descriptorType = descriptorBindings[i][j].descriptorType;
            writeDescriptorSet.descriptorCount = descriptorBindings[i][j].descriptorActualCount;
            
            switch(descriptorBindings[i][j].descriptorType){
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                    writeDescriptorSet.pBufferInfo = descriptorBindings[i][j].descriptorBuffers.data();
                    writeDescriptorSet.pImageInfo = nullptr;
                    break;
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                case VK_DESCRIPTOR_TYPE_SAMPLER:
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                    writeDescriptorSet.pImageInfo = descriptorBindings[i][j].descriptorImages.data();
                    writeDescriptorSet.pBufferInfo = nullptr;
                    break;
                default:
                    throw std::runtime_error("\033[31m\nERROR::UNSUPPORTED DESCRIPTOR TYPE\033[0m");
            };
            writeDescriptorSets[i][j] = writeDescriptorSet;
        }
        vkUpdateDescriptorSets(logicalDevice, numBindings, writeDescriptorSets[i].data(), 0, NULL);
    }
}

std::vector<VkDescriptorSet> DescriptorSet::Allocate(VkDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings)
{
    uint32_t numSets = descriptorBindings.size();
        
    std::vector<uint32_t> descriptorVariableCounts(numSets);
    for(size_t i = 0; i < numSets; i++){
        size_t j = descriptorBindings[i].size() - 1;
        descriptorVariableCounts[i] = descriptorBindings[i][j].descriptorActualCount;
    }
    
    VkDescriptorSetVariableDescriptorCountAllocateInfo descriptorSetVariableDescriptorCountAllocateInfo{};
    descriptorSetVariableDescriptorCountAllocateInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    descriptorSetVariableDescriptorCountAllocateInfo.descriptorSetCount = static_cast<uint32_t>(descriptorVariableCounts.size()) ;
    descriptorSetVariableDescriptorCountAllocateInfo.pDescriptorCounts = descriptorVariableCounts.data();

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts(numSets, descriptorSetLayout);
    
    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = numSets;
    descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();
    descriptorSetAllocateInfo.pNext = &descriptorSetVariableDescriptorCountAllocateInfo;

    std::vector<VkDescriptorSet> descriptorSets(numSets) ;
    VkResult result = vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, descriptorSets.data());
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("ERROR::FAILURE TO ALLOCATE DESCRIPTOR SETS");
    
    return descriptorSets;
}

void DescriptorSet::Bind(VkCommandBuffer commandBuffer, VkPipelineBindPoint bindPoint, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet> descriptorSets)
{
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
}
