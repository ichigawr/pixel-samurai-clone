#pragma once

#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"


class AssetManager {
    private:
        Manager* manager;
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;

    public:
        AssetManager(Manager* man);
        ~AssetManager();

        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);

        void AddFont(std::string id, std::string path, int fontSize);
        TTF_Font* GetFont(std::string id);
};
