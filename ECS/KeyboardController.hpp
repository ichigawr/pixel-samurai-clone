#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"


class KeyboardController : public Component {
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;

        Uint32 lastTick;
        int currentFrame = 0;
        bool isAnimating = false;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
        }

        void update() override {
            if (isAnimating) {
                Uint32 currentTick = SDL_GetTicks();

                if (currentTick - lastTick >= sprite->animations["Attack"].speed) {
                    currentFrame++;
                    lastTick = currentTick;
                }

                if (currentFrame >= sprite->animations["Attack"].frames) {
                    currentFrame = 0;
                    isAnimating = false;
                    sprite->Play("Idle");
                }
            }
            
            if (Game::event.type == SDL_KEYDOWN) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_w:
                        if (!isAnimating) {
                            transform->velocity.y = -1;
                            sprite->Play("Run");
                        }
                        break;

                    case SDLK_a:
                        if (!isAnimating) {
                            transform->velocity.x = -1;
                            sprite->Play("Run");
                            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                        }
                        break;

                    case SDLK_s:
                        if (!isAnimating) {
                            transform->velocity.y = 1;
                            sprite->Play("Run");
                        }
                        break;

                    case SDLK_d:
                        if (!isAnimating) {
                            transform->velocity.x = 1;
                            sprite->Play("Run");
                            sprite->spriteFlip = SDL_FLIP_NONE;
                        }
                        break;

                    case SDLK_j:
                        if (!isAnimating) {
                            sprite->Play("Attack");
                            currentFrame++;
                            isAnimating = true;
                            lastTick = SDL_GetTicks();
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
                        sprite->Play("Idle");
                        break;

                    case SDLK_a:
                        transform->velocity.x = 0;
                        sprite->Play("Idle");
                        break;

                    case SDLK_s:
                        transform->velocity.y = 0;
                        sprite->Play("Idle");
                        break;

                    case SDLK_d:
                        transform->velocity.x = 0;
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
