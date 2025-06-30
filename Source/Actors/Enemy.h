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
        Finished
    };

    const float MOVEMENT_TIMER = 0.5f;
    const float ATTACK_TIMER = 0.5f;
    const float WAIT_TIMER = 0.0f;

    Enemy(Game *game, const std::string &texturePath, int mov = 5);

    ~Enemy();

    // Update functions
    void OnUpdate(float deltaTime) override;

    // State manipulation
    void SetState(EnemyState state) { mState = state; }
    EnemyState GetState() { return mState; }

private:
    // Draw type indicator
    DrawPolygonComponent *mTypeIndicator;

    // State
    EnemyState mState = EnemyState::None;
    Ally *mClosestUnit = nullptr;

    // Timers
    float mMovementTimer = MOVEMENT_TIMER;
    float mAttackTimer = ATTACK_TIMER;
    float mWaitTimer = WAIT_TIMER;
};
