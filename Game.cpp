#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"


Map *map = new Map("terrain", 1, 48, 55, 16);
Map *grass = new Map("grass", 1, 48, 55, 16);

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, map->getScaledSize() * map->getSizeX() - WINDOW_WIDTH, map->getScaledSize() * map->getSizeY() - WINDOW_HEIGHT};
SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};


AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());

int fallY = 416;

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
    assets->AddTexture("grass", "assets/grass.png");
    assets->AddTexture("background", "assets/background.png");
    assets->AddTexture("player", "assets/player_animations.png");
    assets->AddTexture("projectile", "assets/proj.png");

    map->LoadMap("assets/map.map");
    grass->LoadMap("assets/grass.map");

    player.addComponent<TransformComponent>(1);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    // assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
    // assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
    // assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
    // assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& grasses(manager.getGroup(Game::groupGrass));
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
        
    float playerPosX = player.getComponent<TransformComponent>().position.x;

    if ((playerPosX <= 208 || playerPosX >= 2320) && fallY <= 768) {
        fallY += 20;
        player.getComponent<TransformComponent>().position.y = fallY;
    }

    for (auto& p : projectiles) {
        SDL_Rect progCol = p->getComponent<ColliderComponent>().collider;
        
        if (Collision::AABB(playerCol, progCol))
            p->destroy();
    }

    camera.x = player.getComponent<TransformComponent>().position.x - WINDOW_WIDTH / 2;
    camera.y = player.getComponent<TransformComponent>().position.y - WINDOW_HEIGHT / 2 - 120;

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

    for (auto& g : grasses)
        g->draw();
    
    // for (auto& p : projectiles)
    //     p->draw();

    SDL_RenderPresent(renderer);
}


void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
