#include "AttachmentDescription.h"




void AttachmentDescription::BuildAttachmentDescriptions(const std::vector<AttachmentDescriptionBuildConfiguration>& configurations, std::vector<VkAttachmentDescription>& attachmentDescriptions)
{
    attachmentDescriptions.resize(configurations.size());
    for(size_t i = 0; i < configurations.size(); i++){
        attachmentDescriptions[i].format = configurations[i].format ;
        attachmentDescriptions[i].samples = configurations[i].samples;
        attachmentDescriptions[i].loadOp = configurations[i].loadOp ;
        attachmentDescriptions[i].storeOp = configurations[i].storeOp;
        attachmentDescriptions[i].stencilLoadOp = configurations[i].stencilLoadOp;
        attachmentDescriptions[i].stencilStoreOp = configurations[i].stencilStoreOp ;
        attachmentDescriptions[i].initialLayout = configurations[i].initialLayout;
        attachmentDescriptions[i].finalLayout = configurations[i].finalLayout ;
    }
}
