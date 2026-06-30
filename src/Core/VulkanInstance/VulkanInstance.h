#include "../../Includes.h"




class VulkanInstance {
    
private:
    
    static std::vector<const char *> GetGLFWExtensions();

public:
    
    static VkInstance Build();
    static void Destroy(VkInstance& vulkanInstance);
};
