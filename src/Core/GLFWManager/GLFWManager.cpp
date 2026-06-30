#include "GLFWManager.h"




void GLFWManager::Initialize()
{
    if(glfwInit() == GL_FALSE)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO INITIALIZE GLFW\033[0m");
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
}

void GLFWManager::Terminate()
{
    glfwTerminate();
}
