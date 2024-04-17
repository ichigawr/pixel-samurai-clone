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
        std::unordered_map<std::string, Uint32> coolDownStart;
        std::unordered_map<std::string, int> coolDown;
        int currentFrame = 1;
        bool isAnimating = false;
        std::string currentAni;
        std::string currentAttack = "Attack 1";

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
                Animate();
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
                        isAnimating = true;
                        currentAni = currentAttack;
                        lastTick = SDL_GetTicks();
                        transform->velocity.x = 0;
                        break;

                    case SDLK_k:
                        if (SDL_GetTicks() - coolDownStart["Block"] >= coolDown["Block"]) {
                            sprite->Play("Block");
                            isAnimating = true;
                            currentAni = "Block";
                            lastTick = SDL_GetTicks();
                            transform->velocity.x = 0;
                        }

                        break;

                    case SDLK_SPACE:
                        if (SDL_GetTicks() - coolDownStart["Dash"] >= coolDown["Dash"]) {
                            sprite->Play("Dash");
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

        void Animate() {
            Uint32 currentTick = SDL_GetTicks();

            // Uint32 frameDelay = sprite->animations[currentAni].speed - (SDL_GetTicks() - lastTick) % sprite->animations[currentAni].speed;

            // if (frameDelay < 1000 / 60) {
            //     SDL_Delay(frameDelay);
            //     currentFrame++;
            //     lastTick = SDL_GetTicks();
            // }

            if (currentTick - lastTick >= sprite->animations[currentAni].speed) {
                currentFrame++;
                lastTick = currentTick;
            }

            // SDL_Rect lastFrame = sprite->getSrcRect();

            // if (lastFrame.x == sprite->animations[currentAni].frameWidth * (sprite->animations[currentAni].frames - 1)) {

            if (currentFrame > sprite->animations[currentAni].frames) {
                Interrupt();

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

        void Interrupt() {
            currentFrame = 1;
            isAnimating = false;
            coolDownStart[currentAni] = SDL_GetTicks();
            transform->velocity.x = 0;
        }
};
