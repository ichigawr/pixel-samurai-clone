#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "Components.hpp"
#include "../TextureManager.hpp"


class ColliderComponent : public Component {
    public:
        SDL_Rect collider;
        std::string tag;

        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

        TransformComponent *transform;

        ColliderComponent(std::string t) {
            tag = t;
        }

        ColliderComponent(std::string t, int xpos, int ypos, int size) {
            tag = t;
            collider.x = xpos;
            collider.y = ypos;
            collider.w = collider.h = size;
        }

        void init() override {
            if (!entity->hasComponent<TransformComponent>()) {
                entity->addComponent<TransformComponent>();
            }

            transform = &entity->getComponent<TransformComponent>();

            srcRect = {0, 0, 32, 32};
            destRect = {collider.x, collider.y, collider.w, collider.h};
        }

        void update() override {
            if (tag != "terrain") {
                collider.x = static_cast<int>(transform->position.x);
                collider.y = static_cast<int>(transform->position.y);
                collider.w = transform->width * transform->scale;
                collider.h = transform->height * transform->scale;
            }

            destRect.x = collider.x - Game::camera.x;
            destRect.y = collider.y - Game::camera.y;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
        }
};
