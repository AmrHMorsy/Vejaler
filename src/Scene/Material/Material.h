#pragma once
#include "../Parser/Parser.h"
#include "../Texture/Texture.h"
#include "../../Core/Core.h"
#include "../Model/Data/ModelPack.h"
#include "../../Framework/Context/Data/ContextInfo.h"
#include "../BoundingVolume/BoundingBox/BoundingBox.h"
#include "../BoundingVolume/BoundingSphere/BoundingSphere.h"




class Material{
    
private:
    
    static void LoadTexture(ContextInfo& context, TexturePack& texture, const TextureConfig& config);
    
    static void MergeTextures(ContextInfo& context, TexturePack& src1, const TextureConfig& srcConfig1, TexturePack& src2, const TextureConfig& srcConfig2, TexturePack& src3, const TextureConfig& srcConfig3, TexturePack& dst, const TextureConfig& dstConfig);
    
    static void ParseFilePath(const aiMaterial* mMaterial, aiTextureType type, const std::string& baseFilePath, std::string& filePath, bool& load);

    static void Load(ContextInfo& context, ModelPack& modelPack, ModelInfo& model, const std::string& baseFilePath, const aiMaterial * mMaterial);
    
public:
    
    static void Build(ContextInfo& context, MaterialInfo& material, const MaterialConfig& config);
    
    static void Load(ContextInfo& context, ModelPack& modelPack, const std::string& baseFilePath, const aiScene * assimpScene);
};
