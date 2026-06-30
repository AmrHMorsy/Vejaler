#include "Audio.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../../../../Resources/external/include/miniaudio/miniaudio.h"


ma_engine Audio::audioEngine = {};
std::unordered_map<std::string, AudioInfo> Audio::map = {};


void Audio::Build()
{
    ma_result result = ma_engine_init(NULL, &audioEngine);
    if(result != MA_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO BUILD AUDIO ENGINE\033[0m");
}

void Audio::Add(std::vector<std::string>& filePaths, std::string& name)
{
    AudioInfo audio;
    
    if(filePaths.empty())
        throw std::runtime_error("\033[31m\nERROR::MUSIC FILEPATHS ARRAY IS EMPTY\033[0m");
    
    int counter = 0;
    
    for(int i = 0; i < SOUNDS_POOL_SIZE; i++){
        audio.pool[i] = new ma_sound();
        ma_result result = ma_sound_init_from_file(&audioEngine, filePaths[counter].c_str(), 0, NULL, NULL, audio.pool[i]);
        if(result != MA_SUCCESS)
            throw std::runtime_error("\033[31m\nERROR::FAILURE TO LOAD SOUND\033[0m");
        counter = (counter >= filePaths.size()-1)? 0: (counter+1);
    }
    
    map[name] = audio;
}

void Audio::Rewind(std::string& name)
{
    AudioInfo& audio = map[name];
    ma_sound_seek_to_pcm_frame(audio.pool[audio.selected], 0);
}

void Audio::Play(std::string& name)
{
    AudioInfo& audio = map[name];
    int& selected = audio.selected;
    ma_sound_start(audio.pool[selected]);
    selected = (selected >= SOUNDS_POOL_SIZE-1)? 0: (selected + 1);
}

void Audio::SetVolume(std::string& name, float volume)
{
    AudioInfo& audio = map[name];
    
    for(int i = 0; i < SOUNDS_POOL_SIZE; i++)
        ma_sound_set_volume(audio.pool[i], volume);
}

void Audio::Loop(std::string& name)
{
    AudioInfo& audio = map[name];
    
    for(int i = 0; i < SOUNDS_POOL_SIZE; i++)
        ma_sound_set_looping(audio.pool[i], MA_TRUE);
}
