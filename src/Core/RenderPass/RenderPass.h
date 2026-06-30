#pragma once
#include "SubpassDependency.h"
#include "SubpassDescription.h"
#include "AttachmentDescription.h"


struct RenderPassBuildStorage{
    std::vector<VkSubpassDependency> subpassDependencies;
    std::vector<VkSubpassDescription> subpassDescriptions;
    std::vector<VkAttachmentDescription> attachmentDescriptions;
};

struct RenderPassBuildConfiguration{
    VkDevice logicalDevice ;
    std::vector<SubpassDependencyBuildConfiguration> subpassDependencyBuildConfigurations;
    std::vector<AttachmentDescriptionBuildConfiguration> attachmentDescriptionBuildConfigurations;
    std::vector<SubpassDescriptionBuildConfiguration> subpassDescriptionBuildConfigurations;
};

struct RenderPassBeginConfiguration{
    VkExtent2D resolution ;
    VkRenderPass renderPass ;
    VkFramebuffer frameBuffer ;
    VkCommandBuffer commandBuffer ;
    std::vector<VkClearValue> clearValues ;
} ;

struct RenderPassDestroyConfiguration{
    VkDevice logicalDevice ;
    VkRenderPass renderPass ;
} ;

class RenderPass{
    
private:
    
    static void FillRenderPassStorage(const RenderPassBuildConfiguration& configuration, RenderPassBuildStorage& storage);
    static void FillRenderPassCreateInfo(const RenderPassBuildStorage& storage, VkRenderPassCreateInfo& info);
    
public:
    
    static VkRenderPass BuildRenderPass(RenderPassBuildConfiguration configuration);
    static void BeginRenderPass(RenderPassBeginConfiguration configuration);
    static void EndRenderPass(VkCommandBuffer commandBuffer);
    static void DestroyRenderPass(RenderPassDestroyConfiguration configuration);
} ;
