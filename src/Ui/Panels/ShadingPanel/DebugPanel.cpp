#include "DebugPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"




void Ui::DebugPanel::Render(ModelPack& modelPack, int windowWidth)
{
    if(ImGui::CollapsingHeader("Debug")){
        if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            Utilities::SetTableBgColor(column0BgColor);
            Utilities::RenderText("Show Albedo");
            Utilities::RenderText("Show AO");
            Utilities::RenderText("Show Normal (Geometry)");
            Utilities::RenderText("Show Normal (Normal Mapping)");
            Utilities::RenderText("Show Roughness");
            Utilities::RenderText("Show Metallic");
            Utilities::RenderText("Show Opacity");
            ImGui::Separator();
            ImGui::Separator();
            Utilities::RenderText("Show Irradiance");
            Utilities::RenderText("Show Prefiltered Environment");
            Utilities::RenderText("Show BRDF Lut");
            ImGui::Separator();
            ImGui::Separator();
            Utilities::RenderText("Show Shadow");
            Utilities::RenderText("Show Light Culling");
            Utilities::RenderText("Show Depth");

            
            float width = windowWidth / 30.0f;
            ImGui::TableSetColumnIndex(1);
            Utilities::SetTableBgColor(column1BgColor);
            Utilities::RenderCheckBox("##ShowAlbedo", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_ALBEDO, width, ~FLAG_SHOW_ALBEDO);
            Utilities::RenderCheckBox("##ShowAO", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_AO, width, ~FLAG_SHOW_AO);
            Utilities::RenderCheckBox("##ShowNormalGeometry", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_NORMAL_GEOMETRY, width, ~FLAG_SHOW_NORMAL_GEOMETRY);
            Utilities::RenderCheckBox("##ShowNormalMapping", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_NORMAL_MAPPING, width, ~FLAG_SHOW_NORMAL_MAPPING);
            Utilities::RenderCheckBox("##ShowRoughness", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_ROUGHNESS, width, ~FLAG_SHOW_ROUGHNESS);
            Utilities::RenderCheckBox("##ShowMetallic", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_METALLIC, width, ~FLAG_SHOW_METALLIC);
            Utilities::RenderCheckBox("##ShowOpacity", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_OPACITY, width, ~FLAG_SHOW_OPACITY);
            
            ImGui::Separator();
            ImGui::Separator();
            
            Utilities::RenderCheckBox("##ShowIrradiance", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_IRRADIANCE, width, ~FLAG_SHOW_IRRADIANCE);
            Utilities::RenderCheckBox("##ShowPfe", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_PREFILTERED_ENV, width, ~FLAG_SHOW_PREFILTERED_ENV);
            Utilities::RenderCheckBox("##ShowBRDFLUT", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_BRDF_LUT, width, ~FLAG_SHOW_BRDF_LUT);
            
            ImGui::Separator();
            ImGui::Separator();
            
            Utilities::RenderCheckBox("##ShowShadow", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_SHADOW, width, ~FLAG_SHOW_SHADOW);
            Utilities::RenderCheckBox("##ShowLightCulling", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_LIGHT_CULLING, width, ~FLAG_SHOW_LIGHT_CULLING);
            Utilities::RenderCheckBox("##ShowDepth", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_SHOW_DEPTH, width, ~FLAG_SHOW_DEPTH);
            
            ImGui::EndTable();
        }
    }
}
