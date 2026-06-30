#pragma once
#include "../../Core/Core.h"
#include "Data/TexturePack.h"
#include "Data/MipMapsGenerateConfiguration.h"
#include "Data/RawTextureBuildConfiguration.h"
#include "../SourceImage/SourceImage.h"



class Texture{
    
private:
    
    static void GenerateMipmaps(MipMapsGenerateConfiguration configuration);
    
public:
    
    static TexturePack BuildRawTexture(RawTextureBuildConfiguration configuration);
    static void BuildTextureWithImage(ContextInfo& context, TexturePack& texture);
    
    
    
    static void BuildWithImage(ContextInfo& context, TexturePack& texture, const TextureConfig& config);
    static void BuildWithImage(ContextInfo& context, TexturePack& texture, SourceImageInfo& sourceImage, const TextureConfig& config);
};
