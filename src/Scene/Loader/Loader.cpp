#include "Loader.h"





void Loader::Load(ContextInfo& context, ModelPack& modelPack, const aiScene * assimpScene)
{
    for(uint32_t i = 0; i < modelPack.models.size(); i++){
        ModelInfo& model = modelPack.models[i];
        
        Parser::Parse(assimpScene, model.meshes, i);
        
        model.firstInstance = i;
        modelPack.firstInstance = i;
        
        for(size_t j = 0; j < model.meshes.size(); j++)
            Adjoin(modelPack, model, model.meshes[j]);
        
        model.boundingBox = BoundingBox::Build(model.indices, model.vertices);
        model.boundingSphere = BoundingSphere::Build(model.indices, model.vertices);
        
        modelPack.boundingBoxes.push_back(model.boundingBox);
        model.boundingBoxIndex = modelPack.boundingBoxes.size() - 1;
    }
    
    modelPack.indexBuffer = Buffer::BuildBufferWithData(BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = modelPack.primaryCommandPool,
        .data = modelPack.indices.data(),
        .bufferSize = sizeof(uint32_t) * modelPack.indices.size()
    });
    
    modelPack.vertexBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = modelPack.primaryCommandPool,
        .data = modelPack.vertices.data(),
        .bufferSize = sizeof(glm::vec3) * modelPack.vertices.size()
    });
    
    modelPack.normalBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = modelPack.primaryCommandPool,
        .data = modelPack.normals.data(),
        .bufferSize = sizeof(glm::vec3) * modelPack.normals.size()
    });
    
    modelPack.textureCoordinatesBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = modelPack.primaryCommandPool,
        .data = modelPack.textureCoordinates.data(),
        .bufferSize = sizeof(glm::vec2) * modelPack.textureCoordinates.size()
    });
    
    modelPack.drawCommandsBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = modelPack.primaryCommandPool,
        .data = modelPack.drawCommands.data(),
        .bufferSize = sizeof(modelPack.drawCommands[0]) * modelPack.drawCommands.size()
    });
}

void Loader::Adjoin(ModelPack& modelPack, ModelInfo& model, const Mesh& mesh)
{
    for(size_t i = 0; i < mesh.vertices.size(); i++){
        modelPack.max = glm::max(modelPack.max, mesh.vertices[i]);
        modelPack.min = glm::min(modelPack.min, mesh.vertices[i]);
        model.vertices.push_back(mesh.vertices[i]);
    }
    
    for(size_t i = 0; i < mesh.indices.size(); i++)
        model.indices.push_back(mesh.indices[i]);
    
    for(size_t i = 0; i < mesh.vertices.size(); i++)
        modelPack.vertices.push_back(mesh.vertices[i]);
    
    for(size_t i = 0; i < mesh.normals.size(); i++)
        modelPack.normals.push_back(mesh.normals[i]);
    
    for(size_t i = 0; i < mesh.indices.size(); i++)
        modelPack.indices.push_back(mesh.indices[i]);
    
    for(size_t i = 0; i < mesh.textureCoordinates.size(); i++)
        modelPack.textureCoordinates.push_back(mesh.textureCoordinates[i]);
    
    modelPack.boundingSpheresWorldSpace.push_back(BoundingSphere::Build(mesh.indices, mesh.vertices));
    
    modelPack.drawCommands.push_back(VkDrawIndexedIndirectCommand{
        .indexCount = static_cast<uint32_t>(mesh.indices.size()),
        .instanceCount = 1,
        .firstIndex = static_cast<uint32_t>(modelPack.indexOffset),
        .vertexOffset = static_cast<int32_t>(modelPack.vertexOffset),
        .firstInstance = static_cast<uint32_t>(modelPack.firstInstance),
    });
    
    modelPack.vertexOffset += mesh.vertices.size();
    modelPack.indexOffset += mesh.indices.size();
    
    modelPack.totalNumVertices += mesh.vertices.size();
    modelPack.totalNumTriangles += (mesh.indices.size() / 3);
    modelPack.totalNumDrawCommands++;
}
