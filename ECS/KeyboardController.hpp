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
        const char* currentAni;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
        }

        void update() override {
            if (isAnimating) {
                Uint32 currentTick = SDL_GetTicks();

                if (currentTick - lastTick >= sprite->animations[currentAni].speed) {
                    currentFrame++;
                    lastTick = currentTick;
                }

                if (currentFrame >= sprite->animations[currentAni].frames) {
                    currentFrame = 0;
                    isAnimating = false;

                    const Uint8* state = SDL_GetKeyboardState(NULL);
                    if (state[SDL_SCANCODE_A]) {
                        sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                        transform->velocity.x = -1;

                    } else if (state[SDL_SCANCODE_D]) {
                        sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_NONE;
                        transform->velocity.x = 1;

                    } else sprite->Play("Idle");
                }
            }
            
            if (!isAnimating && Game::event.type == SDL_KEYDOWN) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_w:
                        transform->velocity.y = -1;
                        sprite->Play("Run");
                        break;

                    case SDLK_a:
                        transform->velocity.x = -1;
                        sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                        break;

                    case SDLK_s:
                        transform->velocity.y = 1;
                        sprite->Play("Run");
                        break;

                    case SDLK_d:
                        transform->velocity.x = 1;
                        sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_NONE;
                        break;

                    case SDLK_j:
                        sprite->Play("Attack");
                        currentFrame++;
                        isAnimating = true;
                        currentAni = "Attack";
                        lastTick = SDL_GetTicks();
                        transform->velocity.x = 0;
                        break;

                    default:
                        break;
                }
            }

            if (!isAnimating && Game::event.type == SDL_KEYUP) {
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

                    default:
                        break;
                }
            }

            if (Game::event.key.keysym.sym == SDLK_ESCAPE)
                Game::isRunning = false;
        }
};
