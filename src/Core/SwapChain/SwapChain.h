#pragma once
#include "Data/SwapChainInfo.h"
#include "../ImageView/ImageView.h"
#include "../../Framework/Context/Data/ContextInfo.h"
#include "../Window/Data/WindowInfo.h"



class SwapChain{
 
private:
    
    static uint32_t GetImageCount(VkSurfaceCapabilitiesKHR capabilities);
    
    static VkPresentModeKHR GetSwapChainPresentMode(std::vector<VkPresentModeKHR> presentModes);
    
    static VkSurfaceFormatKHR GetSwapChainSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats);
    
    static VkExtent2D GetSwapChainExtent(GLFWwindow * window, VkSurfaceCapabilitiesKHR capabilities);
    
    static void Create(ContextInfo& context);
    
public:
    
    static void Build(GLFWwindow * window, ContextInfo& context);
    
    static void Destroy(VkDevice logicalDevice, SwapChainInfo& swapChain);
    
    static uint32_t AcquireNextImage(ContextInfo& context, WindowInfo& window, uint32_t frame);
    
    static bool PresentImage(ContextInfo& context, VkSemaphore& waitSemaphore);
};
