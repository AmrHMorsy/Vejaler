#include "Window.h"




void Window::Build(WindowInfo& window)
{
    GLFWManager::Initialize();
    
    GLFWmonitor * primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * mode = glfwGetVideoMode(primaryMonitor);
    window.handle = glfwCreateWindow(mode->width, mode->height, "Vejaler", NULL, NULL);
    
    if(window.handle == NULL)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE WINDOW\033[0m");
    
    glfwSetWindowSizeLimits(window.handle, 800, 800, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwGetFramebufferSize(window.handle, &window.frameBufferWidth, &window.frameBufferHeight);
    glfwGetWindowSize(window.handle, &window.windowWidth, &window.windowHeight);
    glfwGetWindowContentScale(window.handle, &window.xScale, &window.yScale);
}

void Window::HandleWindowResize(WindowInfo& window)
{
    glfwGetFramebufferSize(window.handle, &window.frameBufferWidth, &window.frameBufferHeight);
    glfwGetWindowSize(window.handle, &window.windowWidth, &window.windowHeight);
    glfwGetWindowContentScale(window.handle, &window.xScale, &window.yScale);
    while(window.frameBufferWidth == 0 || window.frameBufferHeight == 0){
        glfwGetFramebufferSize(window.handle, &window.frameBufferWidth, &window.frameBufferHeight);
        glfwGetWindowSize(window.handle, &window.windowWidth, &window.windowHeight);
        glfwGetWindowContentScale(window.handle, &window.xScale, &window.yScale);
        glfwWaitEvents();
    }
}

void Window::Destroy(WindowInfo& window)
{
    glfwDestroyWindow(window.handle);
}
