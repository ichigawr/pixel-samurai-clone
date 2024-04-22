#include "Enemies.hpp"


Enemy::Enemy(Manager* mgr, Entity& plyr)
            : manager(mgr), player(plyr), enemy(manager->addEntity()) {    }


void Enemy::init() {
    enemyAnimations = {
        {"Attack"     , Animation(17, 100, 147, 78,   0,  -48)},
        {"Block"      , Animation( 6, 200,  93, 54,   0,  -48)},
        {"Dash Attack", Animation(12, 100, 132, 78,   0,   -9)},
        {"Dead"       , Animation( 1, 100,  87, 72,   0,    0)},
        {"Die"        , Animation( 2, 200,  87, 72,   0,    0)},
        {"Idle"       , Animation( 4, 200,  57, 78,   0,    0)},
        {"Jump Back"  , Animation( 7, 100, 108, 88,   0,  -45)},
        {"Recover"    , Animation( 3, 100,  93, 78,   0,  -36)},
        {"Run"        , Animation( 2, 200,  57, 84,   0,    0)},
        {"Skill"      , Animation( 7, 100, 389, 78,   0, -163)},
        {"Take Hit"   , Animation( 3, 100,  76, 97, -15,  -20)},
        {"Tired"      , Animation(12, 200,  48, 78, -21,    0)}
    };

    coolDownReady = {
        {"Block"      , false},
        {"Skill"      , false}
    };

    coolDown = {
        {"Block"      , 6500},
        {"Skill"      , 9000}
    };

    attackRanges = {
        {"Attack"     , enemyAnimations[     "Attack"].anchor     - 35},
        {"Dash Attack", enemyAnimations["Dash Attack"].frameWidth + 35},
        {"Skill"      ,               WINDOW_WIDTH                /  2}
    };

    enemyHealth = MAX_HEALTH;

    enemy.addComponent<TransformComponent>(DEFAULT_X_POSITION + 400, DEFAULT_Y_POSITION, 1, 4);
    enemy.addComponent<SpriteComponent>("enemy", true, enemyAnimations);
    enemy.addGroup(Game::groupEnemies);

    entity = &enemy;
    enemyTransform = &entity->getComponent<TransformComponent>();
    enemySprite = &entity->getComponent<SpriteComponent>();

    playerTransform = &player.getComponent<TransformComponent>();
    playerSprite = &player.getComponent<SpriteComponent>();
    playerController = &player.getComponent<KeyboardController>();

    SoundManager::Sound()->loadSound("Attack Sfx", "assets/attack.wav", SoundManager::SOUND_SFX);
    SoundManager::Sound()->loadSound("Take Hit Sfx", "assets/take_hit.wav", SoundManager::SOUND_SFX);
}


void Enemy::update() {
    enemyCol = enemySprite->getDestRect();
    enemyPos = enemyTransform->position;

    playerCol = playerSprite->getDestRect();
    playerPos = playerTransform->position;
    playerCurrentFrame = playerController->currentFrame;
    playerCurrentAni = playerController->currentAni;

    if (enemyCurrentAni != "Dash Attack" && enemyCurrentAni != "Skill" && enemyCurrentAni != "Tired") {
        if (playerPos.x < enemyPos.x - 20) {
            enemySprite->spriteFlip = SDL_FLIP_HORIZONTAL;
            enemyDirection = -1;

        } else if (playerPos.x > enemyPos.x + 20) {
            enemySprite->spriteFlip = SDL_FLIP_NONE;
            enemyDirection = 1;
        }
    }

    playerDirection = (playerSprite->spriteFlip == SDL_FLIP_NONE) ? 1 : -1;

    if (enemyDirection == playerDirection)
        playerCol.x += 9 * playerDirection;

    if (enemyPos.x < 270)
        enemyTransform->position.x = 270;
    else if (enemyPos.x > 2300)
        enemyTransform->position.x = 2300;

    if (playerPos.y > DEFAULT_Y_POSITION) {
        Interrupt();
        return;
    }

    characterDistance = (playerPos.x - enemyPos.x) * enemyDirection - 48;

    for (auto& x : coolDownReady) 
        x.second = (SDL_GetTicks() - coolDownStart[x.first] >= coolDown[x.first]) ? true : false;

    if (enemyHealth > 18)
        coolDownReady["Skill"] = false;

    if (coolDownReady["Skill"])
        enemyCurrentAttack = "Skill";
    else if (characterDistance > attackRanges["Attack"] + 50)
            enemyCurrentAttack = "Dash Attack";
    else enemyCurrentAttack = "Attack";

    enemyAttackRange = attackRanges[enemyCurrentAttack];

    if (playerCurrentAni == "Attack 1" || playerCurrentAni == "Attack 2")
        playerIsAttacking();

    playerHealth = playerController->health;
    
    if (playerHealth <= 0) {
        if (!playerDead) {
            Interrupt();
            playerDie();
            enemySprite->Play("Idle");
            enemyCurrentAni = "Idle";
            enemyTransform->velocity.x = 0;

        } else if (!playerController->isAnimating)
            playerSprite->Play("Dead");

        return;
    }

    if (isAnimating) {
        Animate();

        if (enemyCurrentAni == "Attack" ||
            enemyCurrentAni == "Dash Attack" ||
            enemyCurrentAni == "Skill")
                enemyIsAttacking();

        return;
    }

    if (enemyHealth <= 0) {
        if (!enemyDead) {
            playerController->Interrupt();
            enemyDie();
            playerSprite->Play("Idle");
            playerController->currentAni = "Idle";
            playerTransform->velocity.x = 0;
        }

        return;
    }

    if (characterDistance > enemyAttackRange) {
        moveToPlayer();
        return;
    }

    if (enemyCurrentAttack == "Skill") {
        if (characterDistance + 144 < enemyAttackRange && !skillReady) {
            skillReady = true;
            jumpBack();

        } else triggerSkill();

    } else if (enemyCurrentAttack == "Dash Attack")
        dashAttack();

    else if (enemyCurrentAttack == "Attack")
        attackPlayer();
}


