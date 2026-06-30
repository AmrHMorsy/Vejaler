#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <cmath>
#include <queue>
#include <filesystem>
#include <unordered_map>
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_METAL_EXT
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <chrono>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <thread>
#include <functional>
#include <algorithm>
#include "../../../../../Resources/external/Include/nlohmann/json.hpp"
#include "../../../../../Resources/external/lib/ImGuiFileDialog/ImGuiFileDialog.h"

#ifdef DEBUG
#define DEBUG_MODE true
#else
#define DEBUG_MODE false
#endif

#define RED    "\033[31m"
#define BLUE   "\033[34m"
#define GREEN  "\033[32m"
#define RESET  "\033[0m"


const int NUM_SAMPLERS = 5;
const int MAX_NUM_LIGHTS = 100;
const int MAX_FRAMES_IN_FLIGHT = 3;
const int maxNumShadowSamples = 100;
