#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"


Map *map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

// camera.w = width_of_map - width_of_window
// camera.h = height_of_map - height_of_window
SDL_Rect Game::camera = {0, 0, 1680, 228};
SDL_Rect background = {0, 0, 960, 540};

AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());

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
    assets->AddTexture("projectile", "assets/proj.png");

    map = new Map("terrain", 1, 48);
    map->LoadMap("assets/map.map", 55, 16);

    player.addComponent<TransformComponent>(1);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& players(manager.getGroup(Game::groupPlayers));
auto& projectiles(manager.getGroup(Game::groupProjectiles));


void Game::handleEvents() {
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}


void Game::update() {
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    
    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        
        if (Collision::AABB(playerCol, cCol))
            player.getComponent<TransformComponent>().position.y = playerPos.y;
    }

    for (auto& p : projectiles) {
        SDL_Rect progCol = p->getComponent<ColliderComponent>().collider;
        
        if (Collision::AABB(playerCol, progCol))
            p->destroy();
    }

    camera.x = player.getComponent<TransformComponent>().position.x - 480;
    camera.y = player.getComponent<TransformComponent>().position.y - 270 - 120;

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

    // for (auto& c : colliders)
    //     c->draw();

    for (auto& p : players)
        p->draw();
    
    // for (auto& p : projectiles)
    //     p->draw();

    SDL_RenderPresent(renderer);
}


void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
