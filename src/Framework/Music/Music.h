#pragma once
#include "Data/MusicPack.h"

#include "../../Includes.h"
#include "../../Core/Core.h"




class Music{
    
public:
    
    static void Load(nlohmann::json j, MusicPack& musicPack);
    static void PlayLoop(std::string& name);
    static void Play(std::string& name);
    static void Rewind(std::string& name);
};
