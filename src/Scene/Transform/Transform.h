#pragma once
#include "../BVH/BVH.h"
#include "../Shadow/Shadow.h"

#include "Data/TransformInfo.h"
#include "Data/TransformConfig.h"
#include "../Model/Data/ModelPack.h"
#include "../Light/Data/LightPack.h"
#include "../BoundingVolume/BoundingBox/BoundingBox.h"
#include "../BoundingVolume/BoundingSphere/BoundingSphere.h"



class Transform{
    
private:
    
    static void Apply(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex);

public:
    
    static void Reset(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex);
    static void Scale(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex, const glm::vec3& offset);
    static void Translate(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex, const glm::vec3& offset);
    static void Rotate(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex, const glm::vec3& angles);
};