void Enemy::Animate() {
    if (enemyCurrentAni == "Attack") {
        if (6 <= enemyCurrentFrame && enemyCurrentFrame <= 8)
            enemyTransform->velocity.x = 0.4 * enemyDirection;
        else enemyTransform->velocity.x = 0;

    } else if (enemyCurrentAni == "Dash Attack") {
        if (enemyCurrentFrame == 6 || enemyCurrentFrame == 7)
            enemyTransform->velocity.x = 7.5 * enemyDirection;
        else enemyTransform->velocity.x = 0;

    } else if (enemyCurrentAni == "Jump Back") {
        if (4 <= enemyCurrentFrame && enemyCurrentFrame <= 6)
            enemyTransform->velocity.x = -2 * enemyDirection;
        else enemyTransform->velocity.x = 0;

    } else if (enemyCurrentAni == "Skill") {
        if (enemyCurrentFrame == 7)
            enemyTransform->velocity.x = (characterDistance + 140) / enemyTransform->speed * enemyDirection;
        else enemyTransform->velocity.x = 0;
    }

    Uint32 aniTime = enemyAnimations[enemyCurrentAni].speed * enemyAnimations[enemyCurrentAni].frames;
    Uint32 aniElapsedTime = SDL_GetTicks() - lastTick;
    Uint32 aniFrameDelay = (aniTime > aniElapsedTime) ? aniTime - aniElapsedTime : 0;

    enemyCurrentFrame = aniElapsedTime / enemyAnimations[enemyCurrentAni].speed + 1;

    if (aniFrameDelay <= frameDelay) {
        SDL_Delay(aniFrameDelay);
        Interrupt();
    }
}


void Enemy::Interrupt() {
    isAnimating = false;
    enemyTransform->velocity.x = 0;

    coolDownStart[enemyCurrentAni] = SDL_GetTicks();

    if (enemyCurrentAni == "Block") {
        if (characterDistance < attackRanges["Dash Attack"])
            jumpBack();
        else enemyRecover();
    
    } else if (enemyCurrentAni == "Jump Back")
        enemyRecover();
    
    else if (enemyCurrentAni == "Skill") {
        skillReady = false;
        getTired();
    
    } else if (enemyCurrentAni == "Take Hit")
        enemyHealth -= playerDamage;
    
    else if (enemyCurrentAni == "Die" || enemyCurrentAni == "Dead")
        enemySprite->Play("Dead");

    else if (enemyCurrentAni != "Run") {
        enemySprite->Play("Idle");
        enemyCurrentAni = "Idle";
    }
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
    lastTick = SDL_GetTicks();
}


void Enemy::enemyIsAttacking() {
    bool playerIsBlocking = (playerCurrentAni == "Block" || playerCurrentAni == "Block Success" || playerCurrentAni == "Take Hit") ? true : false;

    auto isAttacking = [&]() -> bool {
        if (enemyCurrentAni == "Attack" &&
            (enemyCurrentFrame == 3 || enemyCurrentFrame == 10))
                return true;

        if (enemyCurrentAni == "Dash Attack" &&
            (enemyCurrentFrame == 6 || enemyCurrentFrame == 7))
                return true;
        
        if (enemyCurrentAni == "Skill" && enemyCurrentFrame == 7)
            return true;
        
        return false;
    };

    if (isCollided() && isAttacking()) {
        if (playerDirection == enemyDirection || !playerIsBlocking)
            playerTakeHit();

        else if (playerCurrentAni == "Block")
            playerBlockSuccess();
    }
    
    if ((playerCurrentAni == "Block" || playerCurrentAni == "Block Success")) {
        if (isCollided() && isAttacking())
                playerTransform->velocity.x = 0.7 * enemyDirection;
        else playerTransform->velocity.x = 0;

    } else if (playerCurrentAni == "Take Hit") {
        if (isCollided() && isAttacking())
                playerTransform->velocity.x = 1 * enemyDirection;
        else playerTransform->velocity.x = 0;
    }
}


