#include "Enemies.hpp"


Enemy::Enemy(Manager* mgr, Entity& plyr)
            : manager(mgr), player(plyr), enemy(manager->addEntity()) {    }


void Enemy::init() {
    enemyAnimations = {
        {"Attack"     , Animation(14, 100, 147, 78,   0,  -48)},
        {"Block"      , Animation( 3, 300,  93, 54,   0,  -48)},
        {"Dash Attack", Animation(11, 100, 132, 78,   0,   -9)},
        {"Die"        , Animation( 2, 200,  87, 72,   0,    0)},
        {"Idle"       , Animation( 4, 200,  57, 78,   0,    0)},
        {"Jump Back"  , Animation( 7, 100, 108, 88,   0,  -45)},
        {"Recover"    , Animation( 3, 100,  93, 78,   0,  -36)},
        {"Run"        , Animation( 2, 200,  57, 84,   0,    0)},
        {"SKill"      , Animation( 2, 100, 389, 78,   0, -163)},
        {"Take Hit"   , Animation( 3, 120,  76, 97, -15,  -20)},
        {"Tired"      , Animation( 4, 200,  48, 78, -21,    0)}
    };

    playerAnimations = player.getComponent<SpriteComponent>().animations;

    enemy.addComponent<TransformComponent>(1, 4);
    enemy.addComponent<SpriteComponent>("enemy", true, enemyAnimations);
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(Game::groupEnemies);

    entity = &enemy;
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
}


void Enemy::update() {
    enemyCol = enemy.getComponent<ColliderComponent>().collider;
    enemyPos = enemy.getComponent<TransformComponent>().position;

    playerCol = player.getComponent<ColliderComponent>().collider;
    playerPos = player.getComponent<TransformComponent>().position;

    if (playerPos.x < enemyPos.x) {
        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        direction = -1;
    } else {
        sprite->spriteFlip = SDL_FLIP_NONE;
        direction = 1;
    }

    if (isAnimating) {
        if (characterDistance() > enemyAnimations["Attack"].anchor)
            isAnimating = false;

        Animating();

    } else {
        playerCurrentAni = player.getComponent<SpriteComponent>().currentAni;

        if (characterDistance() <= enemyAnimations["Attack"].anchor)
            attackPlayer();
        else moveToPlayer();

        // block();
        // dash();
        // getBack();
        // jumpBack();
        // skill();
    }
}


void Enemy::Animating() {
    Uint32 currentTick = SDL_GetTicks();

    if (currentTick - lastTick >= sprite->animations[enemyCurrentAni].speed) {
        currentFrame++;
        lastTick = currentTick;
    }

    if (currentFrame > sprite->animations[enemyCurrentAni].frames) {
        currentFrame = 0;
        isAnimating = false;
        coolDownStart[enemyCurrentAni] = currentTick;
        transform->velocity.x = 0;
    }
}


int Enemy::characterDistance() {
    // return ((playerPos.x + playerAnimations[playerCurrentAni].anchor) - (enemyPos.x + enemyAnimations[enemyCurrentAni].anchor)) * direction;
    return (playerPos.x - enemyPos.x) * direction;
}


void Enemy::moveToPlayer() {
    sprite->Play("Run");
    enemyCurrentAni = "Run";
    transform->velocity.x = 1 * direction;
}


void Enemy::attackPlayer() {
    sprite->Play("Attack");
    currentFrame++;
    enemyCurrentAni = "Attack";
    isAnimating = true;
    transform->velocity.x = 0;
}


void Enemy::getHit() {
    sprite->Play("Get Hit");
    currentFrame++;
    enemyCurrentAni = "Get Hit";
    isAnimating = true;
    transform->velocity.x = -1;
}
