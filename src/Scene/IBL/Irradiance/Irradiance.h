#pragma once
#include "../../../Includes.h"
#include "../../../Core/Core.h"
#include "../../../Core/Shader/Shader.h"
#include "../../../Core/VertexInput/VertexInput.h"
#include "../../Texture/Data/TexturePack.h"
#include "../../Texture/Texture.h"
#include "Data/IrradianceInfo.h"



class Irradiance{
    
private:
    
    static void RecordFace(ContextInfo& context, IrradianceInfo& irradiance, int faceIndex);
    
public:
        
    static void Build(ContextInfo& context, IrradianceInfo& irradiance, IrradianceConfig config);
    static void Record(ContextInfo& context, IrradianceInfo& irradiance);
    static void Submit(ContextInfo& context, IrradianceInfo& irradiance, VkSemaphore skyboxSemaphore);
};
