#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"

class AnimatedParticle : public Actor {
public:
    static const int LIVE_TIMER = 1.0f; // In seconds

    AnimatedParticle(Game *game, const std::string &type, const Vector2 &size);

    void OnUpdate(float deltaTime) override;

private:
    // Timers
    float mLiveTimer = LIVE_TIMER;

    // Component that draws the texture
    DrawAnimatedComponent *mDrawComponent = nullptr;

    // Component that moves the particle
    RigidBodyComponent *mRigidBodyComponent = nullptr;
};
