#include "Game.hpp"


Game *game = nullptr;


int main(int argc, char *argv[]) {
    Uint32 frameStart;
    int frameTime;
    
    game = new Game();
    
    game->init("Horzion's Fall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    
    while (game->running()) {
        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    game->clean();

    return EXIT_SUCCESS;
}
