#include <cmath>

#include "Game.hpp"
#include "Enemies.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "AssetManager.hpp"
#include "SoundManager.hpp"


Map *map = new Map("terrain", 1, 48, 55, 16);
Map *grass = new Map("terrain", 1, 48, 55, 16);

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, map->getScaledSize() * map->getSizeX() - WINDOW_WIDTH, map->getScaledSize() * map->getSizeY() - WINDOW_HEIGHT};

SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

int Game::shakeDuration = 0;
int Game::shakeAmount = 0;
SDL_Rect Game::overlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Uint8 Game::overlayAlpha = 255;

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

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0 && TTF_Init() == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        renderer = SDL_CreateRenderer(window, -1, 0);

        isRunning = true;
    }

    assets->AddTexture("terrain", "assets/maps/tileset.png");
    assets->AddTexture("background", "assets/maps/background.png");
    assets->AddTexture("player", "assets/animations/player_animations.png");
    assets->AddTexture("enemy", "assets/animations/boss1_animations.png");

    assets->AddFont("Terminal 32", "assets/fonts/Terminal.ttf", 32);
    assets->AddFont("Terminal 18", "assets/fonts/Terminal.ttf", 18);

    map->LoadMap("assets/maps/map.map");
    grass->LoadMap("assets/maps/grass.map");
        
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

    player.addComponent<TransformComponent>(1, 4);
    player.addComponent<SpriteComponent>("player", true, playerAnimations);
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);

    enemy->init();

    SoundManager::Sound()->loadSound("Background Music", "assets/sounds/background_music.mp3", SoundManager::SOUND_MUSIC);
    SoundManager::Sound()->playMusic("Background Music");
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& grasses(manager.getGroup(Game::groupGrasses));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));


void Game::cameraShake(int duration, int amount) {
    shakeDuration = duration;
    shakeAmount = amount;
}


void Game::fadeIn(Uint8 speed) {
    Uint8 newAlpha = overlayAlpha - speed;

    if (newAlpha > overlayAlpha) // Overflow (0 - 2 = 255 > 0)
        newAlpha = 0;
    
    overlayAlpha = newAlpha;
}


void Game::fadeOut(Uint8 speed) {
    Uint8 newAlpha = overlayAlpha + speed;

    if (newAlpha < overlayAlpha) // Overflow (255 + 2 = 1 < 255)
        newAlpha = 255;
    
    overlayAlpha = newAlpha;
}


const SDL_Rect PLAYER_HEALTHBAR = {0, 0, 120, 24};
const SDL_Rect ENEMY_HEALTHBAR = {0, 0, 408, 24};
int lastPLayerHealthWidth = PLAYER_HEALTHBAR.w - 12;
int lastEnemyHealthWidth = ENEMY_HEALTHBAR.w - 12;


void Game::drawHealthBar() {
    const int playerMaxHealth = player.getComponent<KeyboardController>().MAX_HEALTH;
    const int enemyMaxHealth = enemy->MAX_HEALTH;
    int playerCurrentHealth = player.getComponent<KeyboardController>().health;
    int enemyCurrentHealth = enemy->enemyHealth;

    // Player health bar
    SDL_Rect playerHealthBar_firstLayer = {44, 24, PLAYER_HEALTHBAR.w, PLAYER_HEALTHBAR.h};
    SDL_Rect playerHealthBar_secondLayer = {50, 30, PLAYER_HEALTHBAR.w - 12, PLAYER_HEALTHBAR.h - 12};
    SDL_Rect playerHealthBar_thirdLayer = {50, 30, lastPLayerHealthWidth, PLAYER_HEALTHBAR.h - 12};
    SDL_Rect playerHealthBar_forthLayer = {50, 30, (PLAYER_HEALTHBAR.w - 12) * playerCurrentHealth / playerMaxHealth, PLAYER_HEALTHBAR.h - 12};

    if (lastPLayerHealthWidth > playerHealthBar_forthLayer.w)
        lastPLayerHealthWidth--;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &playerHealthBar_firstLayer);

    SDL_SetRenderDrawColor(renderer, 122, 90, 77, 255);
    SDL_RenderFillRect(renderer, &playerHealthBar_secondLayer);

    SDL_SetRenderDrawColor(renderer, 187, 187, 187, 255);
    SDL_RenderFillRect(renderer, &playerHealthBar_thirdLayer);

    SDL_SetRenderDrawColor(renderer, 122, 0, 0, 255);
    SDL_RenderFillRect(renderer, &playerHealthBar_forthLayer);

    // Enemy health bar
    SDL_Rect enemyHealthBar_firstLayer = {308, 24, ENEMY_HEALTHBAR.w, ENEMY_HEALTHBAR.h};
    SDL_Rect enemyHealthBar_secondLayer = {314, 30, ENEMY_HEALTHBAR.w - 12, ENEMY_HEALTHBAR.h - 12};
    SDL_Rect enemyHealthBar_thirdLayer = {314, 30, lastEnemyHealthWidth, ENEMY_HEALTHBAR.h - 12};
    SDL_Rect enemyHealthBar_forthLayer = {314, 30, (ENEMY_HEALTHBAR.w - 12) * enemyCurrentHealth / enemyMaxHealth, ENEMY_HEALTHBAR.h - 12};

    if (lastEnemyHealthWidth > enemyHealthBar_forthLayer.w)
        lastEnemyHealthWidth--;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &enemyHealthBar_firstLayer);

    SDL_SetRenderDrawColor(renderer, 122, 90, 77, 255);
    SDL_RenderFillRect(renderer, &enemyHealthBar_secondLayer);

    SDL_SetRenderDrawColor(renderer, 187, 187, 187, 255);
    SDL_RenderFillRect(renderer, &enemyHealthBar_thirdLayer);

    SDL_SetRenderDrawColor(renderer, 122, 0, 0, 255);
    SDL_RenderFillRect(renderer, &enemyHealthBar_forthLayer);
}


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
    
    manager.refresh();
    manager.update();

    enemy->update();

    if ((playerPos.x <= 240 || playerPos.x >= 2352) && playerPos.y <= 768) {
        player.getComponent<TransformComponent>().velocity.x = 0;
        player.getComponent<TransformComponent>().velocity.y = 5;

    } else if (playerPos.y > 768)
        player.getComponent<KeyboardController>().health = -2;

    camera.x = player.getComponent<TransformComponent>().position.x - WINDOW_WIDTH / 2;
    camera.y = player.getComponent<TransformComponent>().position.y - WINDOW_HEIGHT / 2 - 150;

    // Camera shake
    if (shakeDuration > 0) {
        // Calculate a random offset
        int offsetX = (rand() % (shakeAmount * 2)) - shakeAmount;
        int offsetY = (rand() % (shakeAmount * 2)) - shakeAmount;

        // Apply the offset to the camera position
        camera.x += offsetX;
        camera.y += offsetY;

        // Decrease the shake duration
        shakeDuration--;

    } else {
        // Reset the shake variables
        shakeDuration = 0;
        shakeAmount = 0;
    }

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
    
    drawHealthBar();

    if (enemy->playerDead || enemy->enemyDead) {
        fadeOut(2);

        if (overlayAlpha == 255)
            enemy->reset();

    } else fadeIn(5);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, overlayAlpha);
    SDL_RenderFillRect(renderer, &overlay);

    SDL_RenderPresent(renderer);
}


void Game::clean() {
    manager.refresh();

    delete map;
    delete grass;
    delete assets;
    delete enemy;
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}
