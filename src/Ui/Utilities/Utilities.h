#pragma once
#include "../../Includes.h"



namespace Ui{
    class  Utilities {
    public:
        static void RenderCheckBox(char * label, uint32_t& flag, uint32_t bit, float width, uint32_t disable);
        static bool RenderCheckBox(char * label, bool& flag, float width);
        static bool RenderButton(char * label, float width);
        static bool RenderImageButton(const char * label, ImTextureID textureID, ImVec2 buttonSize);
        static bool RenderDragFloat(char * label, float& value, float width, float dragSensitivity, float min, float max, const char* format);
        static bool RenderDragFloat3(char * label, float& value, float width, float dragSensitivity, float min, float max, const char* format);
        static bool RenderSliderFloat(char * label, float& value, float min, float max, char * format, float width);
        static bool RenderSliderInt(char * label, int& value, int min, int max, float width);
        static void RenderColorEdit(char * label, glm::vec4& color, float width);
        static void SetTableBgColor(ImVec4 color);
        static void RenderNumber(float number, const char * formatSpecifier);
        static void RenderNumber(int number, const char * formatSpecifier);
        static void RenderNumber(uint32_t number, const char * formatSpecifier);
        static void RenderNumber(glm::vec3 number, const char * formatSpecifier);
        static void RenderNumber(VkExtent2D number, const char * formatSpecifier);
        static bool RenderDropDownList(char * label, int& selectedIndex, std::vector<const char *> names, float width);
        static bool RenderDropDownList(const char * label, int& selectedIndex, std::vector<const char *> names, float width);
        static void RenderText(const char * text);
        static bool RenderButton(const char * text);
    }; 
}
