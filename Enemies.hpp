#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "ECS/Animation.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "AssetManager.hpp"


class Enemy {
    private:
        Manager* manager;
        Entity& enemy;
        Entity& player;

    public:
        Entity* entity;
        TransformComponent* transform;
        SpriteComponent* sprite;

        SDL_Rect enemyCol;
        Vector2D enemyPos;
        SDL_Rect playerCol;
        Vector2D playerPos;

        int direction = 1;

        std::map<std::string, Animation> enemyAnimations;
        std::map<std::string, Animation> playerAnimations;

        Uint32 lastTick;
        std::unordered_map<std::string, Uint32> coolDownStart;
        std::unordered_map<std::string, int> coolDown;
        int currentFrame = 0;
        bool isAnimating = false;
        std::string enemyCurrentAni;
        std::string playerCurrentAni;
        
        Enemy() = default;
        Enemy(Manager* mgr, Entity& plyr);
        ~Enemy() = default;

        void init();
        void update();

        void Animating();
        void Interrupt();
        int characterDistance();

        void moveToPlayer();
        void attackPlayer();
        void getHit();
        // void block();
        // void dash();
        // void getBack();
        // void jumpBack();
        // void skill();
};
