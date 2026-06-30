#include "LogicalDevice.h"



VkDevice LogicalDevice::Build(VkPhysicalDevice physicalDevice, uint32_t queueIndex)
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
        
    std::vector<VkDeviceQueueCreateInfo> queue = BuildDeviceQueue(queueIndex) ;
    
    VkPhysicalDeviceFeatures deviceFeatures{} ;
    deviceFeatures.samplerAnisotropy = VK_TRUE ;
    deviceFeatures.tessellationShader = VK_TRUE ;
    const std::vector<const char*> logicalDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME} ;
    
    VkPhysicalDeviceDescriptorIndexingFeatures indexing{};
    indexing.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    indexing.runtimeDescriptorArray = VK_TRUE;
    indexing.descriptorBindingPartiallyBound = VK_TRUE;
    indexing.descriptorBindingVariableDescriptorCount = VK_TRUE;
    indexing.shaderUniformBufferArrayNonUniformIndexing = VK_TRUE;
    
    VkPhysicalDeviceFeatures2 features2{};
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &indexing;
    
    VkDeviceCreateInfo createInfo{} ;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO ;
    createInfo.pNext = &features2;
    createInfo.queueCreateInfoCount = 2 ;
    createInfo.pQueueCreateInfos = queue.data() ;
    createInfo.pEnabledFeatures = &deviceFeatures ;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(logicalDeviceExtensions.size()) ;
    createInfo.ppEnabledExtensionNames = logicalDeviceExtensions.data() ;
    createInfo.enabledLayerCount = 0 ;
    
    VkDevice logicalDevice;
    VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
    if(result!= VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE LOGICAL DEVICE\033[0m");
    
    return logicalDevice;
}

std::vector<VkDeviceQueueCreateInfo> LogicalDevice::BuildDeviceQueue(uint32_t queueIndex)
{
    float queuePriority = 1.0f ;
    std::vector<VkDeviceQueueCreateInfo> queue ;

    for ( int i = 0 ; i < 2 ; i++ ) {
        VkDeviceQueueCreateInfo queueCreateInfo{} ;
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO ;
        queueCreateInfo.queueFamilyIndex = queueIndex ;
        queueCreateInfo.queueCount = 1 ;
        queueCreateInfo.pQueuePriorities = &queuePriority ;
        queue.push_back( queueCreateInfo ) ;
    }
    
    return queue ;
}

void LogicalDevice::WaitIdle(VkDevice logicalDevice)
{
    vkDeviceWaitIdle(logicalDevice) ;
}

void LogicalDevice::Destroy(VkDevice logicalDevice)
{
    vkDestroyDevice(logicalDevice, NULL) ;
}
