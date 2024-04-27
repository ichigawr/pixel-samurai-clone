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
