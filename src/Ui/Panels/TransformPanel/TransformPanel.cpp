#include "TransformPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"
#include "../../../Scene/Shadow/Shadow.h"



void Ui::TransformPanel::Render(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, SkyboxPack& skyboxPack)
{
    ModelConfig& config = modelPack.models[modelPack.selected].config;
    
    if(ImGui::CollapsingHeader("Transform")){
        if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            Utilities::SetTableBgColor(column0BgColor);
            
            Utilities::RenderText("Translate");
            Utilities::RenderText("Scale");
            Utilities::RenderText("Rotate X");
            Utilities::RenderText("Rotate Y");
            Utilities::RenderText("Rotate Z");
            
            ImGui::TableSetColumnIndex(1);
            Utilities::SetTableBgColor(column1BgColor);
            
            bool transform = false;
            
            transform |= Utilities::RenderDragFloat3("##translate", config.transformConfig.translate.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f");
            transform |= Utilities::RenderDragFloat3("##scale", config.transformConfig.scale.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f");
            transform |= Utilities::RenderDragFloat("##rotateX", config.transformConfig.rotate.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.0f");
            transform |= Utilities::RenderDragFloat("##rotateY", config.transformConfig.rotate.y, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.0f");
            transform |= Utilities::RenderDragFloat("##rotateZ", config.transformConfig.rotate.z, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.0f");
            
            if(transform){
                modelPack.modelMatrices[modelPack.selected] = glm::translate(glm::mat4(1.0f), config.transformConfig.translate) * glm::translate(glm::mat4(1.0f), modelPack.models[modelPack.selected].boundingBox.center) * glm::rotate(glm::mat4(1.0f), glm::radians(config.transformConfig.rotate.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(config.transformConfig.rotate.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(config.transformConfig.rotate.z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), -modelPack.models[modelPack.selected].boundingBox.center);
                
                for(size_t i = 0; i < lightPack.lights.size(); i++)
                    Shadow::Bake(context, modelPack, lightPack, i);
            }
            
            ImGui::EndTable();
        }
    }
}
