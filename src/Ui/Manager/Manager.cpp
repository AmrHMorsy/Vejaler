#include "Manager.h"
#include "../Text/Text.h"
#include "../Element/Element.h"
#include "../Sidebar/Sidebar.h"
#include "../Panels/FilePanel/FilePanel.h"
#include "../Panels/LightPanel/LightPanel.h"
#include "../Panels/ModelPanel/ModelPanel.h"
#include "../Panels/ScenePanel/ScenePanel.h"
#include "../Panels/CameraPanel/CameraPanel.h"
#include "../Panels/SkyboxPanel/SkyboxPanel.h"
#include "../Panels/ShadingPanel/ShadingPanel.h"
#include "../Panels/TransformPanel/TransformPanel.h"
#include "../Panels/UserGuidePanel/UserGuidePanel.h"
#include "../Panels/EditingModePanel/EditingModePanel.h"
#include "../Panels/PerformancePanel/PerformancePanel.h"
#include "../Panels/OptimizationPanel/OptimizationPanel.h"



void Ui::Manager::Build(Ui::ManagerInfo& ui, ContextInfo& context, WindowInfo& window)
{
    ui.renderPass = RenderPass::BuildRenderPass( RenderPassBuildConfiguration{
        .logicalDevice = context.logicalDevice,
        .subpassDependencyBuildConfigurations = std::vector<SubpassDependencyBuildConfiguration>{
            SubpassDependencyBuildConfiguration{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            }
        },
        .attachmentDescriptionBuildConfigurations = std::vector<AttachmentDescriptionBuildConfiguration>{
            AttachmentDescriptionBuildConfiguration{
                .format = context.swapChain.swapChainSurface.format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            }
        },
        .subpassDescriptionBuildConfigurations = std::vector<SubpassDescriptionBuildConfiguration>{
            SubpassDescriptionBuildConfiguration{
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .colorAttachmentReferences = std::vector<VkAttachmentReference>{
                    VkAttachmentReference{
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    }
                }
            }
        }
    });
    
    ui.commandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    ui.commandBuffers = CommandBuffer::Allocate(context.logicalDevice, ui.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);
    
    ui.inflightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        ui.inflightFences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    ui.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        ui.imageAvailableSemaphores[i] = Semaphore::Build(context.logicalDevice) ;
    
    ui.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        ui.renderFinishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    uint32_t numSwapChainImages = context.swapChain.imageCount;
    ui.frameBuffers.resize(numSwapChainImages) ;
    for(size_t i = 0; i < numSwapChainImages; i++)
        ui.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, ui.renderPass, context.swapChain.extent, 1, {context.swapChain.imageViews[i]});
    
    Init(context, window, ui);
}

void Ui::Manager::Init(ContextInfo& context, WindowInfo& window, Ui::ManagerInfo& ui)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    static const ImWchar ranges[] = {0x0020, 0x00FF, 0x2190, 0x21FF, 0};
    io.Fonts->AddFontFromFileTTF("../Resources/assets/Fonts/EpundaSans.ttf", 25.0f, nullptr, ranges);
    
    io.DisplayFramebufferScale = ImVec2(window.xScale, window.yScale);
    
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("../Resources/assets/Fonts/Font Awesome 7 Free-Solid-900.otf", 16.0f, &icons_config, icons_ranges);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard ;
    ImGui::StyleColorsDark();
    
    ui.font = io.Fonts->AddFontFromFileTTF("../Resources/assets/Fonts/CourierNew.ttf", 40.0f, nullptr, ranges);
    ui.boldFont = io.Fonts->AddFontFromFileTTF("../Resources/assets/Fonts/CourierNewBold.ttf", 40.0f, nullptr, ranges);
    
    ImGui::GetIO().IniFilename = nullptr;

    ImGui_ImplGlfw_InitForVulkan(window.handle, true) ;
    
    
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = context.vulkanInstance;
    initInfo.PhysicalDevice = context.physicalDevicePack.physicalDevice;
    initInfo.Device = context.logicalDevice;
    initInfo.Queue = context.queuePack.graphicsQueue;
    initInfo.DescriptorPool = nullptr;
    initInfo.DescriptorPoolSize = 1000;
    initInfo.MinImageCount = context.swapChain.imageCount;
    initInfo.ImageCount = context.swapChain.imageCount;
    initInfo.UseDynamicRendering = false;
    initInfo.PipelineInfoMain.RenderPass = ui.renderPass;
    initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    if(!ImGui_ImplVulkan_Init(&initInfo))
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO INITIALIZE IMGUI VULKAN BACKEND\n\033[0m");
}

