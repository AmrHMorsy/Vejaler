#include "../../Includes.h"



class WindowSurface {
    
public:

    static VkSurfaceKHR Build(GLFWwindow * window, VkInstance& vulkanInstance);
    static void Destroy(VkInstance& vulkanInstance, VkSurfaceKHR& windowSurface);
};
