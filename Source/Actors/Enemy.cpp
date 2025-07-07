#include "Enemy.h"
#include "Unit.h"
#include "../Game.h"
#include "../Audio/AudioSystem.h"
#include "../Effects/ParticleSystem.h"
#include "../Utils/Random.h"

Enemy::Enemy(Game *game, const std::string &unitType, Stats stats)
    : Unit(game, stats, true, unitType) {
    // Draws the colored square where the enemy is
    mTypeIndicator = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mTypeIndicator->SetColor(Vector3(255, 0, 0));
    mTypeIndicator->SetAlpha(50);

    // Life bar
    mRedBar = new DrawPolygonComponent(this, Vector2(3, Game::TILE_SIZE), Vector2(Game::TILE_SIZE - 6, 2));
    mRedBar->SetColor(Vector3(255, 0, 0));

    mGreenBar = new DrawPolygonComponent(this, Vector2(3, Game::TILE_SIZE), Vector2(Game::TILE_SIZE - 6, 2), 150);
    mGreenBar->SetColor(Vector3(0, 255, 0));
}


Enemy::~Enemy() {
    mGame->RemoveEnemy(this);
}

void Enemy::OnUpdate(float deltaTime) {
    // Calls Unit update
    Unit::OnUpdate(deltaTime);

    // Updates type indicator
    if (mEnemyState != EnemyState::None)
        mTypeIndicator->SetAlpha(150);
    else
        mTypeIndicator->SetAlpha(50);

    // Updates life indicator
    mGreenBar->SetSize(Vector2((float(mStats.currHp) / float(mStats.hp)) * (Game::TILE_SIZE - 6), 2));

    // Returns if not enemy turn
    if (mGame->GetGamePlayState() != Game::GamePlayState::EnemyTurn) {
        return;
    }

    // State machine to control actions
    if (mEnemyState == EnemyState::Moving) {
        if (mMovementTimer <= 0) {
            // Self position
            int selfX = GetX();
            int selfY = GetY();

            // Gets the closest unit and its position
            Vector2 closestPos = Vector2::Zero;
            auto units = mGame->GetUnits();
            for (auto unit: units) {
                int unitX = unit->GetX();
                int unitY = unit->GetY();

                if ((Math::Abs(unitX - selfX) + Math::Abs(unitY - selfY)) <
                    (Math::Abs(closestPos.x - selfX) + Math::Abs(closestPos.y - selfY))) {
                    mClosestUnit = unit;
                    closestPos.x = unitX;
                    closestPos.y = unitY;
                }
            }

            // Calculates distance to the closest unit
            int distanceX = Math::Abs(closestPos.x - selfX);
            int distanceY = Math::Abs(closestPos.y - selfY);
            int directionX = distanceX != 0 ? (closestPos.x - selfX) / Math::Abs(closestPos.x - selfX) : 1;
            int directionY = distanceY != 0 ? (closestPos.y - selfY) / Math::Abs(closestPos.y - selfY) : 1;

            // If low hp, moves away from units
            bool lowHP = false;
            if (static_cast<float>(mStats.currHp) < 0.25f * static_cast<float>(mStats.hp)) {
                directionX *= -1;
                directionY *= -1;
                lowHP = true;
            }

            // Moves the enemy towards the closest unit if not in attack range of weapon
            if ((distanceX + distanceY) > GetEquippedWeapon()->range || lowHP) {
                // Auxiliary declarations/calls
                int targetX = selfX;
                int targetY = selfY;
                SetMovementRange();

                // Different enemies have different patterns
                if (mUnitType == "Orc") {
                    // Moves the most it can in X and Y directions
                    while (true) {
                        // Sanity checks
                        if (targetX + directionX >= Game::LEVEL_HEIGHT)
                            break;
                        if (!MovementIsInRange(targetX + directionX, targetY))
                            break;

                        targetX += directionX;
                    }
                    while (true) {
                        // Sanity checks
                        if (targetY + directionY >= Game::LEVEL_WIDTH)
                            break;
                        if (!MovementIsInRange(targetX, targetY + directionY))
                            break;

                        targetY += directionY;
                    }
                } else if (mUnitType == "Skeleton") {
                    // Moves just one tile to each direction
                    // Sanity checks
                    if (!(targetX + directionX >= Game::LEVEL_HEIGHT))
                        if (MovementIsInRange(targetX + directionX, targetY))
                            targetX += directionX;
                    if (!(targetY + directionY >= Game::LEVEL_WIDTH))
                        if (!MovementIsInRange(targetX, targetY + directionY))
                            targetY += directionY;
                } else if (mUnitType == "Boss") {
                    // Moves randomly a certain amount of times
                    int movementX = Random::GetIntRange(1, static_cast<int>((static_cast<float>(mStats.mov) + 1) / 2));
                    for (int i = 0; i < movementX; i++) {
                        // Sanity checks
                        if (targetX + directionX >= Game::LEVEL_HEIGHT)
                            break;
                        if (!MovementIsInRange(targetX + directionX, targetY))
                            break;

                        targetX += directionX;
                    }
                    int movementY = Random::GetIntRange(1, static_cast<int>((static_cast<float>(mStats.mov) + 1) / 2));
                    for (int i = 0; i < movementY; i++) {
                        // Sanity checks
                        if (targetY + directionY >= Game::LEVEL_WIDTH)
                            break;
                        if (!MovementIsInRange(targetX, targetY + directionY))
                            break;

                        targetY += directionY;
                    }
                }

                // Sets the enemy to new position
                SetXY(targetX, targetY);
            }

            // Plays audio for movement
            mGame->GetAudio()->PlaySound("CursorMove.ogg");

            // Restart timer
            mMovementTimer = MOVEMENT_TIMER;

            // Changes state
            mEnemyState = EnemyState::Attacking;
        } else
            mMovementTimer -= deltaTime;
    } else if (mEnemyState == EnemyState::Attacking) {
        if (mAttackTimer <= 0) {
            // If a closest unit was found
            if (mClosestUnit != nullptr) {
                // Calculates distance to closest unit
                int distance = Math::Abs(mClosestUnit->GetX() - GetX()) + Math::Abs(mClosestUnit->GetY() - GetY());

                // If it is in attack distance, attacks it
                if (GetEquippedWeapon())
                    if (distance <= GetEquippedWeapon()->range)
                        Attack(mClosestUnit);
            }

            // Restart timer
            mAttackTimer = ATTACK_TIMER;

            // Changes state
            if (GetStats().currHp <= 0) {
                mEnemyState = EnemyState::Dead;
            } else {
                mEnemyState = EnemyState::Waiting;
            }
        } else
            mAttackTimer -= deltaTime;
    } else if (mEnemyState == EnemyState::Waiting) {
        if (mWaitTimer <= 0) {
            // Heals with a probability
            float chanceHeal = mUnitType == "Boss" ? 0.25f : 0.1f;
            float chanceHealLow = mUnitType == "Boss" ? 0.75f : 0.5f;

            if (static_cast<float>(mStats.currHp) < 0.25f * static_cast<float>(mStats.hp)) {
                if (Random::GetFloat() < chanceHealLow) {
                    mStats.currHp = static_cast<int>(Math::Clamp(
                        mStats.currHp + Random::GetFloatRange(0.33, 0.66) * static_cast<float>(mStats.hp),
                        0.0f, static_cast<float>(mStats.hp)));

                    // Plays audio
                    mGame->GetAudio()->PlaySound("Heal.ogg");

                    // Plays visual effects
                    mGame->GetParticleSystem()->CreateAnimatedParticle(GetX(), GetY(), "Heal");
                }
            } else {
                if (Random::GetFloat() < chanceHeal) {
                    mStats.currHp = static_cast<int>(Math::Clamp(
                        mStats.currHp + Random::GetFloatRange(0.33, 0.66) * static_cast<float>(mStats.hp),
                        0.0f, static_cast<float>(mStats.hp)));

                    // Plays audio
                    mGame->GetAudio()->PlaySound("Heal.ogg");

                    // Plays visual effects
                    mGame->GetParticleSystem()->CreateAnimatedParticle(GetX(), GetY(), "Heal");
                }
            }

            // Restart timer
            mWaitTimer = WAIT_TIMER;

            // Changes state
            mEnemyState = EnemyState::Finished;
        } else
            mWaitTimer -= deltaTime;
    }
}
