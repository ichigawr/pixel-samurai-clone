#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"


class KeyboardController : public Component {
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;

        bool isFinished = true;
        int lastTick;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
        }

        void update() override {
            if (!sprite->animations["Attack"].isFinished()) {
                sprite->Play("Attack");
            } else {
                isFinished = true;
                sprite->Play("Idle");
            }
            
            if (Game::event.type == SDL_KEYDOWN) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_w:
                        transform->velocity.y = -1;
                        if (isFinished)
                            sprite->Play("Run");
                        break;

                    case SDLK_a:
                        transform->velocity.x = -1;
                        if (isFinished)
                            sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                        break;

                    case SDLK_s:
                        transform->velocity.y = 1;
                        if (isFinished)
                            sprite->Play("Run");
                        break;

                    case SDLK_d:
                        transform->velocity.x = 1;
                        if (isFinished)
                            sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_NONE;
                        break;

                    case SDLK_j:
                        if (isFinished) {
                            sprite->Play("Attack");
                            lastTick = SDL_GetTicks();
                            sprite->animations["Attack"].currentFrame = 0;
                            isFinished = false;
                        }
                        break;

                    default:
                        break;
                }
            }

            if (Game::event.type == SDL_KEYUP) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_w:
                        transform->velocity.y = 0;
                        if (isFinished)
                            sprite->Play("Idle");
                        break;

                    case SDLK_a:
                        transform->velocity.x = 0;
                        if (isFinished)
                            sprite->Play("Idle");
                        break;

                    case SDLK_s:
                        transform->velocity.y = 0;
                        if (isFinished)
                            sprite->Play("Idle");
                        break;

                    case SDLK_d:
                        transform->velocity.x = 0;
                        if (isFinished)
                            sprite->Play("Idle");
                        break;

                    case SDLK_ESCAPE:
                        Game::isRunning = false;
                        break;

                    default:
                        break;
                }
            }
        }
};
