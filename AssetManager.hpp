#pragma once

#include <map>
#include <string>

#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"


class AssetManager {
    private:
        Manager* manager;
        std::map<std::string, SDL_Texture*> textures;

    public:
        AssetManager(Manager* man);
        ~AssetManager();

        void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);

        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);
};
