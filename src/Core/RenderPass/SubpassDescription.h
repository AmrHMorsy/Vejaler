#pragma once
#include "../../Includes.h"



struct SubpassDescriptionBuildConfiguration{
    VkPipelineBindPoint pipelineBindPoint;
    VkAttachmentReference depthAttachmentReference;
    std::vector<VkAttachmentReference> colorAttachmentReferences;
} ;

class RenderPass ;

class SubpassDescription{
    
private:
    
    friend class RenderPass ;
    
    static void BuildSubpassDescriptions(const std::vector<SubpassDescriptionBuildConfiguration>& configurations, std::vector<VkSubpassDescription>& subpassDescriptions);
};
