#include "../../../includes.h"
#include "../../../Core/Core.h"
#include "../../Texture/Data/TexturePack.h"
#include "Data/PreFilteredEnvironmentInfo.h"
#include "Data/PreFilteredEnvironmentShaderUniform.h"




class PreFilteredEnvironment{
    
private:
    
    static void RecordFace(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment, uint32_t mip, uint32_t i);

public:
    
    static void Build(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment, PreFilteredEnvironmentConfig config);
    static void Record(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment);
    static void Submit(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment, VkSemaphore skyboxSemaphore);
};
