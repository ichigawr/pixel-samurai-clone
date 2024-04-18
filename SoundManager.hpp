#pragma once

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <unordered_map>


class SoundManager {
    private:
        static SoundManager* soundManager;
        std::unordered_map<std::string, Mix_Chunk*> sfx;
        std::unordered_map<std::string, Mix_Music*> music;

    public:
        enum sound_type {
            SOUND_MUSIC,
            SOUND_SFX
        };

        ~SoundManager() {
            Mix_CloseAudio();
        }

        static SoundManager* Sound();
        bool loadSound(std::string id, std::string filePath, sound_type type);
        void playSound(std::string id);
        void playMusic(std::string id);
};
