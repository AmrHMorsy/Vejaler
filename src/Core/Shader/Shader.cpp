#include "Shader.h"




ShaderInfo Shader::Build(VkDevice logicalDevice, ShaderConfig config)
{
    if(config.filePaths.size() != config.stageFlags.size())
        throw std::runtime_error("\033[31mERROR::FAILURE TO CREATE SHADERS::MISSING STAGE FLAGS OR FILE PATHS\033[0m");
    
    uint32_t n = config.filePaths.size();
    
    ShaderInfo shader;
    shader.config = config;
    shader.shaderModules.resize(n);
    shader.pipelineShaderStages.resize(n);
    for(size_t i = 0; i < n; i++){
        shader.shaderModules[i] = CreateShaderModule(logicalDevice, ReadFile(shader.config.filePaths[i]));
        shader.pipelineShaderStages[i] = CreatePipelineShaderStage(shader.shaderModules[i], shader.config.stageFlags[i]);
    }
    
    return shader;
}

VkPipelineShaderStageCreateInfo Shader::CreatePipelineShaderStage(VkShaderModule shaderModule, VkShaderStageFlagBits stageFlag )
{
    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO ;
    info.stage = stageFlag ;
    info.module = shaderModule ;
    info.pName = "main" ;
    return info ;
}

VkShaderModule Shader::CreateShaderModule(VkDevice logicalDevice, std::vector<uint32_t> shaderFileData )
{
    VkShaderModuleCreateInfo createInfo{} ;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO ;
    createInfo.codeSize = shaderFileData.size() * sizeof(uint32_t) ;
    createInfo.pCode = shaderFileData.data() ;

    VkShaderModule shaderModule ;
    if( vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS )
        throw std::runtime_error( "\033[31mFailed To Create Shader Module\033[0m" );
    
    return shaderModule ;
}

std::vector<uint32_t> Shader::ReadFile( std::string filepath )
{
    std::vector<uint32_t> fileData ;
    std::ifstream file( filepath, std::ios::binary | std::ios::ate ) ;
    if( !file.is_open() )
        throw std::runtime_error( "\033[31m\nERROR::FAILURE TO OPEN SHADER FILE FOR READING\033[0m" + filepath ) ;
    size_t fileSize = file.tellg() ;
    file.seekg( 0, std::ios::beg ) ;
    fileData.resize( fileSize / sizeof(uint32_t) ) ;
    file.read( reinterpret_cast<char*>( fileData.data()), fileSize ) ;
    
    return fileData ;
}

void Shader::Destroy(VkDevice logicalDevice, ShaderInfo& shader)
{
    for( size_t j = 0 ; j < shader.shaderModules.size() ; j++ )
        vkDestroyShaderModule(logicalDevice, shader.shaderModules[j], NULL);
}
