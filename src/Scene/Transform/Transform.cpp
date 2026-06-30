#include "Transform.h"



void Transform::Reset(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex)
{
    ModelInfo& model = modelPack.models[modelIndex];
    
    model.transform.config = TransformConfig{};
    
    model.transform.modelMatrix = glm::mat4(1.0f);
    
    modelPack.modelMatrices[modelIndex] = model.transform.modelMatrix;
    
    Apply(context, modelPack, lightPack, modelIndex);
}

void Transform::Translate(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex, const glm::vec3& offset)
{
    ModelInfo& model = modelPack.models[modelIndex];
    
    model.transform.config.translate += offset;
    
    model.transform.modelMatrix = glm::translate(glm::mat4(1.0f), offset) * model.transform.modelMatrix;
    
    Apply(context, modelPack, lightPack, modelIndex);
}

void Transform::Scale(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex, const glm::vec3& offset)
{
    ModelInfo& model = modelPack.models[modelIndex];
    
    model.transform.config.scale *= offset;
    
    model.transform.modelMatrix = glm::scale(glm::mat4(1.0f), offset) * model.transform.modelMatrix;
    
    Apply(context, modelPack, lightPack, modelIndex);
}

void Transform::Rotate(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex, const glm::vec3& angles)
{
    ModelInfo& model = modelPack.models[modelIndex];
    
    model.transform.config.rotate += angles;
    
    glm::vec3 pivot = model.boundingBox.center;
    
    glm::mat4 toOrigin = glm::translate(glm::mat4(1.0f), -pivot);
    
    glm::mat4 toPivot = glm::translate(glm::mat4(1.0f), pivot);
    
    glm::mat4 rX = glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 rY = glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 rZ = glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    model.transform.modelMatrix = toPivot * rX * rY * rZ * toOrigin * model.transform.modelMatrix;
    
    Apply(context, modelPack, lightPack, modelIndex);
}

void Transform::Apply(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, int modelIndex)
{
    modelPack.modelMatrices[modelIndex] = modelPack.models[modelIndex].transform.modelMatrix;
    
    for(size_t i = 0; i < lightPack.lights.size(); i++)
        Shadow::Bake(context, modelPack, lightPack, i);
}
