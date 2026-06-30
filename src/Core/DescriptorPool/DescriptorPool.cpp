#include "DescriptorPool.h"



VkDescriptorPool DescriptorPool::Build(VkDevice logicalDevice, const std::vector<DescriptorBinding>& descriptorBindings, uint32_t numSets)
{
    size_t numBindings = descriptorBindings.size();
    
    std::unordered_map<VkDescriptorType, uint32_t> descriptorCountMap;
    for(size_t i = 0; i < numBindings; i++)
        descriptorCountMap[descriptorBindings[i].descriptorType] += descriptorBindings[i].descriptorMaxCapacity;

    std::vector<VkDescriptorPoolSize> descriptorPoolSizes ;
    for(const auto & [type, count] : descriptorCountMap){
        VkDescriptorPoolSize descriptorPoolSize{};
        descriptorPoolSize.type = type;
        descriptorPoolSize.descriptorCount = numSets * count;
        descriptorPoolSizes.push_back(descriptorPoolSize) ;
    }
    
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{} ;
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO ;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data() ;
    descriptorPoolCreateInfo.maxSets = numSets;
    
    VkDescriptorPool descriptorPool;
    VkResult result = vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &descriptorPool);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE DESCRIPTOR POOL\033[0m");
        
    return descriptorPool;
}

VkDescriptorPool DescriptorPool::Build2(VkDevice logicalDevice, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings)
{
    uint32_t numSets = descriptorBindings.size();
    std::unordered_map<VkDescriptorType, uint32_t> descriptorCountMap;
    
    for(uint32_t i = 0; i < numSets; i++){
        for(size_t j = 0; j < descriptorBindings[i].size(); j++)
            descriptorCountMap[descriptorBindings[i][j].descriptorType] += descriptorBindings[i][j].descriptorMaxCapacity;
    }

    std::vector<VkDescriptorPoolSize> descriptorPoolSizes ;
    for(const auto & [type, count] : descriptorCountMap){
        VkDescriptorPoolSize descriptorPoolSize{};
        descriptorPoolSize.type = type;
        descriptorPoolSize.descriptorCount = count;
        descriptorPoolSizes.push_back(descriptorPoolSize) ;
    }
    
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{} ;
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO ;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data() ;
    descriptorPoolCreateInfo.maxSets = numSets;
    
    VkDescriptorPool descriptorPool;
    VkResult result = vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &descriptorPool);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE DESCRIPTOR POOL\033[0m");
        
    return descriptorPool;
}

void DescriptorPool::Destroy(VkDevice logicalDevice, VkDescriptorPool descriptorPool)
{
    if(descriptorPool != VK_NULL_HANDLE)
        vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
}

void DescriptorPool::Reset(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    vkResetDescriptorPool(logicalDevice, descriptorPool, flags);
}
