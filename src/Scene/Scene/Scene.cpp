#include "Scene.h"
#include <fstream>
#include "../../Ui/Text/Data/TextInfo.h"



void Scene::Add(ScenePack& scenePack, ContextInfo& context)
{
    scenePack.names.resize(scenePack.names.size() + 1);
    scenePack.namesPtr.resize(scenePack.namesPtr.size() + 1);
    for(size_t i = 0; i < scenePack.names.size(); i++){
        scenePack.names[i] = "Scene " + std::to_string(i+1);
        scenePack.namesPtr[i] = scenePack.names[i].c_str();
    }
    
    scenePack.scenes.resize(scenePack.scenes.size() + 1);
    scenePack.selected = scenePack.scenes.size() - 1;
    scenePack.nextSelected = scenePack.selected;
    SceneInfo& scene = scenePack.scenes[scenePack.selected];
    
    scene.screenResolution = context.swapChain.extent;
        
    scene.renderPass = RenderPass::BuildRenderPass( RenderPassBuildConfiguration{
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
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            },
            AttachmentDescriptionBuildConfiguration{
                .format = context.depthFormat,
                .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
            }
        },
        .subpassDescriptionBuildConfigurations = std::vector<SubpassDescriptionBuildConfiguration>{
            SubpassDescriptionBuildConfiguration{
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .depthAttachmentReference = VkAttachmentReference{
                    .attachment = 1,
                    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                },
                .colorAttachmentReferences = std::vector<VkAttachmentReference>{
                    VkAttachmentReference{
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    }
                }
            }
        }
    });
    
    scene.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    scene.primaryCommandBuffers = CommandBuffer::Allocate(context.logicalDevice, scene.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);
    
    scene.inflightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        scene.inflightFences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    scene.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        scene.renderFinishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    Model::Build(context, scene);
    
    DepthPrepass::Build(context, scene.depthPrepass);
        
    FrustumCulling::Build(context, scene.frustumCulling, scene.frustumCullingEnable);
    
    NormalPass::Build(context, scene.normalPass, scene.depthPrepass, scene.modelPack);
    
    SSAO::Build(context, scene.ssao, scene.depthPrepass, scene.normalPass, scene.modelPack, scene.modelPack.samplers[NEAREST_CLAMP_INT_OPAQUE_BLACK_SAMPLER]);
    
    DepthPyramid::Build(context, scene.depthPyramid, scene.depthPrepass);
    
    OcclusionCulling::Build(context, scene.occlusionCulling, scene.depthPyramid);
        
    uint32_t n = context.swapChain.imageCount;
    scene.frameBuffers.resize(n) ;
    for(size_t i = 0; i < n; i++)
        scene.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, scene.renderPass, context.swapChain.extent, 1, {context.swapChain.imageViews[i], scene.depthPrepass.imageViews[i]});
        
    Skybox::Build(context, scene);
        
    Light::Build(context, scene.lightPack, scene.depthPrepass, scene.lightCulling, scene.modelPack, scene.cameraPack.cameras[scene.cameraPack.selected]);
}

void Scene::HandleWindowResize(ContextInfo& context, SceneInfo& scene)
{
    scene.screenResolution = context.swapChain.extent;
    scene.modelPack.fsData.screenResolution = glm::vec2(scene.screenResolution.width, scene.screenResolution.height);
    
    Camera::HandleWindowResize(scene, context.swapChain.extent);
    
    DepthPrepass::HandleWindowResize(context, scene.depthPrepass);
    
    NormalPass::HandleWindowResize(context, scene.normalPass, scene.depthPrepass, scene.modelPack);
        
    SSAO::HandleWindowResize(context, scene.ssao, scene.normalPass,  scene.depthPrepass, scene.modelPack, scene.modelPack.samplers[NEAREST_CLAMP_INT_OPAQUE_BLACK_SAMPLER]);
        
    for(size_t i = 0; i < context.swapChain.imageCount; i++){
        FrameBuffer::Destroy(context.logicalDevice, scene.frameBuffers[i]);
        scene.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, scene.renderPass, context.swapChain.extent, 1, {context.swapChain.imageViews[i], scene.depthPrepass.imageViews[i]});
    }
    
    LightCulling::HandleWindowResize(context, scene.lightPack.lightCulling, scene.modelPack, scene.cameraPack.cameras[scene.cameraPack.selected]);
        
    Model::UpdateDescriptorSets(context, scene.modelPack);
    
    for(size_t i = 0; i < scene.lightPack.lights.size(); i++)
        Shadow::Bake(context, scene.modelPack, scene.lightPack, i);
}

