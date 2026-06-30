#include "Sampler.h"



VkSampler Sampler::Build(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const SamplerConfig& config)
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    
    VkSamplerCreateInfo samplerCreateInfo{};
    
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = config.magFilter;
    samplerCreateInfo.minFilter = config.minFilter;
    samplerCreateInfo.addressModeU = config.addressMode;
    samplerCreateInfo.addressModeV = config.addressMode;
    samplerCreateInfo.addressModeW = config.addressMode;
    samplerCreateInfo.anisotropyEnable = config.anisotropyEnable;
    samplerCreateInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerCreateInfo.borderColor = config.borderColor;
    samplerCreateInfo.unnormalizedCoordinates = config.unnormalizedCoordinates;
    samplerCreateInfo.compareEnable = config.compareEnable;
    samplerCreateInfo.compareOp = config.compareOp;
    samplerCreateInfo.mipmapMode = config.mipMapMode;
    samplerCreateInfo.mipLodBias = config.mipLodBias;
    samplerCreateInfo.minLod = config.minLod;
    samplerCreateInfo.maxLod = config.maxLod;
    
    VkSampler sampler;
    VkResult result = vkCreateSampler(logicalDevice, &samplerCreateInfo, nullptr, &sampler);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE SAMPLER\n\033[0m");

    return sampler;
}

void Sampler::Destroy(VkDevice logicalDevice, VkSampler& sampler)
{
    vkDestroySampler(logicalDevice, sampler, nullptr);
}
