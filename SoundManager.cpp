#include "SoundManager.hpp"


SoundManager* SoundManager::soundManager = 0;


SoundManager* SoundManager::Sound() {
    if (soundManager == 0)
        soundManager = new SoundManager();

    return soundManager;
}


bool SoundManager::loadSound(std::string id, std::string filePath, sound_type type) {
    if (type == SOUND_MUSIC) {
        Mix_Music* pMusic = Mix_LoadMUS(filePath.c_str());

        if (pMusic == 0)
            return false;

        music[id] = pMusic;
        return true;
    }
    
    if (type == SOUND_SFX) {
        Mix_Chunk* pChunk = Mix_LoadWAV(filePath.c_str());

        if(pChunk == 0)
            return false;

        sfx[id] = pChunk;
        return true;
    }

    return false;
}


void SoundManager::playSound(std::string id) {
    Mix_PlayChannel(-1, sfx[id], 0);
}


void SoundManager::playMusic(std::string id) {
    Mix_PlayMusic(music[id], -1);
}
