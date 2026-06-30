#include "PhysicalDevice.h"



PhysicalDevicePack PhysicalDevice::PickPhysicalDevice(PhysicalDevicePickConfiguration configuration)
{
    uint32_t queueIndex;
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    std::vector<VkPhysicalDevice> physicalDevices = FindPhysicalDevices(configuration.vulkanInstance) ;

    for( int i = 0 ; i < physicalDevices.size() ; i++ ){
        queueIndex = FindQueueFamilies(physicalDevices[i]) ;
        capabilities = QueryPhysicalDeviceCapabilities(configuration.windowSurface, physicalDevices[i]) ;
        formats = QueryPhysicalDeviceFormats(configuration.windowSurface, physicalDevices[i]) ;
        presentModes = QueryPhysicalDevicePresentModes(configuration.windowSurface, physicalDevices[i]) ;
        if(IsDeviceSuitable(DeviceSuitabilityConfiguration{
            .formats = formats,
            .queueIndex = queueIndex,
            .presentModes = presentModes,
            .physicalDevice = physicalDevices[i]
        })){
            VkBool32 isSupported = false ;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], queueIndex, configuration.windowSurface, &isSupported) ;
            if(isSupported){
                physicalDevice = physicalDevices[i] ;
                break ;
            }
        }
    }
    if( physicalDevice == VK_NULL_HANDLE )
        throw std::runtime_error( "\033[31mFAILURE TO FIND SUITABLE PHYSICAL DEVICE\033[0m" ) ;
    
    int maxPerStageDescriptorSampledImages = QueryMaxPerStageDescriptorSampledImages(physicalDevice);

    return PhysicalDevicePack{
        .formats = formats,
        .queueIndex = queueIndex,
        .presentModes = presentModes,
        .capabilities = capabilities,
        .physicalDevice = physicalDevice,
        .maxPerStageDescriptorSampledImages = maxPerStageDescriptorSampledImages
    };
}

bool PhysicalDevice::IsDeviceSuitable(DeviceSuitabilityConfiguration configuration)
{
    bool extensionsSupported = CheckDeviceExtensionSupport(configuration.physicalDevice) ;
    bool swapChainSupported = CheckSwapChainSupport(configuration.formats, configuration.presentModes);
    
    VkPhysicalDeviceFeatures supportedFeatures ;
    vkGetPhysicalDeviceFeatures(configuration.physicalDevice, &supportedFeatures) ;
    
    if(!supportedFeatures.tessellationShader)
        throw std::runtime_error( "\033[31m\nERROR::TESSELLATION SHADER IS NOT SUPPORTED ON THIS DEVICE\033[0m");
    
    if(configuration.queueIndex == -1 || !extensionsSupported || !swapChainSupported || !supportedFeatures.samplerAnisotropy)
        return false ;

    return true ;
}

int PhysicalDevice::QueryMaxPerStageDescriptorSampledImages(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties properties ;
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    
    return properties.limits.maxPerStageDescriptorSampledImages ;
}

bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL) ;
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount) ;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, availableExtensions.data()) ;
    
    const std::vector<const char*> logicalDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    std::set<std::string> requiredExtensions(logicalDeviceExtensions.begin(), logicalDeviceExtensions.end()) ;

    for(const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName) ;

    return requiredExtensions.empty() ;
}

bool PhysicalDevice::CheckSwapChainSupport(std::vector<VkSurfaceFormatKHR> formats, std::vector<VkPresentModeKHR> presentModes)
{
    return !formats.empty() && !presentModes.empty();
}

VkSurfaceCapabilitiesKHR PhysicalDevice::QueryPhysicalDeviceCapabilities(VkSurfaceKHR windowSurface, VkPhysicalDevice physicalDevice)
{
    VkSurfaceCapabilitiesKHR capabilities ;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, windowSurface, &capabilities) ;
    
    return capabilities ;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::QueryPhysicalDeviceFormats(VkSurfaceKHR windowSurface, VkPhysicalDevice physicalDevice)
{
    uint32_t formatCount ;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, windowSurface, &formatCount, nullptr);
    
    if(formatCount == 0)
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO FIND DEVICE FORMATS\033[0m");

    std::vector<VkSurfaceFormatKHR> formats(formatCount) ;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, windowSurface, &formatCount, formats.data()) ;

    return formats ;
}

std::vector<VkPresentModeKHR> PhysicalDevice::QueryPhysicalDevicePresentModes(VkSurfaceKHR windowSurface, VkPhysicalDevice physicalDevice)
{
    uint32_t presentModeCount ;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, windowSurface, &presentModeCount, nullptr);
    
    if(presentModeCount == 0)
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO FIND PRESENT MODES\033[0m");

    std::vector<VkPresentModeKHR> presentModes(presentModeCount) ;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, windowSurface, &presentModeCount, presentModes.data()) ;

    return presentModes ;
}

int PhysicalDevice::FindQueueFamilies(VkPhysicalDevice physicalDevice)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
    
    if(queueFamilyCount == 0)
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO FIND QUEUE FAMILIES\033[0m");
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    
    for(int i = 0 ; i < queueFamilies.size() ; i++){
        if(queueFamilies[i].queueFlags && VK_QUEUE_GRAPHICS_BIT)
            return i;
    }
    return -1;
}

std::vector<VkPhysicalDevice> PhysicalDevice::FindPhysicalDevices(VkInstance vulkanInstance)
{
    uint32_t deviceCount = 0 ;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, NULL);
    
    if(deviceCount == 0)
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO FIND GPUs WITH VULKAN SUPPORT\033[0m" ) ;
    
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount) ;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());
    
    return physicalDevices;
}

VkFormat PhysicalDevice::QuerySupportedFormat(SupportedFormatQueryConfiguration configuration)
{
    for(VkFormat format : configuration.candidates){
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(configuration.physicalDevice, format, &properties);

        if(configuration.imageTiling == VK_IMAGE_TILING_LINEAR){
            VkFormatFeatureFlags aggregatedFlags = properties.linearTilingFeatures & configuration.formatFeatureFlags;
            if(aggregatedFlags == configuration.formatFeatureFlags)
                return format ;
        }
        else if(configuration.imageTiling == VK_IMAGE_TILING_OPTIMAL){
            VkFormatFeatureFlags aggregatedFlags = properties.optimalTilingFeatures & configuration.formatFeatureFlags;
            if(aggregatedFlags == configuration.formatFeatureFlags)
                return format ;
        }
    }

    throw std::runtime_error("\033[31m\nERROR::FAILURE TO FIND SUPPORTED FORMAT\033[0m") ;
}
