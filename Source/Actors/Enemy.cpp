#include "Enemy.h"
#include "Unit.h"
#include "../Game.h"

Enemy::Enemy(Game *game, const std::string &spriteSheetPath, const std::string &spriteSheetData, Stats stats)
    : Unit(game, spriteSheetPath, spriteSheetData, stats) {
    // Draws the colored square where the enemy is
    mTypeIndicator = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mTypeIndicator->SetColor(Vector3(255, 0, 0));
    mTypeIndicator->SetAlpha(50);

    // Life bar
    mRedBar = new DrawPolygonComponent(this, Vector2(0, Game::TILE_SIZE), Vector2(Game::TILE_SIZE, 2));
    mRedBar->SetColor(Vector3(255, 0, 0));

    mGreenBar = new DrawPolygonComponent(this, Vector2(0, Game::TILE_SIZE), Vector2(Game::TILE_SIZE, 2), 150);
    mGreenBar->SetColor(Vector3(0, 255, 0));
}


Enemy::~Enemy() {
    mGame->RemoveEnemy(this);
}

void Enemy::OnUpdate(float deltaTime) {
    // Updates life indicator
    mGreenBar->SetSize(Vector2((float(mStats.currHp) / float(mStats.hp)) * Game::TILE_SIZE, 2));

    // Returns if not enemy turn
    if (mGame->GetGamePlayState() != Game::GamePlayState::EnemyTurn) {
        return;
    }

    // State machine to control actions
    if (mEnemyState == EnemyState::Moving) {
        if (mMovementTimer <= 0) {
            // Gets the closest unit and its position
            int selfX = GetX();
            int selfY = GetY();

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

            // Moves the enemy towards the closest unit
            // Assumes that the arena is convex
            int distance = Math::Abs(closestPos.x - selfX) + Math::Abs(closestPos.y - selfY);
            if (distance > mStats.mov) // Trims the distance to movement stat
                distance = mStats.mov;
            if (GetEquippedWeapon()) // Does not move if in attack range
                if (distance <= GetEquippedWeapon()->range)
                    distance = 0;
            // Alternates moving between X and Y for the value of the distance
            while (distance > 0) {
                if (distance % 2 == 0) {
                    if ((closestPos.x - selfX) > 0) {
                        // Only goes to the direction if there is no unit and if it is valid
                        if (mGame->GetUnitByPosition(selfX + 1, selfY) == nullptr && mGame->GetLevelData(
                                selfX + 1, selfY) != 0)
                            selfX += 1;
                    } else {
                        // Only goes to the direction if there is no unit and if it is valid
                        if (mGame->GetUnitByPosition(selfX - 1, selfY) == nullptr && mGame->GetLevelData(
                                selfX - 1, selfY) != 0)
                            selfX -= 1;
                    }
                } else {
                    if ((closestPos.y - selfY) > 0) {
                        // Only goes to the direction if there is no unit and if it is valid
                        if (mGame->GetUnitByPosition(selfX, selfY + 1) == nullptr && mGame->GetLevelData(
                                selfX, selfY + 1) != 0)
                            selfY += 1;
                    } else {
                        // Only goes to the direction if there is no unit and if it is valid
                        if (mGame->GetUnitByPosition(selfX, selfY - 1) == nullptr && mGame->GetLevelData(
                                selfX, selfY - 1) != 0)
                            selfY -= 1;
                    }
                }
                distance--;
            }
            SetXY(selfX, selfY);

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
            // Restart timer
            mWaitTimer = WAIT_TIMER;

            // Changes state
            mEnemyState = EnemyState::Finished;
        } else
            mWaitTimer -= deltaTime;
    }
}
