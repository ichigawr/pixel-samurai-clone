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

        int health = 10;
        int enemyDamage = 2;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();

            coolDown = {
                {"Block", 300},
                {"Dash" , 500}
            };
        }

        void update() override {
            if (isAnimating) {
                Animate();
                return;
            }

            if (health > 0) {
                const Uint8* state = SDL_GetKeyboardState(NULL);
                if (state[SDL_SCANCODE_A]) {
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    transform->velocity.x = -1;

                } else if (state[SDL_SCANCODE_D]) {
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    transform->velocity.x = 1;
                }
            }

            if (health > 0 && Game::event.type == SDL_KEYDOWN) {
                int direction = (sprite->spriteFlip == SDL_FLIP_NONE) ? 1 : -1;

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
                        transform->velocity.x = 0;
                        lastTick = SDL_GetTicks();
                        break;

                    case SDLK_k:
                        if (SDL_GetTicks() - coolDownStart["Block"] >= coolDown["Block"]) {
                            sprite->Play("Block");
                            isAnimating = true;
                            currentAni = "Block";
                            transform->velocity.x = 0;
                            lastTick = SDL_GetTicks();
                        }

                        break;

                    case SDLK_SPACE:
                        if (SDL_GetTicks() - coolDownStart["Dash"] >= coolDown["Dash"]) {
                            sprite->Play("Dash");
                            isAnimating = true;
                            currentAni = "Dash";
                            transform->velocity.x = 2 * direction;
                            lastTick = SDL_GetTicks();
                        }

                        break;

                    default:
                        break;
                }
            }

            if (health > 0 && Game::event.type == SDL_KEYUP) {
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
            Uint32 aniTime = sprite->animations[currentAni].speed * sprite->animations[currentAni].frames;
            Uint32 aniElapsedTime = SDL_GetTicks() - lastTick;
            Uint32 aniFrameDelay = (aniTime > aniElapsedTime) ? aniTime - aniElapsedTime : 0;

            currentFrame = aniElapsedTime / sprite->animations[currentAni].speed + 1;

            if (aniFrameDelay <= frameDelay) {
                currentAttack = (currentAttack == "Attack 1") ? "Attack 2" : "Attack 1";

                SDL_Delay(aniFrameDelay);

                Interrupt();
            }
        }

        void Interrupt() {
            if (currentAni == "Take Hit")
                health -= enemyDamage;
            
            isAnimating = false;
            coolDownStart[currentAni] = SDL_GetTicks();
            sprite->Play("Idle");
            currentAni = "Idle";
            transform->velocity.x = 0;
        }
};
