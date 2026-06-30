#include "DebugPanel.h"
#include "ShadingPanel.h"
#include "TexturesPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"
#include "PostProcessingEffectsPanel.h"
#include "../../../Scene/Model/Data/ModelConstants.h"




void Ui::ShadingPanel::Render(ContextInfo& context, SceneInfo& scene, int windowWidth, ApplicationInfo& application)
{
    if(ImGui::CollapsingHeader("Shading")){
        if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            Utilities::SetTableBgColor(column0BgColor);
            
            Utilities::RenderText("Direct Lighting");
            Utilities::RenderText("Indirect Lighting");

            ImGui::TableSetColumnIndex(1);
            Utilities::SetTableBgColor(column1BgColor);
            
            Utilities::RenderCheckBox("##EnableDirectLighting", scene.modelPack.modelSettings[scene.modelPack.selected].shadingFlags, FLAG_DIRECT_LIGHTING, windowWidth/30, PBRDebugFlags);
            
            Utilities::RenderCheckBox("##EnableIndirectLighting", scene.modelPack.modelSettings[scene.modelPack.selected].shadingFlags, FLAG_INDIRECT_LIGHTING, windowWidth/30.0f, PBRDebugFlags);
            ImGui::EndTable();
        }
        
        Ui::PostProcessingEffectsPanel::Render(scene.modelPack, windowWidth/30);
        Ui::TexturesPanel::Render(context, scene, windowWidth/30, application);
        Ui::DebugPanel::Render(scene.modelPack, windowWidth);
    }
}
