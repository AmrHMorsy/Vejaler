#include "ProgressBar.h"




void Ui::ProgressBar::Load(nlohmann::json& j, std::vector<ProgressBarInfo>& progressBars, float windowWidth, float windowHeight)
{
    ProgressBarInfo progressBar;
    
    if(j.contains("position") && j["position"].is_array() && j["position"].size() == 2)
        progressBar.position = ImVec2(j["position"][0].get<float>() * windowWidth, j["position"][1].get<float>() * windowHeight);
    
    if(j.contains("size") && j["size"].is_array() && j["size"].size() == 2)
        progressBar.size = ImVec2(j["size"][0].get<float>() * windowWidth, j["size"][1].get<float>() * windowHeight);
    
    if(j.contains("label"))
        progressBar.label = j["label"];
    
    if(j.contains("variableIndex"))
        progressBar.variableIndex = j["variableIndex"];
    
    progressBars.push_back(progressBar);
}

void Ui::ProgressBar::Render(ProgressBarInfo& progressBar, std::vector<VariableInfo>& variables)
{
    ImGui::SetCursorPos(progressBar.position);
    
    float progress = std::get<float>(variables[progressBar.variableIndex].value);
    
    ImGui::ProgressBar(progress, progressBar.position, progressBar.label.c_str());
}