void Scene::Load(ScenePack& scenePack, ContextInfo& context, WindowInfo& window, nlohmann::json j)
{    
    if(j.contains("scenes")){
        for(size_t i = 0; i < j["scenes"].size(); i++){
            nlohmann::json s = j["scenes"][i];
            Scene::Add(scenePack, context);
            SceneInfo& scene = scenePack.scenes[scenePack.selected];
            
            if(s.contains("cameras"))
                Camera::Load(s["cameras"], context, scene);
            
            if(s.contains("numLights"))
                scene.lightPack.numLights = s["numLights"];
                                                
            if(s.contains("skyboxes"))
                Skybox::Load(context, scene, s["skyboxes"]);
            
            Queue::WaitIdle(context.queuePack.graphicsQueue);
            
            if(s.contains("models"))
                Model::Load(context, scene, s["models"]);
            
            Light::Load(context, scene.lightPack, scene.modelPack, scene.cameraPack.cameras[scene.cameraPack.selected]);
        }
    }
}

void Scene::Record(ScenePack& scenePack, ContextInfo& context, uint32_t frame)
{
    if(scenePack.scenes.size() == 0)
        return;
    
    SceneInfo& scene = scenePack.scenes[scenePack.selected];
            
    FrustumCulling::Record(context, scene.frustumCulling, scene.modelPack, scene.cameraPack.cameras[scene.cameraPack.selected], frame);
    
    DepthPyramid::Record(context, scene.depthPyramid, frame);
    
    OcclusionCulling::Record(context, scene.occlusionCulling, scene.modelPack, frame);
    
    DepthPrepass::Record(context, scene.depthPrepass, scene.occlusionCulling, scene.modelPack, scene.cameraPack, frame);
            
    LightCulling::Record(context, scene.lightPack, scene.cameraPack.cameras[scene.cameraPack.selected], frame);
    
    NormalPass::Record(scene, context, frame);
    
    SSAO::Record(context, scene.ssao, scene.cameraPack, frame);
    
    Fence::Wait(context.logicalDevice, {scene.inflightFences[frame]}, VK_TRUE, UINT64_MAX);
    
    Fence::Reset(context.logicalDevice, {scene.inflightFences[frame]});
    
    CommandBuffer::Reset(scene.primaryCommandBuffers[frame]) ;
    
    CommandBuffer::Begin(scene.primaryCommandBuffers[frame], 0, nullptr);
            
    RenderPass::BeginRenderPass( RenderPassBeginConfiguration{
        .commandBuffer = scene.primaryCommandBuffers[frame],
        .frameBuffer = scene.frameBuffers[frame],
        .resolution = context.swapChain.extent,
        .renderPass = scene.renderPass,
        .clearValues = std::vector<VkClearValue>{
            { .color = scene.backgroundColor },
            { .depthStencil = {1.0f, 0} }}
    });
    
    Viewport::Set(scene.primaryCommandBuffers[frame], 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(context.swapChain.extent.width),
            .height = static_cast<float>(context.swapChain.extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = scene.primaryCommandBuffers[frame],
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = {0, 0},
                .extent = context.swapChain.extent
            }
        }
    });
    
    Skybox::Render(scene, frame);
    
    Model::Render(context.logicalDevice, scene.modelPack, scene.frustumCulling, scene.occlusionCulling, scene.primaryCommandBuffers[frame], frame);
    
    RenderPass::EndRenderPass(scene.primaryCommandBuffers[frame]) ;
    
    CommandBuffer::End(scene.primaryCommandBuffers[frame]);
}

void Scene::Submit(ScenePack& scenePack, ContextInfo& context, uint32_t frame)
{
    if(scenePack.scenes.size() == 0)
        return;
    
    SceneInfo& scene = scenePack.scenes[scenePack.selected];
    
    FrustumCulling::Submit(context, scene.frustumCulling, frame);
    
    DepthPyramid::Submit(context, scene.depthPyramid, scene.frustumCulling, frame);
    
    OcclusionCulling::Submit(context, scene.occlusionCulling, scene.depthPyramid, frame);
    
    DepthPrepass::Submit(scene.depthPrepass, scene.occlusionCulling, context, frame);
            
    LightCulling::Submit(context, scene.lightPack.lightCulling, scene.depthPrepass, frame);
    
    NormalPass::Submit(scene.normalPass, scene.lightPack.lightCulling, context, frame);
    
    SSAO::Submit(scene.ssao, scene.normalPass, context, frame);
    
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = scene.inflightFences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {context.imageAvailableSemaphores[frame], scene.ssao.finishedSemaphores[frame]},
                .signalSemaphores = {scene.renderFinishedSemaphores[frame]},
                .commandBuffers = {scene.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}
            }
        }
    });
}

void Scene::SetBackgroundColor(SceneInfo& scene, glm::vec3 color)
{
    scene.backgroundColor = {{color.r, color.g, color.b, 1.0}};
}
