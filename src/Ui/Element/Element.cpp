#include "Element.h"
#include "../Utilities/Utilities.h"
#include "../ProgressBar/ProgressBar.h"




void Ui::Element::Load(WindowInfo& window, Ui::ManagerInfo& ui, nlohmann::json& j)
{
    for(size_t k = 0; k < j.size(); k++){
        ElementInfo element;
        if(Load(window, j[k], element))
            ui.elements.push_back(element);
    }
}

bool Ui::Element::Load(WindowInfo& window, nlohmann::json& e, Ui::ElementInfo& element)
{
    if(!e.contains("label"))
        return false;
    
    element.label = e["label"];
    
    if(e.contains("flag"))
        element.flag = e["flag"];
    
    if(e.contains("noCloseOption"))
        element.noCloseOption = e["noCloseOption"];
    
    if(e.contains("windowSize") && e["windowSize"].is_array() && e["windowSize"].size() == 2)
        element.windowSize = ImVec2(e["windowSize"][0].get<float>() * window.windowWidth, e["windowSize"][1].get<float>() * window.windowHeight);

    if(e.contains("windowColors") && e["windowColors"].is_array()){
        for(size_t i = 0; i < e["windowColors"].size(); i++)
            element.windowColors.push_back(ImVec4(e["windowColors"][i][0].get<float>(), e["windowColors"][i][1].get<float>(), e["windowColors"][i][2].get<float>(), e["windowColors"][i][3].get<float>()));
    }
    
    if(element.windowColors.empty())
        element.windowColors.push_back(ImVec4(0.2, 0.2, 0.2, 1.0));
    
    if(e.contains("windowPosition") && e["windowPosition"].is_array() && e["windowPosition"].size() == 2)
        element.windowPosition = ImVec2(e["windowPosition"][0].get<float>() * window.windowWidth, e["windowPosition"][1].get<float>() * window.windowHeight);
        
    if(e.contains("windowPositionPivot") && e["windowPositionPivot"].is_array() && e["windowPositionPivot"].size() == 2)
        element.windowPivot = ImVec2(e["windowPositionPivot"][0].get<float>(), e["windowPositionPivot"][1].get<float>());

    if(e.contains("noMove")){
        if(e["noMove"])
            element.windowFlags |= ImGuiWindowFlags_NoMove;
    }
        
    if(e.contains("noResize")){
        if(e["noResize"])
            element.windowFlags |= ImGuiWindowFlags_NoResize;
    }
    
    if(e.contains("noCollapse")){
        if(e["noCollapse"])
            element.windowFlags |= ImGuiWindowFlags_NoCollapse;
    }
    
    if(e.contains("noTitleBar")){
        if(e["noTitleBar"])
            element.windowFlags |= ImGuiWindowFlags_NoTitleBar;
    }
    
    if(e.contains("columns")){
        for(size_t i = 0; i < e["columns"].size(); i++){
            ColumnInfo column;
            if(Load(window, e["columns"][i], column))
                element.columns.push_back(column);
        }
    }
    
    if(e.contains("progressBar"))
        Ui::ProgressBar::Load(e["progressBar"], element.progressBars, element.windowSize.x, element.windowSize.y);
    
    return true;
}

bool Ui::Element::Load(WindowInfo& window, nlohmann::json& c, ColumnInfo& column)
{
    if(c.contains("color") && c["color"].is_array() && c["color"].size() == 4)
        column.color = ImVec4(c["color"][0].get<float>(), c["color"][1].get<float>(), c["color"][2].get<float>(), c["color"][3].get<float>());
    
    if(!c.contains("data"))
        return false;
    
    for(size_t i = 0; i < c["data"].size(); i++){
        RowInfo row;
        if(Load(window, c["data"][i], row))
            column.rows.push_back(row);
    }
    
    return true;
}

bool Ui::Element::Load(WindowInfo& window, nlohmann::json& r, RowInfo& row)
{
    if(!r.contains("type"))
        return false;
        
    if(r["type"] == "IMAGE_BUTTON"){
        row.type = IMAGE_BUTTON;
        if(!r.contains("buttonLabel"))
            return false;
        
        row.buttonLabel = r["buttonLabel"];
        if(r.contains("buttonSize") && r["buttonSize"].is_array() && r["buttonSize"].size() == 2)
            row.buttonSize = ImVec2(r["buttonSize"][0].get<float>() * window.windowWidth, r["buttonSize"][1].get<float>() * window.windowHeight);
    }
    else if(r["type"] == "TEXT"){
        if(!r.contains("text"))
            return false;
        
        row.text = r["text"];
        row.type = TEXT;
    }
    else
        return false;
        
    return true;
}

void Ui::Element::Render(Ui::ElementInfo& element, ModelPack& modelPack, std::vector<VariableInfo>& variables)
{
    if(!element.flag){
        element.oldFlag = true;
        return;
    }
    
    element.oldFlag = false;
    
    
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, element.windowColors[element.selectedColor]);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, element.windowColors[element.selectedColor]);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, element.windowColors[element.selectedColor]);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, element.windowColors[element.selectedColor]);
    ImGui::SetNextWindowSize(element.windowSize, ImGuiCond_Once);
    ImGui::SetNextWindowPos(element.windowPosition, ImGuiCond_Once, element.windowPivot);
    
    if(element.noCloseOption)
        ImGui::Begin(element.label.c_str(), nullptr, element.windowFlags);
    else
        ImGui::Begin(element.label.c_str(), &element.flag, element.windowFlags);
    
    if(!element.columns.empty()){
        if(ImGui::BeginTable("Table", element.columns.size(), element.tableFlags)){
            
            ImGui::TableNextRow();
            
            for(uint32_t i = 0; i < element.columns.size(); i++){
                ImGui::TableSetColumnIndex(i);
                Ui::Utilities::SetTableBgColor(element.columns[i].color);
                
                for(uint32_t j = 0; j < element.columns[i].rows.size(); j++){
                    if(element.columns[i].rows[j].type == IMAGE_BUTTON)
                        Ui::Utilities::RenderImageButton(element.columns[i].rows[j].buttonLabel.c_str(), modelPack.defaultMaterial.metallic.uiTextureID, element.columns[i].rows[j].buttonSize);
                    else if(element.columns[i].rows[j].type == TEXT)
                        Ui::Utilities::RenderText(element.columns[i].rows[j].text.c_str());
                }
            }
            
            ImGui::EndTable();
        }
    }
    
    for(size_t i = 0; i < element.progressBars.size(); i++)
        Ui::ProgressBar::Render(element.progressBars[i], variables);
    
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}
