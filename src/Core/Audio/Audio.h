#pragma once
#include "Data/AudioInfo.h"
#include "../../Includes.h"
#include "../../../Resources/external/include/miniaudio/miniaudio.h"


class Audio{

private:
    
    static ma_engine audioEngine;
    static std::unordered_map<std::string, AudioInfo> map;
    
public:
    
    static void Build();
    static void Add(std::vector<std::string>& filePaths, std::string& name);
    static void Rewind(std::string& name);
    static void Play(std::string& name);
    static void Loop(std::string& name);
    static void SetVolume(std::string& name, float volume);
};
