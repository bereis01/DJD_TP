#pragma once
#include "Unit.h"
#include "Ally.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

class Enemy : public Unit {
public:
    enum class EnemyState {
        None,
        Moving,
        Attacking,
        Waiting,
        Finished,
        Dead
    };

    const float MOVEMENT_TIMER = 0.5f;
    const float ATTACK_TIMER = 0.5f;
    const float WAIT_TIMER = 0.0f;

    Enemy(Game *game, const std::string &unitType, Stats stats);

    ~Enemy();

    // Update functions
    void OnUpdate(float deltaTime) override;

    // State manipulation
    void SetEnemyState(EnemyState state) { mEnemyState = state; }
    EnemyState GetEnemyState() { return mEnemyState; }

private:
    // Draw type indicator
    DrawPolygonComponent *mTypeIndicator;

    // State
    EnemyState mEnemyState = EnemyState::None;
    Ally *mClosestUnit = nullptr;

    // Timers
    float mMovementTimer = MOVEMENT_TIMER;
    float mAttackTimer = ATTACK_TIMER;
    float mWaitTimer = WAIT_TIMER;

    // Life bar
    DrawPolygonComponent *mRedBar;
    DrawPolygonComponent *mGreenBar;
};
