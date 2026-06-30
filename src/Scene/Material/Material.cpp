#include "Material.h"




void Material::Load(ContextInfo& context, ModelPack& modelPack, const std::string& baseFilePath, const aiScene * assimpScene)
{
    modelPack.models.resize(assimpScene->mNumMaterials);
    for(uint32_t i = 0; i < assimpScene->mNumMaterials; i++)
        Load(context, modelPack, modelPack.models[i], baseFilePath, assimpScene->mMaterials[i]);
}

void Material::Load(ContextInfo& context, ModelPack& modelPack, ModelInfo& model, const std::string& baseFilePath, const aiMaterial * material)
{
    model.config.materialConfig.albedo.load = false;
    model.config.materialConfig.normal.load = false;
    model.config.materialConfig.roughness.load = false;
    model.config.materialConfig.metallic.load = false;
    model.config.materialConfig.opacity.load = false;
    
    model.material.albedo = modelPack.defaultMaterial.albedo;
    model.material.normal = modelPack.defaultMaterial.normal;
    model.material.metallic = modelPack.defaultMaterial.metallic;
    model.material.roughness = modelPack.defaultMaterial.roughness;
    model.material.opacity = modelPack.defaultMaterial.opacity;
    model.material.roughnessMetallicOpacity = modelPack.defaultMaterial.roughnessMetallicOpacity;
    
    aiString materialName;
    aiReturn result = material->Get(AI_MATKEY_NAME, materialName);
    if(result == AI_SUCCESS)
        model.material.name = materialName.C_Str();
        
    MaterialConfig& config = model.config.materialConfig;
        
    ParseFilePath(material, aiTextureType_DIFFUSE, baseFilePath, config.albedo.filePath, config.albedo.load);
    ParseFilePath(material, aiTextureType_NORMALS, baseFilePath, config.normal.filePath, config.normal.load);
    ParseFilePath(material, aiTextureType_DIFFUSE_ROUGHNESS, baseFilePath, config.roughness.filePath, config.roughness.load);
    ParseFilePath(material, aiTextureType_METALNESS, baseFilePath, config.metallic.filePath, config.metallic.load);
        
    Build(context, model.material, model.config.materialConfig);
}

void Material::Build(ContextInfo& context, MaterialInfo& material, const MaterialConfig& config)
{
    LoadTexture(context, material.albedo, config.albedo);
    LoadTexture(context, material.normal, config.normal);
    LoadTexture(context, material.metallic, config.metallic);
    LoadTexture(context, material.roughness, config.roughness);
    LoadTexture(context, material.opacity, config.opacity);
    MergeTextures(context, material.roughness, config.roughness, material.metallic, config.metallic, material.opacity, config.opacity, material.roughnessMetallicOpacity, config.roughnessMetallicOpacity);
}

void Material::LoadTexture(ContextInfo& context, TexturePack& texture, const TextureConfig& config)
{
    if(!config.load)
        return;
    
    Texture::BuildWithImage(context, texture, config);
    Queue::WaitIdle(context.queuePack.graphicsQueue);
}

void Material::MergeTextures(ContextInfo& context, TexturePack& src1, const TextureConfig& srcConfig1, TexturePack& src2, const TextureConfig& srcConfig2, TexturePack& src3, const TextureConfig& srcConfig3, TexturePack& dst, const TextureConfig& dstConfig)
{
    if(!srcConfig1.load && !srcConfig2.load && !srcConfig3.load)
        return;
    
    dst.config = dstConfig;
    dst.sourceImage = SourceImage::Merge(&src1.sourceImage, &src2.sourceImage, &src3.sourceImage, nullptr, dst.config.channelMap);
    Texture::BuildTextureWithImage(context, dst);
    Queue::WaitIdle(context.queuePack.graphicsQueue);
}

void Material::ParseFilePath(const aiMaterial* mMaterial, aiTextureType type, const std::string& baseFilePath, std::string& filePath, bool& load)
{
    aiString path;
    aiReturn result = mMaterial->GetTexture(type, 0, &path);
    if(result == AI_SUCCESS){
        load = true;
        filePath = path.C_Str();
        filePath = baseFilePath + filePath;
        std::replace(filePath.begin(), filePath.end(), '\\', '/');
    }
    else
        load = false;
}
