#include "Application.h"
#include "../Core/Core.h"
#include "../Ui/Text/Text.h"
#include "../Scene/Scene/Scene.h"
#include "../Framework/Framework.h"






static void WindowResizeCallBack(GLFWwindow* window, int width, int height)
{
    ApplicationInfo* application = static_cast<ApplicationInfo*>(glfwGetWindowUserPointer(window));
        
    Queue::WaitIdle(application->context.queuePack.graphicsQueue);
    
    LogicalDevice::WaitIdle(application->context.logicalDevice);
    
    Window::HandleWindowResize(application->window);
    
    SwapChain::Destroy(application->context.logicalDevice, application->context.swapChain);
    
    SwapChain::Build(application->window.handle, application->context);
    
    Scene::HandleWindowResize(application->context, application->scenePack.scenes[application->scenePack.selected]);
    
    Ui::Manager::HandleWindowResize(application->context, application->ui);
}

Application::Application()
{
    Window::Build(application.window);
    
    Context::Build(application.context, application.window);
        
    Ui::Manager::Build(application.ui, application.context, application.window);
    
    Scene::Add(application.scenePack, application.context);
    
    Camera::Add(application.context, application.scenePack.scenes[application.scenePack.selected], application.scenePack.scenes[application.scenePack.selected].screenResolution);
    
    std::ifstream gameFilepath("../Resources/assets/GG.json");
    if(!gameFilepath.is_open())
        return;
    
    application.gameJSON = nlohmann::json::parse(gameFilepath);
    
    Scene::Load(application.scenePack, application.context, application.window, application.gameJSON);
    
    State::Load(application.gameJSON, application.scenePack, application.statePack);
    
    Ui::Text::Load(application.scenePack, application.textPack, application.window, application.gameJSON, application.ui.font);
    
    Ui::Manager::Load(application.window, application.ui, application.gameJSON);
    
    Music::Load(application.gameJSON, application.musicPack);
    
    Variable::Load(application.gameJSON, application.variables);
}

void Application::Launch()
{
    Camera::Switch(application.scenePack.scenes[application.scenePack.selected].cameraPack, application.scenePack.scenes[application.scenePack.selected], 1);
    
    glfwSetWindowUserPointer(application.window.handle, this);
    
    glfwSetWindowSizeCallback(application.window.handle, WindowResizeCallBack);
    
    while(!glfwWindowShouldClose(application.window.handle)){
        application.scenePack.selected = (application.scenePack.selected != application.scenePack.nextSelected)? application.scenePack.nextSelected: application.scenePack.selected;
        
        Events::Handle(application.context, application.window.handle, application.scenePack, application.statePack, application.ui, application.textPack, application.variables);
        
        application.context.imageIndex = SwapChain::AcquireNextImage(application.context, application.window, application.frame);

        if(application.context.imageIndex == -1)
            continue;
        
        Scene::Record(application.scenePack, application.context, application.frame);
        
        Ui::Manager::Record(application);
        
        Scene::Submit(application.scenePack, application.context, application.frame);
        
        Ui::Manager::Submit(application.ui, application.scenePack, application.window, application.frame, application.context);
        
        if(!SwapChain::PresentImage(application.context, application.ui.renderFinishedSemaphores[application.frame]))
           continue;
           
        application.frame = (application.frame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}

Application::~Application()
{
    LogicalDevice::WaitIdle(application.context.logicalDevice);
    
    glfwSetWindowUserPointer(application.window.handle, nullptr);
    
    Window::Destroy(application.window);
    
    Context::Destroy(application.context);

    LogicalDevice::Destroy(application.context.logicalDevice);
    
    WindowSurface::Destroy(application.context.vulkanInstance, application.context.windowSurface);
    
    VulkanInstance::Destroy(application.context.vulkanInstance);
    
    GLFWManager::Terminate();
}
