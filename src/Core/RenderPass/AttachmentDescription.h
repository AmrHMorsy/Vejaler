#pragma once
#include "../../Includes.h"




struct AttachmentDescriptionBuildConfiguration{
    VkFormat format ;
    VkAttachmentLoadOp loadOp ;
    VkImageLayout finalLayout ;
    VkAttachmentStoreOp storeOp ;
    VkImageLayout initialLayout ;
    VkSampleCountFlagBits samples ;
    VkAttachmentLoadOp stencilLoadOp ;
    VkAttachmentStoreOp stencilStoreOp ;
} ;

class RenderPass ;

class AttachmentDescription{
    
private:
    
    friend class RenderPass ;

    static void BuildAttachmentDescriptions(const std::vector<AttachmentDescriptionBuildConfiguration>& configurations, std::vector<VkAttachmentDescription>& attachmentDescriptions);
} ;
