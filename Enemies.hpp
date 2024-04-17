#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "ECS/Animation.hpp"
#include "Game.hpp"
#include "AssetManager.hpp"
#include "TextureManager.hpp"


class Enemy {
    private:
        Manager* manager;
        Entity& enemy;
        Entity& player;

    public:
        Entity* entity;
        TransformComponent* enemyTransform;
        SpriteComponent* enemySprite;
        TransformComponent* playerTransform;
        SpriteComponent* playerSprite;
        KeyboardController* playerController;

        SDL_Rect enemyCol;
        Vector2D enemyPos;
        SDL_Rect playerCol;
        Vector2D playerPos;

        std::map<std::string, Animation> enemyAnimations;

        Uint32 lastTick;
        std::unordered_map<std::string, Uint32> coolDownStart;
        std::unordered_map<std::string, int> coolDown;
        bool isAnimating = false;
        int enemyCurrentFrame = 1;
        int playerCurrentFrame;
        std::string enemyCurrentAni;
        std::string playerCurrentAni;

        int enemyDirection = 1;
        int playerDirection = 1;
        int characterDistance;
        int enemyAttackRange;
        std::string enemyCurrentAttack = "Attack";

        Enemy(Manager* mgr, Entity& plyr);
        ~Enemy() = default;

        void init();
        void update();

        void Animate();
        void Interrupt();
        bool isCollided();

        void moveToPlayer();
        void attackPlayer();
        void enemyIsAttacking();
        void enemyTakeHit();
        // void enemyBlock();
        // void dashAttack();
        // void getBack();
        // void jumpBack();
        // void skill();

        void playerIsAttacking();
        void playerTakeHit();
        void playerBlock();
};
