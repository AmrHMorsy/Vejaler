#pragma once
#include "CameraInfo.h"



struct CameraPack{
    int selected = 0;
    CameraConfig config;
    std::vector<CameraInfo> cameras;
    std::vector<const char *> names;
    bool hideAllCameraModels = false;
};
