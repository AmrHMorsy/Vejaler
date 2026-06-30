#include "WindowSurface.h"




VkSurfaceKHR WindowSurface::Build(GLFWwindow* window, VkInstance& vulkanInstance)
{
    VkSurfaceKHR windowSurface{};
    VkResult result = glfwCreateWindowSurface(vulkanInstance, window, nullptr, &windowSurface);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO BUILD WINDOW SURFACE\033[0m");
    
    return windowSurface;
}

void WindowSurface::Destroy(VkInstance& vulkanInstance, VkSurfaceKHR& windowSurface)
{
    vkDestroySurfaceKHR(vulkanInstance, windowSurface, NULL) ;
}
