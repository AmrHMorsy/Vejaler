#include "Context.h"




void Context::Build(ContextInfo& context, const WindowInfo& window)
{
    context.vulkanInstance = Core::VulkanInstance::Build();
    
    context.windowSurface = Core::WindowSurface::Build(window.handle, context.vulkanInstance);
    
    context.physicalDevicePack = Core::PhysicalDevice::PickPhysicalDevice(PhysicalDevicePickConfiguration{
        .vulkanInstance = context.vulkanInstance,
        .windowSurface = context.windowSurface
    });
    
    context.logicalDevice = Core::LogicalDevice::Build(context.physicalDevicePack.physicalDevice, context.physicalDevicePack.queueIndex);
    
    context.queuePack = Core::Queue::GetDeviceQueue(GetDeviceQueueConfiguration{
        .logicalDevice = context.logicalDevice,
        .queueIndex = context.physicalDevicePack.queueIndex,
        .queueFamilyIndex = 0
    });
    
    context.depthFormat = Core::PhysicalDevice::QuerySupportedFormat(SupportedFormatQueryConfiguration{
        .candidates = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        .imageTiling = VK_IMAGE_TILING_OPTIMAL,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .formatFeatureFlags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    });
    
    Core::SwapChain::Build(window.handle, context);

    context.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        context.imageAvailableSemaphores[i] = Core::Semaphore::Build(context.logicalDevice);
    
    Core::Audio::Build();
}

void Context::HandleWindowResize(ContextInfo& context, const WindowInfo& window)
{
    SwapChain::Destroy(context.logicalDevice, context.swapChain);
    
    SwapChain::Build(window.handle, context);
}

void Context::Destroy(ContextInfo& context)
{
    Core::LogicalDevice::Destroy(context.logicalDevice);
    
    Core::WindowSurface::Destroy(context.vulkanInstance, context.windowSurface);
    
    Core::VulkanInstance::Destroy(context.vulkanInstance);
}
