#include "OptimizationPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"




void Ui::OptimizationPanel::Render(int windowWidth, int windowHeight, SceneInfo& scene)
{
    if(!showOptimizationsPanel)
        return;

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 80), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    ImGui::Begin("Optimizations", &showOptimizationsPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText("Frustum Culling");
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column0BgColor);
        
        if(Utilities::RenderCheckBox("##FrustumCulling", scene.frustumCullingEnable, windowWidth/30.0f))
            scene.frustumCulling.uniformData.parameters = glm::vec4(static_cast<float>(0.0f), static_cast<float>(scene.frustumCullingEnable), 0.0f, 0.0f);
        
        ImGui::EndTable();
    }
    
    ImGui::End();
}
