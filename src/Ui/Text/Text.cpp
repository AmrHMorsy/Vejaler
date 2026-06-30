#include "Text.h"
#include "../../Framework/Variable/Data/VariableConstants.h"




void Ui::Text::Load(ScenePack& scenePack, Ui::TextPack& textPack, WindowInfo& window, nlohmann::json j, ImFont * font)
{
    if(!j.contains("texts"))
        return;
    
    for(int i = 0; i < j["texts"].size(); i++){
        TextConfig config;
        
        nlohmann::json t = j["texts"][i];
        
        if(t.contains("size"))
            config.size = t.value("size", config.size);
        
        if(t.contains("display"))
            config.display = t.value("display", config.display);
        
        if(t.contains("blinkSpeed"))
            config.blinkSpeed = t.value("blinkSpeed", config.blinkSpeed);
        
        if(t.contains("duration"))
            config.duration = t.value("duration", config.duration);
        
        if(t.contains("text"))
            config.text = t.value("text", config.text);
        
        if(t.contains("variable")){
            config.variableIndex = t.value("variable", config.variableIndex);
            config.isVariable = true;
        }
        
        if(t.contains("waitTime"))
            config.waitTime = t.value("waitTime", config.waitTime);
        
        if(t.contains("progressive"))
            config.progressive = t.value("progressive", config.progressive);
        
        if(t.contains("blink"))
            config.blink = t.value("blink", config.blink);
        
        if(config.progressive && t.contains("silentWhenProgressing"))
            config.silentWhenProgressing = t.value("silentWhenProgressing", config.silentWhenProgressing);
        
        if(t.contains("soundEffectName"))
            config.soundEffectName = t.value("soundEffectName", config.soundEffectName);
        
        if(t.contains("fillBackground"))
            config.fillBackground = t.value("fillBackground", config.fillBackground);
        
        if(t.contains("upIndex"))
            config.upIndex = t.value("upIndex", config.upIndex);
        
        if(t.contains("downIndex"))
            config.downIndex = t.value("downIndex", config.downIndex);
        
        if(t.contains("bold"))
            config.bold = t.value("bold", config.bold);
        
        if(t.contains("hoverUnconditionally"))
            config.hoverUnconditionally = t.value("hoverUnconditionally", config.hoverUnconditionally);
        
        if(t.contains("position") && t["position"].is_array() && t["position"].size() == 2)
            config.position = ImVec2(t["position"][0].get<float>() * window.windowWidth, t["position"][1].get<float>() * window.windowHeight);
        
        if(t.contains("changeColorWhenHovered"))
            config.changeColorWhenHovered = t.value("changeColorWhenHovered", config.changeColorWhenHovered);
        
        if(t.contains("color") && t["color"].is_array() && t["color"].size() == 4)
            config.color = IM_COL32(t["color"][0].get<uint32_t>(), t["color"][1].get<uint32_t>(), t["color"][2].get<uint32_t>(), t["color"][3].get<uint32_t>());
        
        if(t.contains("backgroundColor") && t["backgroundColor"].is_array() && t["backgroundColor"].size() == 4)
            config.backgroundColor = IM_COL32(t["backgroundColor"][0].get<uint32_t>(), t["backgroundColor"][1].get<uint32_t>(), t["backgroundColor"][2].get<uint32_t>(), t["backgroundColor"][3].get<uint32_t>());
        
        if(t.contains("hoverColor") && t["hoverColor"].is_array() && t["hoverColor"].size() == 4)
            config.hoverColor = IM_COL32(t["hoverColor"][0].get<uint32_t>(), t["hoverColor"][1].get<uint32_t>(), t["hoverColor"][2].get<uint32_t>(), t["hoverColor"][3].get<uint32_t>());
        
        TextInfo text;
        text.config = config;
        text.textSize = font->CalcTextSizeA(text.config.size, FLT_MAX, 0.0f, text.config.text.c_str());
        text.minX = text.config.position.x;
        text.maxX = text.config.position.x + text.textSize.x;
        text.minY = text.config.position.y;
        text.maxY = text.config.position.y + text.textSize.y;
        textPack.texts.push_back(text);
    }
}

bool Ui::Text::IsTextHovered(Ui::TextInfo& text, double xpos, double ypos)
{
    if(text.config.changeColorWhenHovered && (xpos >= text.minX) && (xpos <= text.maxX) && (ypos >= text.minY) && (ypos <= text.maxY))
        return true;
    
    return false;
}

void Ui::Text::Render(ImFont * font, Ui::TextPack& textPack, std::vector<VariableInfo>& variables, double xpos, double ypos)
{
    int hoverIndex = GetHoverIndex(textPack, xpos, ypos);
    if(hoverIndex != -1){
        textPack.selected = hoverIndex;
        textPack.usingMouse = true;
        textPack.usingKeyboard = false;
    }
    else if(!textPack.usingKeyboard)
        textPack.selected = -1;

    for(int i = 0; i < textPack.texts.size(); i++)
        Render(font, textPack, variables, i, xpos, ypos);
}

