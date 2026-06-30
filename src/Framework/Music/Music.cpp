#include "Music.h"




void Music::Load(nlohmann::json j, MusicPack& musicPack)
{
    if(!j.contains("music"))
        return;
    
    musicPack.background = j["music"]["background"];
    
    for(size_t i = 0; i < j["music"]["songs"].size(); i++){
        MusicInfo m;
        m.name = j["music"]["songs"][i]["name"].get<std::string>();
        m.volume = j["music"]["songs"][i]["volume"].get<float>();
        for(size_t k = 0; k < j["music"]["songs"][i]["filePaths"].size(); k++)
            m.filePaths.push_back(j["music"]["songs"][i]["filePaths"][k].get<std::string>());
        musicPack.music.push_back(m);
        Audio::Add(m.filePaths, m.name);
        Audio::SetVolume(m.name, m.volume);
    }
}

void Music::PlayLoop(std::string& name)
{
    Audio::Loop(name);
    
    Audio::Rewind(name);
    
    Audio::Play(name);
}

void Music::Play(std::string& name)
{
    Audio::Play(name);
}

void Music::Rewind(std::string& name)
{
    Audio::Rewind(name);
}
