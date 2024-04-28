#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>


const int FPS = 60;
const int frameDelay = 1000 / FPS;

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

class AssetManager;


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
        
        static bool gameStarted;
        static SDL_Renderer *renderer;
        static SDL_Event event;
        static bool isRunning;
        static SDL_Rect camera;
        static AssetManager *assets;

        static int shakeDuration;
        static int shakeAmount;
        static SDL_Rect overlay;
        static Uint8 overlayAlpha;
        
        static void cameraShake(int duration, int magnitude);
        static void fadeIn(Uint8 speed);
        static void fadeOut(Uint8 speed);
        static void drawHealthBar();

        enum groupLabels : std::size_t {
            groupMap,
            groupGrasses,
            groupColliders,
            groupPlayers,
            groupEnemies,
            groupLabels
        };
};
