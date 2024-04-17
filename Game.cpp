#include "Game.hpp"
#include "Enemies.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "AssetManager.hpp"


Map *map = new Map("terrain", 1, 48, 55, 16);
Map *grass = new Map("terrain", 1, 48, 55, 16);

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, map->getScaledSize() * map->getSizeX() - WINDOW_WIDTH, map->getScaledSize() * map->getSizeY() - WINDOW_HEIGHT};
SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

Entity& player(manager.addEntity());
Enemy* enemy = new Enemy(&manager, player);

Game::Game() {    }
Game::~Game() {    }


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        renderer = SDL_CreateRenderer(window, -1, 0);

        isRunning = true;

    } else isRunning = false;

    assets->AddTexture("terrain", "assets/tileset.png");
    assets->AddTexture("background", "assets/background.png");
    assets->AddTexture("player", "assets/player_animations.png");
    assets->AddTexture("enemy", "assets/boss1_animations.png");

    map->LoadMap("assets/map.map");
    grass->LoadMap("assets/grass.map");
        
    std::map<std::string, Animation> playerAnimations = {
        {"Attack 1", Animation(4, 100, 126, 57,  -3, -36)},
        {"Attack 2", Animation(4, 100, 126, 63, -12, -36)},
        {"Block"   , Animation(1, 200,  67, 48,  -6,   0)},
        {"Dash"    , Animation(3, 100,  88, 45,   0, -40)},
        {"Die"     , Animation(3, 200,  63, 51,  -3, -15)},
        {"Idle"    , Animation(3, 200,  48, 48,   0,   0)},
        {"Run"     , Animation(3, 200,  48, 51,   0,   0)},
        {"Take Hit", Animation(2, 150,  48, 45,   0,   0)}
    };

    player.addComponent<TransformComponent>(1, 4);
    player.addComponent<SpriteComponent>("player", true, playerAnimations);
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);

    enemy->init();
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& grasses(manager.getGroup(Game::groupGrasses));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));


void Game::handleEvents() {
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT :
            isRunning = false;
            break;

        case SDL_KEYDOWN :
            if (event.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            break;

        default:
            break;
	}
}


void Game::update() {
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    enemy->update();
    
    manager.refresh();
    manager.update();

    if ((playerPos.x <= 240 || playerPos.x >= 2352) && playerPos.y <= 768) {
        player.getComponent<TransformComponent>().velocity.x = 0;
        player.getComponent<TransformComponent>().velocity.y = 5;
    }

    camera.x = player.getComponent<TransformComponent>().position.x - WINDOW_WIDTH / 2;
    camera.y = player.getComponent<TransformComponent>().position.y - WINDOW_HEIGHT / 2 - 150;

    if (camera.x < 0)
        camera.x = 0;

    if (camera.y < 0)
        camera.y = 0;

    if (camera.x > camera.w)
        camera.x = camera.w;

    if (camera.y > camera.h)
        camera.y = camera.h;
}


void Game::render() {
    SDL_RenderClear(renderer);

    TextureManager::Draw(assets->GetTexture("background"), background, background, SDL_FLIP_NONE);

    for (auto& t : tiles)
        t->draw();

    for (auto& p : players)
        p->draw();

    for (auto& e : enemies)
        e->draw();

    for (auto& g : grasses)
        g->draw();

    SDL_RenderPresent(renderer);
}


void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
