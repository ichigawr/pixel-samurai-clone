#pragma once

#include <vector>

#include "ECS/Components.hpp"
#include "Game.hpp"
#include "AssetManager.hpp"
#include "SoundManager.hpp"


class Menu {
    private:
        Manager manager;

    public:
        int scene = 1;
        bool isFadingIn = true;

        std::vector<Entity*>& labels = manager.getGroup(Game::groupLabels);
        Entity& player = manager.addEntity();
    
        Menu() = default;
        ~Menu() = default;

        void addText(int xpos, int ypos, std::string text, std::string fontFamily);

        void firstScene();
        void secondScene();

        void init() {
            std::map<std::string, Animation> playerAnimations = {
                {"Attack 1"     , Animation(4, 100, 126, 57,  -3, -36)},
                {"Attack 2"     , Animation(4, 100, 126, 63, -12, -36)},
                {"Block"        , Animation(1, 200,  67, 48,  -6,   0)},
                {"Block Success", Animation(1, 200,  67, 48,  -6,   0)},
                {"Dash"         , Animation(3, 100,  88, 45,   0, -40)},
                {"Dead"         , Animation(1, 100,  63, 51,  -3, -15)},
                {"Die"          , Animation(3, 200,  63, 51,  -3, -15)},
                {"Idle"         , Animation(3, 200,  48, 48,   0,   0)},
                {"Run"          , Animation(3, 200,  48, 51,   0,   0)},
                {"Take Hit"     , Animation(2, 200,  48, 45,   0,   0)} 
            };

            player.addComponent<TransformComponent>(0, 70, 2, 0);
            player.addComponent<SpriteComponent>("player", true, playerAnimations);
            player.addComponent<KeyboardController>();

            TransformComponent* transform = &player.getComponent<TransformComponent>();
            transform->position.x = WINDOW_WIDTH / 2 - transform->width * transform->scale / 2 + 30;
            
            addText(-1, -1, "Press Enter to Start", "Terminal 32");
            addText(-1, 50, "Press the keys to preview the character animations", "Terminal 18");
            addText(WINDOW_WIDTH / 2 - 140, 250, "A / D", "Terminal 18");
            addText(WINDOW_WIDTH / 2 - 140, 300, "J / K", "Terminal 18");
            addText(WINDOW_WIDTH / 2 - 140, 350, "Space", "Terminal 18");
            addText(WINDOW_WIDTH / 2 - 140, 400, "ESC", "Terminal 18");
            addText(WINDOW_WIDTH / 2 + 40, 250, "Run left / right", "Terminal 18");
            addText(WINDOW_WIDTH / 2 + 40, 300, "Attack / Block", "Terminal 18");
            addText(WINDOW_WIDTH / 2 + 40, 350, "Dash", "Terminal 18");
            addText(WINDOW_WIDTH / 2 + 40, 400, "Quit game", "Terminal 18");
            addText(-1, 480, "Press Enter to continue", "Terminal 20");
        }

        void update() {
            manager.refresh();
            manager.update();
            
            if (Game::event.type == SDL_KEYDOWN) {
                switch (Game::event.key.keysym.sym) {
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        scene++;
                        Game::overlayAlpha = 0;
                        break;
                    
                    default:
                        break;
                }
            }
        }

        void render() {
            SDL_RenderClear(Game::renderer);

            const SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(Game::renderer, &background);

            switch (scene) {
                case 1:
                    firstScene();
                    break;

                case 2:
                    Game::overlay.y = 470;
                    secondScene();
                    break;

                case 3:
                    Game::overlay.y = 0;
                    secondScene();

                default:
                    break;
            }

            switch (scene) {
                case 3:
                    Game::fadeOut(5);

                    if (Game::overlayAlpha == 255) {
                        scene = 1;

                        for (auto& l : labels)
                            l->destroy();

                        player.destroy();

                        Game::gameStarted = true;
                        SoundManager::Sound()->playMusic("Background Music");
                    }

                    break;

                default:
                    auto newFadeOut = [&](Uint8 speed) {
                        Uint8 newAlpha = Game::overlayAlpha + speed;

                        if (newAlpha > 160)
                            newAlpha = 160;
                        
                        Game::overlayAlpha = newAlpha;
                    };

                    if (isFadingIn) {
                        Game::fadeIn(3);

                        if (Game::overlayAlpha == 0)
                            isFadingIn = false;

                    } else {
                        newFadeOut(3);

                        if (Game::overlayAlpha == 160)
                            isFadingIn = true;
                    }

                    break;
            }

            SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, Game::overlayAlpha);
            SDL_RenderFillRect(Game::renderer, &Game::overlay);

            SDL_RenderPresent(Game::renderer);
        }
};
