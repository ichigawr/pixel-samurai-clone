#pragma once

#include "Game.hpp"
#include "TextureManager.hpp"


class GameObject {
    private:
        int xpos;
        int ypos;

        SDL_Texture* objTexture;
        SDL_Rect srcRect, destRect;

    public:
        GameObject(const char* texturesheet, int x, int y);
        ~GameObject();

        void Update();
        void Render();
};