void Enemy::enemyTakeHit() {
    enemySprite->Play("Take Hit");
    enemyCurrentAni = "Take Hit";
    isAnimating = true;
    lastTick = SDL_GetTicks();
    
    Game::cameraShake(6, 3);
    SoundManager::Sound()->playSound("Take Hit Sfx");
}


void Enemy::enemyBlock() {
    enemySprite->Play("Block");
    enemyCurrentAni = "Block";
    isAnimating = true;
    enemyTransform->velocity.x = 0;
    lastTick = SDL_GetTicks();

    Game::cameraShake(6, 3);
    SoundManager::Sound()->playSound("Attack Sfx");
}


void Enemy::dashAttack() {
    enemySprite->Play("Dash Attack");
    enemyCurrentAni = "Dash Attack";
    enemyCurrentAttack = "Dash Attack";
    isAnimating = true;
    lastTick = SDL_GetTicks();
}


void Enemy::enemyRecover() {
    enemySprite->Play("Recover");
    enemyCurrentAni = "Recover";
    isAnimating = true;
    lastTick = SDL_GetTicks();
}


void Enemy::jumpBack() {
    enemySprite->Play("Jump Back");
    enemyCurrentAni = "Jump Back";
    isAnimating = true;
    lastTick = SDL_GetTicks();
}


void Enemy::triggerSkill() {
    enemySprite->Play("Skill");
    enemyCurrentAni = "Skill";
    enemyCurrentAttack = "Skill";
    isAnimating = true;
    lastTick = SDL_GetTicks();
}


void Enemy::getTired() {
    enemySprite->Play("Tired");
    enemyCurrentAni = "Tired";
    isAnimating = true;
    enemyTransform->position.x += 87 * enemyDirection;
    lastTick = SDL_GetTicks();
}


void Enemy::enemyDie() {
    enemySprite->Play("Die");
    enemyCurrentAni = "Die";
    isAnimating = true;
    enemyDead = true;
    playerTransform->velocity.x = 0;
    lastTick = SDL_GetTicks();
}


void Enemy::playerIsAttacking() {
    bool enemyIsBlocking = (enemyCurrentAni == "Block" || enemyCurrentAni == "Take Hit" || enemyCurrentAni == "Skill") ? true : false;

    auto isAttacking = [&]() -> bool {
        if (playerCurrentAni == "Attack 1" && playerCurrentFrame == 3)
                return true;

        if (playerCurrentAni == "Attack 2" && playerCurrentFrame == 2)
                return true;
        
        return false;
    };

    if (isCollided() && isAttacking()) {
        srand(time(0));

        if ((rand() % 100 <= 70 && coolDownReady["Block"]) ||
            enemyCurrentAni == "Tired" || enemyCurrentAni == "Recover")
                enemyBlock();

        if (!enemyIsBlocking)
            enemyTakeHit();

        if (enemyCurrentAni == "Block") {
            enemyBlock();
            playerTransform->velocity.x = 0.5 * enemyDirection;

        } else playerTransform->velocity.x = 0;
    }

    if (enemyCurrentAni == "Take Hit") {
        if (isCollided() && isAttacking())
            enemyTransform->velocity.x = -1 * enemyDirection;
        else enemyTransform->velocity.x = 0;
    }
}


void Enemy::playerTakeHit() {
    playerSprite->Play("Take Hit");
    playerController->isAnimating = true;
    playerController->currentAni = "Take Hit";
    playerController->lastTick = SDL_GetTicks();
    
    Game::cameraShake(6, 3);
    SoundManager::Sound()->playSound("Take Hit Sfx");
}


void Enemy::playerBlockSuccess() {
    playerSprite->Play("Block Success");
    playerController->currentAni = "Block Success";
    playerController->isAnimating = true;
    playerController->lastTick = SDL_GetTicks();
    
    Game::cameraShake(6, 3);
    SoundManager::Sound()->playSound("Attack Sfx");
}


void Enemy::playerDie() {
    playerSprite->Play("Die");
    playerController->currentAni = "Die";
    playerController->isAnimating = true;
    playerDead = true;
    playerTransform->velocity.x = 0;
    playerController->lastTick = SDL_GetTicks();
}


void Enemy::reset() {
    enemyTransform->position.x = DEFAULT_X_POSITION + 400;
    enemyTransform->position.y = DEFAULT_Y_POSITION;
    playerTransform->position.x = DEFAULT_X_POSITION;
    playerTransform->position.y = DEFAULT_Y_POSITION;

    enemyHealth = MAX_HEALTH;
    playerController->health = playerController->MAX_HEALTH;
    playerDead = false;
    enemyDead = false;

    enemySprite->Play("Idle");
    playerSprite->Play("Idle");
    enemyCurrentAni = "Idle";
    playerCurrentAni = "Idle";

    skillReady = false;
    enemyCurrentAttack = "";
    coolDownReady["Block"] = false;
    coolDownReady["Skill"] = false;
    coolDownStart["Block"] = 0;
    coolDownStart["Skill"] = 0;

    enemyDirection = 1;
    playerDirection = 1;

    lastTick = 0;
    isAnimating = false;
}
