#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "ECS.hpp"
#include "../AssetManager.hpp"
#include "../TextureManager.hpp"
#include "../Game.hpp"


class UILabel : public Component {
    private:
        SDL_Rect position;
        std::string text;
        std::string fontFamily;
        SDL_Color color;
        SDL_Texture* texture;

    public:
        UILabel(int xpos, int ypos, std::string text, std::string fontFamily, SDL_Color& color) {
            position.x = 0;
            position.y = 0;

            this->text = text;
            this->fontFamily = fontFamily;
            this->color = color;

            SetLabelText(text, fontFamily);
        }

        ~UILabel() {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        SDL_Rect getPosition() {
            return position;
        }

        void setPosition(int xpos, int ypos) {
            position.x = xpos;
            position.y = ypos;
        }

        void SetLabelText(std::string text, std::string fontFamily) {
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assets->GetFont(fontFamily), text.c_str(), color);
            texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            SDL_FreeSurface(surface);

            SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
        }
        
        void draw() override {
            SDL_RenderCopy(Game::renderer, texture, nullptr, &position);
        }
};
