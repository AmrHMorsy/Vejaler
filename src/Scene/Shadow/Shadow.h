#pragma once
#include "Data/ShadowInfo.h"
#include "Data/ShadowConfig.h"
#include "../../Core/Core.h"
#include "../Light/Data/LightInfo.h"
#include "../Light/Data/LightPack.h"
#include "../Model/Data/ModelPack.h"




class Shadow{
    
private:
        
    static void RecordFace(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, LightInfo& light, uint32_t i);
    static void Record(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, uint lightIndex);
    static void Submit(ContextInfo& context, ShadowInfo& shadow);
    
public:
    
    static void Build(ContextInfo& context, ModelPack& modelPack, LightInfo& light);
    static void Load(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack);
    static void Bake(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, uint lightIndex);
};
