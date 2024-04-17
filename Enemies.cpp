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
        {"Take Hit"   , Animation( 3, 100,  76, 97, -15,  -20)},
        {"Tired"      , Animation( 4, 200,  48, 78, -21,    0)}
    };

    enemy.addComponent<TransformComponent>(DEFAULT_X_POSITION - 350, DEFAULT_Y_POSITION, 1, 4.5);
    enemy.addComponent<SpriteComponent>("enemy", true, enemyAnimations);
    enemy.addGroup(Game::groupEnemies);

    entity = &enemy;
    enemyTransform = &entity->getComponent<TransformComponent>();
    enemySprite = &entity->getComponent<SpriteComponent>();

    playerTransform = &player.getComponent<TransformComponent>();
    playerSprite = &player.getComponent<SpriteComponent>();
    playerController = &player.getComponent<KeyboardController>();
}


void Enemy::update() {
    enemyCol = enemySprite->getDestRect();
    enemyPos = enemyTransform->position;

    playerCol = playerSprite->getDestRect();
    playerPos = playerTransform->position;
    playerCurrentFrame = playerController->currentFrame;
    playerCurrentAni = playerController->currentAni;

    if (playerPos.x < enemyPos.x) {
        enemySprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        enemyDirection = -1;
    } else {
        enemySprite->spriteFlip = SDL_FLIP_NONE;
        enemyDirection = 1;
    }

    if (playerSprite->spriteFlip == SDL_FLIP_HORIZONTAL)
        playerDirection = -1;
    else playerDirection = 1;

    if (enemyDirection == playerDirection)
        playerCol.x += 9 * playerDirection;

    if (enemyPos.x <= 270 || enemyPos.x >= 2325) {
        enemyTransform->velocity.x = 0;
        return;
    }

    if (playerPos.y > DEFAULT_Y_POSITION) {
        Interrupt();
        return;
    }

    characterDistance = (playerPos.x - enemyPos.x) * enemyDirection;
    enemyAttackRange = enemyAnimations[enemyCurrentAttack].anchor;

    if (characterDistance > enemyAttackRange + 10) {
        Interrupt();

        if (!isAnimating) {
            moveToPlayer();
            return;
        }
    }

    if (isAnimating) {
        Animate();

        if (enemyCurrentAni == "Attack")
            enemyIsAttacking();
        
        if (playerCurrentAni == "Attack 1" || playerCurrentAni == "Attack 2")
            playerIsAttacking();

        return;
    }

    if (playerCurrentAni == "Attack 1" || playerCurrentAni == "Attack 2")
        playerIsAttacking();

    attackPlayer();
}


void Enemy::Animate() {
    Uint32 currentTick = SDL_GetTicks();

    if (currentTick - lastTick >= enemySprite->animations[enemyCurrentAni].speed) {
        enemyCurrentFrame++;
        lastTick = currentTick;
    }

    if (enemyCurrentFrame > enemySprite->animations[enemyCurrentAni].frames)
        Interrupt();
}


void Enemy::Interrupt() {
    if (enemyCurrentAni == "Attack" && enemyCurrentFrame != 6 && enemyCurrentFrame != 15)
        return;

    enemyCurrentFrame = 1;
    isAnimating = false;
    coolDownStart[enemyCurrentAni] = SDL_GetTicks();
    enemyTransform->velocity.x = 0;

    enemySprite->Play("Idle");
}


bool Enemy::isCollided() {
    if (enemyCol.x + enemyCol.w >= playerCol.x + 15 &&
        playerCol.x + playerCol.w >= enemyCol.x + 15)
            return true;

    return false;
}


void Enemy::moveToPlayer() {
    enemySprite->Play("Run");
    enemyCurrentAni = "Run";
    enemyTransform->velocity.x = 1 * enemyDirection;
}


void Enemy::attackPlayer() {
    enemySprite->Play("Attack");
    enemyCurrentAni = "Attack";
    enemyCurrentAttack = "Attack";
    isAnimating = true;
    enemyTransform->velocity.x = 0;
}


void Enemy::enemyIsAttacking() {    
    if (isCollided() &&
        (enemyCurrentFrame == 3 || enemyCurrentFrame == 4 ||
        enemyCurrentFrame == 10 || enemyCurrentFrame == 11))
            playerTakeHit();

    if (characterDistance > enemyAttackRange - 10 &&
        6 <= enemyCurrentFrame && enemyCurrentFrame <= 8)
            enemyTransform->position.x += 2 * enemyDirection;
}


void Enemy::enemyTakeHit() {
    enemySprite->Play("Take Hit");
    enemyCurrentAni = "Take Hit";
    isAnimating = true;
    enemyTransform->velocity.x = -0.3 * enemyDirection;
}


void Enemy::playerIsAttacking() {
    if (isCollided()) {
        if (playerCurrentAni == "Attack 1") {
            if (playerCurrentFrame == 3)
                enemyTakeHit();
        } else if (playerCurrentFrame == 2)
            enemyTakeHit();
    }
}


void Enemy::playerTakeHit() {
    playerSprite->Play("Take Hit");
    playerController->isAnimating = true;
    playerController->currentAni = "Take Hit";
    playerTransform->velocity.x = 0.6 * enemyDirection;
}
