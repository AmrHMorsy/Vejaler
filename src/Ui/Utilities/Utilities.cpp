#include "Utilities.h"



void Ui::Utilities::SetTableBgColor(ImVec4 color)
{
    ImU32 bgColor = ImGui::GetColorU32(color);
    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, bgColor);
}

bool Ui::Utilities::RenderDropDownList(char * label, int& selectedIndex, std::vector<const char *> names, float width)
{
    float columnWidth = ImGui::GetColumnWidth();
    float cursorPosition = ImGui::GetCursorPosX();
    cursorPosition += (columnWidth - width) * 0.5f;
    ImGui::SetCursorPosX(cursorPosition);
    ImGui::SetNextItemWidth(width);
    ImGui::AlignTextToFramePadding();
    
    return ImGui::Combo(label, &selectedIndex, names.data(), (int)names.size());
}

bool Ui::Utilities::RenderDropDownList(const char * label, int& selectedIndex, std::vector<const char *> names, float width)
{
    float columnWidth = ImGui::GetColumnWidth();
    float cursorPosition = ImGui::GetCursorPosX();
    cursorPosition += (columnWidth - width) * 0.5f;
    ImGui::SetCursorPosX(cursorPosition);
    ImGui::SetNextItemWidth(width);
    ImGui::AlignTextToFramePadding();
    
    return ImGui::Combo(label, &selectedIndex, names.data(), (int)names.size());
}

void Ui::Utilities::RenderText(const char * text)
{
    float textWidth = ImGui::CalcTextSize(text).x;
    float colWidth = ImGui::GetColumnWidth();
    float center = (colWidth - textWidth) * 0.5f;
    center += ImGui::GetCursorPosX();
    ImGui::SetCursorPosX(center);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(text);
}

bool Ui::Utilities::RenderButton(const char * text)
{
    float textWidth = ImGui::CalcTextSize(text).x;
    float colWidth = ImGui::GetColumnWidth();
    float center = (colWidth - textWidth) * 0.5f;
    center += ImGui::GetCursorPosX();
    ImGui::SetCursorPosX(center);
    ImGui::AlignTextToFramePadding();
    return ImGui::Button(text);
}

void Ui::Utilities::RenderNumber(float number, const char * formatSpecifier)
{
    char text[32];
    snprintf(text, sizeof(text), formatSpecifier, number);
    RenderText(text);
}

void Ui::Utilities::RenderNumber(uint32_t number, const char * formatSpecifier)
{
    char text[32];
    snprintf(text, sizeof(text), formatSpecifier, number);
    RenderText(text);
}

void Ui::Utilities::RenderNumber(int number, const char * formatSpecifier)
{
    char text[32];
    snprintf(text, sizeof(text), formatSpecifier, number);
    RenderText(text);
}

void Ui::Utilities::RenderNumber(glm::vec3 number, const char * formatSpecifier)
{
    char text[32];
    snprintf(text, sizeof(text), formatSpecifier, number.x, number.y, number.z);
    RenderText(text);
}

void Ui::Utilities::RenderNumber(VkExtent2D number, const char * formatSpecifier)
{
    char text[32];
    snprintf(text, sizeof(text), formatSpecifier, number.width, number.height);
    RenderText(text);
}

bool Ui::Utilities::RenderDragFloat(char * label, float& value, float width, float dragSensitivity, float min, float max, const char* format)
{
    ImGui::SetNextItemWidth(width);
    
    return ImGui::DragFloat(label, &value, dragSensitivity, min, max, format);
}

bool Ui::Utilities::RenderDragFloat3(char * label, float& value, float width, float dragSensitivity, float min, float max, const char* format)
{
    ImGui::SetNextItemWidth(width);
    
    return ImGui::DragFloat3(label, &value, dragSensitivity, min, max, format);
}

bool Ui::Utilities::RenderSliderFloat(char * label, float& value, float min, float max, char * format, float width)
{
    ImGui::SetNextItemWidth(width);
    
    return ImGui::SliderFloat(label, &value, min, max, format);
}


bool Ui::Utilities::RenderSliderInt(char * label, int& value, int min, int max, float width)
{
    ImGui::SetNextItemWidth(width);
    
    return ImGui::SliderInt(label, &value, min, max, "%d", 0);
}

void Ui::Utilities::RenderColorEdit(char * label, glm::vec4& color, float width)
{
    float cursorPosition = ImGui::GetCursorPosX();
    cursorPosition += (ImGui::GetColumnWidth() - width) * 0.5f;
    ImGui::SetCursorPosX(cursorPosition);
    ImGui::AlignTextToFramePadding();
    ImGui::ColorEdit3(label, glm::value_ptr(color));
}

bool Ui::Utilities::RenderButton(char * label, float width)
{
    float cursorPosition = ImGui::GetCursorPosX();
    cursorPosition += (ImGui::GetColumnWidth() - width) * 0.5f;
    ImGui::SetCursorPosX(cursorPosition);
    ImGui::AlignTextToFramePadding();
    
    return ImGui::Button(label);
}

void Ui::Utilities::RenderCheckBox(char * label, uint32_t& flag, uint32_t bit, float width, uint32_t disable)
{
    bool enabled = (flag & bit) != 0;    
    if(RenderCheckBox(label, enabled, width)){
        if(enabled){
            flag &= ~disable;
            flag |= bit;
        }
        else
            flag &= ~bit;
    }
}

bool Ui::Utilities::RenderCheckBox(char * label, bool& flag, float width)
{
    float cursorPosition = ImGui::GetCursorPosX();
    cursorPosition += (ImGui::GetColumnWidth() - width) * 0.5f;
    ImGui::SetCursorPosX(cursorPosition);
    ImGui::AlignTextToFramePadding();
    
    return ImGui::Checkbox(label, &flag);
}

bool Ui::Utilities::RenderImageButton(const char * label, ImTextureID textureID, ImVec2 buttonSize)
{
    return ImGui::ImageButton(label, textureID, buttonSize);
}
