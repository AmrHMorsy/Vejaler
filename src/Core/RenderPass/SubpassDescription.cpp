#include "SubpassDescription.h"



void SubpassDescription::BuildSubpassDescriptions(const std::vector<SubpassDescriptionBuildConfiguration>& configurations, std::vector<VkSubpassDescription>& subpassDescriptions)
{
    size_t n = configurations.size();
    
    subpassDescriptions.resize(n);
    for(size_t i = 0; i < n; i++){
        subpassDescriptions[i] = {};
        subpassDescriptions[i].pipelineBindPoint = configurations[i].pipelineBindPoint;
        subpassDescriptions[i].colorAttachmentCount = static_cast<uint32_t>(configurations[i].colorAttachmentReferences.size());
        subpassDescriptions[i].pColorAttachments = configurations[i].colorAttachmentReferences.data() ;
        subpassDescriptions[i].pDepthStencilAttachment = &configurations[i].depthAttachmentReference;
    }
}
