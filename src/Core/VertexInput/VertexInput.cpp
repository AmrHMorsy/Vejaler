#include "VertexInput.h"



VertexInputInfo VertexInput::Build(std::string vsFilePath)
{
    VertexInputInfo vertexInput;
        
    BuildVertexAttributeDescription(ReadFile(vsFilePath), vertexInput.vertexAttributeDescription);
    BuildVertexBindingDescription(vertexInput.vertexAttributeDescription, vertexInput.vertexBindingDescription);
    
    return vertexInput;
}

void VertexInput::BuildVertexBindingDescription(std::vector<VkVertexInputAttributeDescription> attributes, std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions)
{
    for(const auto& attr : attributes){
        VkVertexInputBindingDescription desc = {};
        desc.binding = attr.binding;
        desc.stride = GetAttributeSize(attr.format);
        desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        vertexBindingDescriptions.push_back(desc);
    }
}

void VertexInput::BuildVertexAttributeDescription(std::vector<uint32_t> vsFileData, std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions)
{
    spirv_cross::Compiler compiler(vsFileData);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();
    vertexAttributeDescriptions.resize(resources.stage_inputs.size());

    for(int i = 0; i < resources.stage_inputs.size(); i++){
        spirv_cross::SPIRType type = compiler.get_type(resources.stage_inputs[i].type_id);
        vertexAttributeDescriptions[i].location = compiler.get_decoration(resources.stage_inputs[i].id, spv::DecorationLocation);
        vertexAttributeDescriptions[i].binding = vertexAttributeDescriptions[i].location;
        vertexAttributeDescriptions[i].format = GetAttributeFormat(type);
        vertexAttributeDescriptions[i].offset = 0;
    }

    std::sort(vertexAttributeDescriptions.begin(), vertexAttributeDescriptions.end(),
        [](const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b) {
            return a.location < b.location;
        });
}

std::vector<uint32_t> VertexInput::ReadFile(std::string filepath)
{
    std::vector<uint32_t> fileData ;
    std::ifstream file( filepath, std::ios::binary | std::ios::ate ) ;
    if( !file.is_open() )
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO OPEN SHADER FILE FOR READING\033[0m" ) ;
    size_t fileSize = file.tellg() ;
    file.seekg( 0, std::ios::beg ) ;
    fileData.resize( fileSize / sizeof(uint32_t) ) ;
    file.read( reinterpret_cast<char*>( fileData.data()), fileSize ) ;
    
    return fileData ;
}

size_t VertexInput::GetAttributeSize(VkFormat format)
{
    if( format == VK_FORMAT_R32G32B32A32_SFLOAT )
        return 4 * sizeof(float) ;
    else if( format == VK_FORMAT_R32G32B32_SFLOAT )
        return 3 * sizeof(float) ;
    else if( format == VK_FORMAT_R32G32_SFLOAT )
        return 2 * sizeof(float) ;
    else if( format == VK_FORMAT_R32_SFLOAT )
        return sizeof(float) ;
    
    else if(format == VK_FORMAT_R16G16B16A16_SFLOAT)
        return 4 * sizeof(uint16_t);
    else if(format == VK_FORMAT_R16G16B16_SFLOAT)
        return 3 * sizeof(uint16_t);
    else if(format == VK_FORMAT_R16G16_SFLOAT)
        return 2 * sizeof(uint16_t);
    else if(format == VK_FORMAT_R16_SFLOAT)
        return sizeof(uint16_t);
}

VkFormat VertexInput::GetAttributeFormat(spirv_cross::SPIRType type)
{
    if( type.basetype == spirv_cross::SPIRType::Float ){
        if( type.vecsize == 4 )
            return VK_FORMAT_R32G32B32A32_SFLOAT ;
        else if( type.vecsize == 3 )
            return VK_FORMAT_R32G32B32_SFLOAT ;
        else if( type.vecsize == 2 )
            return VK_FORMAT_R32G32_SFLOAT ;
        else if( type.vecsize == 1 )
            return VK_FORMAT_R32_SFLOAT ;
    }
}
