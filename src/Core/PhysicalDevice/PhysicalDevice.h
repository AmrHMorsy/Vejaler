#pragma once
#include "Data/PhysicalDevicePack.h"
#include "Data/DeviceSuitabilityConfiguration.h"
#include "Data/PhysicalDevicePickConfiguration.h"
#include "Data/SupportedFormatQueryConfiguration.h"



class PhysicalDevice {

private:
    
    static bool IsDeviceSuitable(DeviceSuitabilityConfiguration configuration);
    static int QueryMaxPerStageDescriptorSampledImages(VkPhysicalDevice physicalDevice);
    static bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
    static bool CheckSwapChainSupport(std::vector<VkSurfaceFormatKHR> formats, std::vector<VkPresentModeKHR> presentModes);
    static VkSurfaceCapabilitiesKHR QueryPhysicalDeviceCapabilities(VkSurfaceKHR windowSurface, VkPhysicalDevice physicalDevice);
    static std::vector<VkSurfaceFormatKHR> QueryPhysicalDeviceFormats(VkSurfaceKHR windowSurface, VkPhysicalDevice physicalDevice);
    static std::vector<VkPresentModeKHR> QueryPhysicalDevicePresentModes(VkSurfaceKHR windowSurface, VkPhysicalDevice physicalDevice);
    static int FindQueueFamilies(VkPhysicalDevice physicalDevice);
    static std::vector<VkPhysicalDevice> FindPhysicalDevices(VkInstance vulkanInstance);

public:
    
    static PhysicalDevicePack PickPhysicalDevice(PhysicalDevicePickConfiguration configuration);
    static VkFormat QuerySupportedFormat(SupportedFormatQueryConfiguration configuration);
};
