#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawTextureComponent.h"

class Particle : public Actor {
public:
    static const int LIVE_TIMER = 1.0f; // In seconds

    Particle(Game *game, SDL_Texture *texture, const Vector2 &size);

    void OnUpdate(float deltaTime) override;

private:
    // Timers
    float mLiveTimer = LIVE_TIMER;

    // Component that draws the texture
    DrawTextureComponent *mDrawComponent = nullptr;

    // Component that moves the particle
    RigidBodyComponent *mRigidBodyComponent = nullptr;
};
