#include "AssetManager.hpp"
#include "ECS/Components.hpp"


AssetManager::AssetManager(Manager* man)
    : manager(man) {    }


AssetManager::~AssetManager() {
    manager = nullptr;
    
    for (auto& tex : textures) {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }
}


void AssetManager::AddTexture(std::string id, const char* path) {
    textures.emplace(id, TextureManager::LoadTexture(path));
}


SDL_Texture* AssetManager::GetTexture(std::string id) {
    return textures[id];
}


void AssetManager::AddFont(std::string id, std::string path, int fontSize) {
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}


TTF_Font* AssetManager::GetFont(std::string id) {
    return fonts[id];
}
