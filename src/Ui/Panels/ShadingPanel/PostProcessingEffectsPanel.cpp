#include "PostProcessingEffectsPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"



void Ui::PostProcessingEffectsPanel::Render(ModelPack& modelPack, int panelWidth)
{
    if(ImGui::CollapsingHeader("Post-Processing Effects")){
        if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
            
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            Utilities::SetTableBgColor(column0BgColor);
            Utilities::RenderText("HDR Tonemapping");
            Utilities::RenderText("Gamma Correction");
            
            bool gammaCorrectionEnabled = (modelPack.modelSettings[modelPack.selected].shadingFlags & FLAG_GAMMA_CORRECTION) != 0;

            if(gammaCorrectionEnabled)
                Utilities::RenderText("Gamma Exponent");
            
            ImGui::TableSetColumnIndex(1);
            Utilities::SetTableBgColor(column1BgColor);
            Utilities::RenderCheckBox("##EnableHDRTonemapping", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_HDR_TONEMAPPING, panelWidth, 0);
            Utilities::RenderCheckBox("##EnableGammaCorrection", modelPack.modelSettings[modelPack.selected].shadingFlags, FLAG_GAMMA_CORRECTION, panelWidth, 0);
            
            if(gammaCorrectionEnabled)
                Utilities::RenderSliderFloat("##Exponent", modelPack.modelSettings[modelPack.selected].gammaExponent, 1.0f, 3.0f, "%.1f", ImGui::GetColumnWidth());

            ImGui::EndTable();
        }
    }
}
