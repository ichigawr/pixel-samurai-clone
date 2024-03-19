#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"


class KeyboardController : public Component {
    public:
        TransformComponent *transform;
        // SpriteComponent *sprite;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            // sprite = &entity->getComponent<SpriteComponent>();
        }

        void update() override {
            if (Game::event.type == SDL_KEYDOWN) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_w:
                        transform->velocity.y = -1;
                        // sprite->Play("UpAnimation");
                        break;
                    case SDLK_a:
                        transform->velocity.x = -1;
                        // sprite->Play("LeftAnimation");
                        break;
                    case SDLK_s:
                        transform->velocity.y = 1;
                        // sprite->Play("DownAnimation");
                        break;
                    case SDLK_d:
                        transform->velocity.x = 1;
                        // sprite->Play("RightAnimation");
                        break;
                    default:
                        break;
                }
            }

            if (Game::event.type == SDL_KEYUP) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_w:
                        transform->velocity.y = 0;
                        break;
                    case SDLK_a:
                        transform->velocity.x = 0;
                        break;
                    case SDLK_s:
                        transform->velocity.y = 0;
                        break;
                    case SDLK_d:
                        transform->velocity.x = 0;
                        break;
                    default:
                        break;
                }
            }
        }
};