#pragma once
#include "../Parser/Parser.h"
#include "../Model/Data/ModelPack.h"
#include "../../Core/Core.h"
#include "../../Framework/Context/Data/ContextInfo.h"
#include "../BoundingVolume/BoundingBox/BoundingBox.h"
#include "../BoundingVolume/BoundingSphere/BoundingSphere.h"




class Loader{
    
private:
    
    static void Adjoin(ModelPack& modelPack, ModelInfo& model, const Mesh& mesh);
    
public:
    
    static void Load(ContextInfo& context, ModelPack& modelPack, const aiScene * assimpScene);
};
