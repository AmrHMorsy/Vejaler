#pragma once
#define ICON_FA_CAMERA "\xef\x80\xb0"
#define ICON_FA_LIGHTBULB "\xef\x83\xab"
#define ICON_FA_CHART_BAR "\xef\x82\x80"
#define ICON_FA_CUBE "\xef\x86\xb2"
#define ICON_FA_STAR "\xef\x80\x85"
#define ICON_FA_GLOBE "\xef\x82\xac"
#define ICON_FA_CLOUD "\xef\x83\x82"
#define ICON_FA_EDIT         "\xef\x84\x84"
#define ICON_FA_PENCIL_ALT "\xef\x8c\x83"
#define ICON_FA_PEN "\xef\x8c\x84"
#define ICON_FA_TACHOMETER_ALT "\xef\x8f\xbd"
#define ICON_FA_SLIDERS_H      "\xef\x87\x9e"
#define ICON_FA_COGS           "\xef\x83\x85"
#define ICON_FA_CHART_LINE     "\xef\x88\x81"
#define ICON_FA_OBJECT_GROUP "\xef\x89\x87"
#define ICON_FA_LAYER_GROUP    "\xef\x97\xbd"

inline bool showScenePanel = false;
inline bool showLightPanel = false;
inline bool showModelPanel = false;
inline bool isProjectSaved = false;
inline bool showCameraPanel = false;
inline bool showSkyboxPanel = false;
inline bool showAddLightWindow = false;
inline bool showUserGuidePanel = false;
inline bool showAddCameraWindow = false;
inline bool showEditingModePanel = false;
inline bool showUserManualPanel = true;
inline bool showPerformancePanel = false;
inline bool showOptimizationsPanel = false;

const ImVec4 column0BgColor(0.2f, 0.4f, 1.0f, 0.2f);
const ImVec4 column1BgColor(0.2f, 0.2f, 0.2f, 0.2f);

enum PBRShadingFlagBits {
    FLAG_DIRECT_LIGHTING             = 1u << 0,
    FLAG_INDIRECT_LIGHTING           = 1u << 1,
    FLAG_HDR_TONEMAPPING             = 1u << 2,
    FLAG_GAMMA_CORRECTION            = 1u << 3,
    FLAG_SHOW_ALBEDO                 = 1u << 4,
    FLAG_SHOW_AO                     = 1u << 5,
    FLAG_SHOW_NORMAL_GEOMETRY        = 1u << 6,
    FLAG_SHOW_NORMAL_MAPPING         = 1u << 7,
    FLAG_SHOW_ROUGHNESS              = 1u << 8,
    FLAG_SHOW_METALLIC               = 1u << 9,
    FLAG_SHOW_IRRADIANCE             = 1u << 10,
    FLAG_SHOW_PREFILTERED_ENV        = 1u << 11,
    FLAG_SHOW_BRDF_LUT               = 1u << 12,
    FLAG_SHOW_SHADOW                 = 1u << 13,
    FLAG_SHOW_LIGHT_CULLING          = 1u << 14,
    FLAG_SHOW_OPACITY                = 1u << 15,
    FLAG_SHOW_DEPTH                  = 1u << 16
}; 

const uint32_t PBRDebugFlags =
    FLAG_SHOW_ALBEDO |
    FLAG_SHOW_AO |
    FLAG_SHOW_NORMAL_GEOMETRY |
    FLAG_SHOW_NORMAL_MAPPING |
    FLAG_SHOW_ROUGHNESS |
    FLAG_SHOW_METALLIC |
    FLAG_SHOW_IRRADIANCE |
    FLAG_SHOW_PREFILTERED_ENV |
    FLAG_SHOW_BRDF_LUT |
    FLAG_SHOW_SHADOW |
    FLAG_SHOW_LIGHT_CULLING |
    FLAG_SHOW_OPACITY |
    FLAG_SHOW_DEPTH;
