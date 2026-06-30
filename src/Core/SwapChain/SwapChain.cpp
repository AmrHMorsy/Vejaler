#include "SwapChain.h"




void SwapChain::Build(GLFWwindow * window, ContextInfo& context)
{
    context.swapChain.extent = GetSwapChainExtent(window, context.physicalDevicePack.capabilities);
        
    context.swapChain.swapChainSurface = GetSwapChainSurfaceFormat(context.physicalDevicePack.formats);
    
    context.swapChain.swapChainPresentMode = GetSwapChainPresentMode(context.physicalDevicePack.presentModes);
    
    context.swapChain.imageCount = GetImageCount(context.physicalDevicePack.capabilities);
    
    Create(context);
    
    context.swapChain.images.resize(context.swapChain.imageCount);
    vkGetSwapchainImagesKHR(context.logicalDevice, context.swapChain.handle, &context.swapChain.imageCount, context.swapChain.images.data());
        
    context.swapChain.imageViews.resize(context.swapChain.imageCount);
    for(size_t i = 0; i < context.swapChain.imageCount; i++)
        context.swapChain.imageViews[i] = ImageView::Build(ImageViewBuildInfo{
            .format = context.swapChain.swapChainSurface.format,
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .image = context.swapChain.images[i],
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseArrayLayer = 0,
            .baseMipLevel = 0,
            .levelCount = 1,
            .layerCount = 1,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            }
        });
}

uint32_t SwapChain::GetImageCount(VkSurfaceCapabilitiesKHR capabilities)
{
    uint32_t imageCount;
    imageCount = capabilities.minImageCount + 1;
    if(capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        imageCount = capabilities.maxImageCount;

    return imageCount;
}

void SwapChain::Create(ContextInfo& context)
{
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = context.windowSurface;
    createInfo.minImageCount = context.swapChain.imageCount;
    createInfo.imageFormat = context.swapChain.swapChainSurface.format;
    createInfo.imageColorSpace = context.swapChain.swapChainSurface.colorSpace;
    createInfo.imageExtent = context.swapChain.extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = context.physicalDevicePack.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = context.swapChain.swapChainPresentMode;
    createInfo.clipped = VK_TRUE;
    
    VkResult result = vkCreateSwapchainKHR(context.logicalDevice, &createInfo, nullptr, &context.swapChain.handle);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE SWAP CHAIN\033[0m");
}

VkPresentModeKHR SwapChain::GetSwapChainPresentMode(std::vector<VkPresentModeKHR> presentModes)
{
    for(auto mode : presentModes)
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
            return mode;
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR SwapChain::GetSwapChainSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats)
{
    for( int i = 0 ; i < formats.size() ; i++ ){
        if(formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return formats[i] ;
    }

    return formats[0];
}

VkExtent2D SwapChain::GetSwapChainExtent(GLFWwindow* window, VkSurfaceCapabilitiesKHR capabilities)
{
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        if(capabilities.currentExtent.width != static_cast<uint32_t>(fbWidth) ||
            capabilities.currentExtent.height != static_cast<uint32_t>(fbHeight)){
            VkExtent2D actualExtent = {static_cast<uint32_t>(fbWidth), static_cast<uint32_t>(fbHeight)};
            actualExtent.width  = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            return actualExtent;
        }
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    actualExtent.width  = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    
    return actualExtent;
}

uint32_t SwapChain::AcquireNextImage(ContextInfo& context, WindowInfo& window, uint32_t frame)
{
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(context.logicalDevice, context.swapChain.handle, UINT64_MAX, context.imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
    
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        imageIndex = -1;
    
    else if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31mFailed to Acquire Swap Chain Image \033[0m");

    return imageIndex;
}

bool SwapChain::PresentImage(ContextInfo& context, VkSemaphore& waitSemaphore)
{
    std::vector<VkSemaphore> waitSemaphores = {waitSemaphore};
    std::vector<VkSwapchainKHR> swapChains = {context.swapChain.handle};

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    presentInfo.pWaitSemaphores = waitSemaphores.data();
    presentInfo.swapchainCount = static_cast<uint32_t>(swapChains.size());
    presentInfo.pSwapchains = swapChains.data();
    presentInfo.pImageIndices = &context.imageIndex;
    
    VkResult result = vkQueuePresentKHR(context.queuePack.presentQueue, &presentInfo);

    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        return false;
    
    else if(result != VK_SUCCESS)
        throw std::runtime_error( "\033[31mFailed To present Swap Chain Image \033[0m");
    
    return true;
}

void SwapChain::Destroy(VkDevice logicalDevice, SwapChainInfo& swapChain)
{
    for(size_t i = 0; i < swapChain.imageViews.size(); i++)
        ImageView::Destroy(logicalDevice, swapChain.imageViews[i]);
    
    vkDestroySwapchainKHR(logicalDevice, swapChain.handle, NULL);
}
