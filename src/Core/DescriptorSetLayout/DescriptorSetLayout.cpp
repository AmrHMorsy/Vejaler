#include "DescriptorSetLayout.h"




VkDescriptorSetLayout DescriptorSetLayout::Create(VkDevice logicalDevice, const std::vector<DescriptorBinding>& descriptorBindings)
{
    uint32_t numBindings = static_cast<uint32_t>(descriptorBindings.size());
    
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings = GetDescriptorSetLayoutBindings(descriptorBindings, numBindings);
    
    std::vector<VkDescriptorBindingFlags> descriptorBindingFlags = GetDescriptorBindingFlags(descriptorBindings, numBindings);
    
    VkDescriptorSetLayoutBindingFlagsCreateInfo descriptorSetLayoutBindingFlagsCreateInfo{};
    descriptorSetLayoutBindingFlagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    descriptorSetLayoutBindingFlagsCreateInfo.bindingCount = numBindings;
    descriptorSetLayoutBindingFlagsCreateInfo.pBindingFlags = descriptorBindingFlags.data();
    
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    descriptorSetLayoutCreateInfo.bindingCount = numBindings,
    descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings.data();
    descriptorSetLayoutCreateInfo.pNext = &descriptorSetLayoutBindingFlagsCreateInfo;
    
    VkDescriptorSetLayout descriptorSetLayout;
    VkResult result = vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout);
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE DESCRIPTOR SET LAYOUT\033[0m");
    
    return descriptorSetLayout;
}

std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayout::GetDescriptorSetLayoutBindings(const std::vector<DescriptorBinding>& descriptorBindings, uint32_t numBindings)
{
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings(numBindings);

    for(size_t i = 0; i < numBindings; i++){
        descriptorSetLayoutBindings[i].binding = descriptorBindings[i].binding ;
        descriptorSetLayoutBindings[i].descriptorCount = descriptorBindings[i].descriptorMaxCapacity;
        descriptorSetLayoutBindings[i].descriptorType = descriptorBindings[i].descriptorType ;
        descriptorSetLayoutBindings[i].pImmutableSamplers = nullptr;
        descriptorSetLayoutBindings[i].stageFlags = descriptorBindings[i].stageFlag;
    }

    return descriptorSetLayoutBindings;
}

std::vector<VkDescriptorBindingFlags> DescriptorSetLayout::GetDescriptorBindingFlags(const std::vector<DescriptorBinding>& descriptorBindings, uint32_t numBindings)
{
    std::vector<VkDescriptorBindingFlags> descriptorBindingFlags(numBindings);

    for(size_t i = 0; i < numBindings; i++)
        descriptorBindingFlags[i] = descriptorBindings[i].descriptorBindingFlags;

    return descriptorBindingFlags;
}