int Ui::Text::GetHoverIndex(Ui::TextPack& textPack, double xpos, double ypos)
{
    int index = -1;
    for(int i = 0; i < textPack.texts.size(); i++){
        if(!textPack.texts[i].config.display)
            continue;
        
        if(IsTextHovered(textPack.texts[i], xpos, ypos)){
            index = i;
            break;
        }
    }
    
    return index;
}

void Ui::Text::Render(ImFont * font, Ui::TextPack& textPack, std::vector<VariableInfo>& variables, int& textIndex, double xpos, double ypos)
{
    Ui::TextInfo& text = textPack.texts[textIndex];
    
    if(!text.config.display)
        return;
    
    if(text.config.isVariable){
        VariableInfo& variable = variables[text.config.variableIndex];
        if(variable.type == STRING)
            text.config.text = std::get<std::string>(variable.value);
        else if(variable.type == INT)
            text.config.text = std::to_string(std::get<int>(variable.value));
        else if(variable.type == FLOAT){
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << std::get<float>(variable.value);
            text.config.text = ss.str();
        }
    }
    
    if(text.config.fillBackground){
        std::string visibleText = text.config.text.substr(0, text.characterCount);
        ImVec2 textSize = font->CalcTextSizeA(text.config.size, FLT_MAX, 0.0f, visibleText.c_str());
        float padding = 10.0f;
        ImVec2 rectMin = ImVec2(text.config.position.x - padding, text.config.position.y - padding);
        ImVec2 rectMax = ImVec2(text.config.position.x + textSize.x + padding, text.config.position.y + textSize.y + padding);
        ImGui::GetForegroundDrawList()->AddRectFilled(rectMin, rectMax, text.config.backgroundColor);
    }
    
    if(text.config.progressive){
        text.time++;
        if(text.time >= text.config.waitTime){
            text.time = 0;
            if(text.characterCount < text.config.text.size()){
                text.characterCount++;
                if(!text.config.silentWhenProgressing)
                    Music::Play(text.config.soundEffectName);
            }
        }
    }
    else
        text.characterCount = text.config.text.size();
    
    if(text.config.hoverUnconditionally)
        text.config.color = text.config.hoverColor;
    else{
        if(textPack.selected == textIndex)
            text.config.color = text.config.hoverColor;
        else
            text.config.color = text.config.originalColor;
    }
    
    if((text.characterCount == text.config.text.size()) && text.config.blink){
        float time = ImGui::GetTime();
        float opacity = (sin(time * text.config.blinkSpeed) * 0.5f + 0.5f);
        int a = std::max((int)(opacity * 255.0f), 0);
        text.config.color = IM_COL32((text.config.color >> 0) & 0xFF, (text.config.color >> 8) & 0xFF, (text.config.color >> 16) & 0xFF, a);
    }
    
    ImGui::GetForegroundDrawList()->AddText(font, text.config.size, text.config.position, text.config.color, text.config.text.c_str(), text.config.text.c_str() + text.characterCount);
}

void Ui::Text::HoverNext(Ui::TextPack& textPack)
{
    if(textPack.selected == -1)
        textPack.selected = textPack.randomDisplayedTextIndex;
    else if(textPack.texts[textPack.selected].config.downIndex != -1)
        textPack.selected = textPack.texts[textPack.selected].config.downIndex;
    
    textPack.usingMouse = false;
    textPack.usingKeyboard = true;
}

void Ui::Text::HoverPrevious(Ui::TextPack& textPack)
{
    if(textPack.selected == -1)
        textPack.selected = textPack.randomDisplayedTextIndex;
    else if(textPack.texts[textPack.selected].config.upIndex != -1)
        textPack.selected = textPack.texts[textPack.selected].config.upIndex;
    
    textPack.usingMouse = false;
    textPack.usingKeyboard = true;
}

void Ui::Text::Display(Ui::TextPack& textPack, int textIndex)
{
    textPack.texts[textIndex].config.display = true;
    Reset(textPack, textIndex);
    textPack.randomDisplayedTextIndex = textIndex;
}

void Ui::Text::Hide(Ui::TextPack& textPack, int textIndex)
{
    textPack.texts[textIndex].config.display = false;
    Reset(textPack, textIndex);
}

void Ui::Text::Reset(Ui::TextPack& textPack, int textIndex)
{
    Ui::TextInfo& text = textPack.texts[textIndex];
    text.time = 0;
    text.characterCount = 0;
    textPack.selected = -1;
}
