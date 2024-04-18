#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "ECS/Animation.hpp"
#include "Game.hpp"


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
        bool isAnimating = false;
        int enemyCurrentFrame = 1;
        int playerCurrentFrame;
        std::string enemyCurrentAni;
        std::string playerCurrentAni;

        int enemyDirection = 1;
        int playerDirection = 1;
        int characterDistance;

        int MAX_HEALTH = 30;
        int enemyHealth = MAX_HEALTH;
        int playerHealth = 10;
        int playerDamage = 2;

        bool enemyDead = false;
        bool playerDead = false;

        bool skillReady = false;
        int enemyAttackRange;
        std::string enemyCurrentAttack;
        std::unordered_map<std::string, int> attackRanges;

        std::unordered_map<std::string, Uint32> coolDownStart;
        std::unordered_map<std::string, bool> coolDownReady;
        std::unordered_map<std::string, int> coolDown;

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
        void enemyBlock();
        void dashAttack();
        void enemyRecover();
        void jumpBack();
        void triggerSkill();
        void getTired();
        void enemyDie();

        void playerIsAttacking();
        void playerTakeHit();
        void playerBlockSuccess();
        void playerDie();
};
