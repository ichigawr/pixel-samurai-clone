#pragma once

#include <unordered_map>

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"


class KeyboardController : public Component {
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;

        Uint32 lastTick;
        std::unordered_map<const char*, Uint32> coolDownStart;
        std::unordered_map<const char*, int> coolDown;
        int currentFrame = 0;
        bool isAnimating = false;
        const char* currentAni;
        const char* currentAttack = "Attack 1";

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();

            coolDown = {
                {"Block", 300},
                {"Dash", 500}
            };
        }

        void update() override {
            if (isAnimating) {
                Animating();
                return;
            }

            if (Game::event.type == SDL_KEYDOWN) {
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
                        sprite->Play(currentAttack);
                        currentFrame++;
                        isAnimating = true;
                        currentAni = currentAttack;
                        lastTick = SDL_GetTicks();
                        transform->velocity.x = 0;
                        break;

                    case SDLK_k:
                        if (SDL_GetTicks() - coolDownStart["Block"] >= coolDown["Block"]) {
                            sprite->Play("Block");
                            currentFrame++;
                            isAnimating = true;
                            currentAni = "Block";
                            lastTick = SDL_GetTicks();
                            transform->velocity.x = 0;
                        }

                        break;

                    case SDLK_SPACE:
                        if (SDL_GetTicks() - coolDownStart["Dash"] >= coolDown["Dash"]) {
                            sprite->Play("Dash");
                            currentFrame++;
                            isAnimating = true;
                            currentAni = "Dash";
                            lastTick = SDL_GetTicks();

                            if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
                                transform->velocity.x = -2;
                            else transform->velocity.x = 2;
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

                    default:
                        break;
                }
            }
        }

        void Animating() {
            Uint32 currentTick = SDL_GetTicks();

            if (currentTick - lastTick >= sprite->animations[currentAni].speed) {
                currentFrame++;
                lastTick = currentTick;
            }

            if (currentFrame > sprite->animations[currentAni].frames) {
                currentFrame = 0;
                isAnimating = false;
                coolDownStart[currentAni] = currentTick;
                transform->velocity.x = 0;
                currentAttack = (currentAttack == "Attack 1") ? "Attack 2" : "Attack 1";

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
};