void Ui::Manager::HandleWindowResize(ContextInfo& context, Ui::ManagerInfo& ui)
{
    uint32_t n = context.swapChain.imageCount;
    for(size_t i = 0; i < n; i++){
        FrameBuffer::Destroy(context.logicalDevice, ui.frameBuffers[i]);
        ui.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, ui.renderPass, context.swapChain.extent, 1, {context.swapChain.imageViews[i]});
    }
}

void Ui::Manager::Load(WindowInfo& window, Ui::ManagerInfo& ui, nlohmann::json j)
{
    if(!j.contains("uiElements"))
        return;
    
    Ui::Element::Load(window, ui, j["uiElements"]);
}

void Ui::Manager::Record(ApplicationInfo& application)
{
    Ui::ManagerInfo& ui = application.ui;
    
    Fence::Wait(application.context.logicalDevice, {ui.inflightFences[application.frame]}, VK_TRUE, UINT64_MAX);
        
    Fence::Reset(application.context.logicalDevice, {ui.inflightFences[application.frame]});

    CommandBuffer::Reset(ui.commandBuffers[application.frame]) ;
    
    CommandBuffer::Begin(ui.commandBuffers[application.frame], 0, nullptr);
        
    RenderPass::BeginRenderPass( RenderPassBeginConfiguration{
        .commandBuffer = ui.commandBuffers[application.frame],
        .frameBuffer = ui.frameBuffers[application.frame],
        .resolution = application.context.swapChain.extent,
        .renderPass = ui.renderPass,
        .clearValues = std::vector<VkClearValue>{
            { .color = {{0.0f, 0.0f, 0.0f, 1.0f}} },
            { .depthStencil = {1.0f, 0} }}
    }) ;
    
    Viewport::Set(ui.commandBuffers[application.frame], 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(application.context.swapChain.extent.width),
            .height = static_cast<float>(application.context.swapChain.extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = ui.commandBuffers[application.frame],
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = application.context.swapChain.extent
            }
        }
    });
    
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    Ui::Sidebar::Render(application.scenePack, application.window.windowWidth, application.window.windowHeight);
    
    Ui::FilePanel::Render(application.scenePack, application.context, application.window.windowWidth, application.window.windowHeight, application);
    
    for(size_t i = 0; i < ui.elements.size(); i++)
        Ui::Element::Render(ui.elements[i], application.scenePack.scenes[application.scenePack.selected].modelPack, application.variables);
    
    if(application.scenePack.scenes.size() > 0){
        SceneInfo& scene = application.scenePack.scenes[application.scenePack.selected];
        
        double xpos, ypos;
        glfwGetCursorPos(application.window.handle, &xpos, &ypos);
        
        Ui::Text::Render(ui.font, application.textPack, application.variables, xpos, ypos);
        
        Ui::PerformancePanel::Render(scene, application.window.windowWidth, application.window.windowHeight, application.frame);
        Ui::EditingModePanel::Render(scene, application.window.windowWidth, application.window.windowHeight);
        Ui::ModelPanel::Render(application.context, scene, scene.modelPack, scene.skyboxPack, scene.lightPack, application.window.windowWidth, application.window.windowHeight, application);
        Ui::CameraPanel::Render(application.context, scene, application.window.windowWidth, application.window.windowHeight);
        Ui::LightPanel::Render(application.context, scene, application.window.windowWidth, application.window.windowHeight);
        Ui::SkyboxPanel::Render(application.context, scene, application.window.windowWidth, application.window.windowHeight);
        Ui::ScenePanel::Render(application.scenePack, application.window.windowWidth, application.window.windowHeight);
        Ui::OptimizationPanel::Render(application.window.windowWidth, application.window.windowHeight, scene);
    }
    
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ui.commandBuffers[application.frame]);
    
    RenderPass::EndRenderPass(ui.commandBuffers[application.frame]) ;
    
    CommandBuffer::End(ui.commandBuffers[application.frame]);
}

void Ui::Manager::Submit(Ui::ManagerInfo& ui, ScenePack& scenePack, WindowInfo& window, uint32_t frame, ContextInfo& context)
{
    std::vector<VkSemaphore> waitSemaphores;
    std::vector<VkPipelineStageFlags> waitStages;
    
    if(!scenePack.scenes.empty()){
        waitSemaphores.push_back(scenePack.scenes[scenePack.selected].renderFinishedSemaphores[frame]);
        waitStages.push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    }
    
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = ui.inflightFences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = waitSemaphores,
                .signalSemaphores = {ui.renderFinishedSemaphores[frame]},
                .commandBuffers = {ui.commandBuffers[frame]},
                .waitStages = waitStages
            }
        }
    });
}

void Ui::Manager::Destroy()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
