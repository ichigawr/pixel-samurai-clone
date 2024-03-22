#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>


class AssetManager;
class ColliderComponent;


class Game {
    private:
        SDL_Window *window;

    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        
        void handleEvents();
        void update();
        void render();
        void clean();

        bool running() {
            return isRunning;
        }
        
        static SDL_Renderer *renderer;
        static SDL_Event event;
        static bool isRunning;
        static SDL_Rect camera;
        static AssetManager *assets;

        enum groupLabels : std::size_t {
            groupMap,
            groupPlayers,
            groupColliders,
            groupProjectiles
        };
};
