#include "Enemy.h"
#include "Unit.h"
#include "../Game.h"

Enemy::Enemy(Game *game, const std::string &texturePath, int mov)
    : Unit(game, texturePath, mov) {
    // Draws the colored square where the enemy is
    mTypeIndicator = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mTypeIndicator->SetColor(Vector3(255, 0, 0));
    mTypeIndicator->SetAlpha(50);
}


Enemy::~Enemy() {
    mGame->RemoveEnemy(this);
}

void Enemy::OnUpdate(float deltaTime) {
    // Returns if not enemy turn
    if (mGame->GetGamePlayState() != Game::GamePlayState::EnemyTurn) {
        return;
    }

    // State machine to control actions
    if (mState == EnemyState::Moving) {
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
            if (distance > mMovement) // Trims the distance to movement stat
                distance = mMovement;
            if (mEquippedWeapon) // Does not move if in attack range
                if (distance <= mEquippedWeapon->range)
                    distance = 0;
            while (distance > 0) {
                if (distance % 2 == 0)
                    if ((closestPos.x - selfX) > 0)
                        selfX += 1;
                    else
                        selfX -= 1;
                else if ((closestPos.y - selfY) > 0)
                    selfY += 1;
                else
                    selfY -= 1;
                distance--;
            }
            SetXY(selfX, selfY);

            // Restart timer
            mMovementTimer = MOVEMENT_TIMER;

            // Changes state
            mState = EnemyState::Attacking;
        } else
            mMovementTimer -= deltaTime;
    } else if (mState == EnemyState::Attacking) {
        if (mAttackTimer <= 0) {
            // If a closest unit was found
            if (mClosestUnit != nullptr) {
                // Calculates distance to closest unit
                int distance = Math::Abs(mClosestUnit->GetX() - GetX()) + Math::Abs(mClosestUnit->GetY() - GetY());

                // If it is in attack distance, attacks it
                if (mEquippedWeapon)
                    if (distance <= mEquippedWeapon->range)
                        Attack(mClosestUnit);
            }

            // Restart timer
            mAttackTimer = ATTACK_TIMER;

            // Changes state
            mState = EnemyState::Waiting;
        } else
            mAttackTimer -= deltaTime;
    } else if (mState == EnemyState::Waiting) {
        if (mWaitTimer <= 0) {
            // Restart timer
            mWaitTimer = WAIT_TIMER;

            // Changes state
            mState = EnemyState::Finished;
        } else
            mWaitTimer -= deltaTime;
    }
}
