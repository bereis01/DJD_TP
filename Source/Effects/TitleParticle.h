#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawTextureComponent.h"

class TitleParticle : public Actor {
public:
    static const int LIVE_TIMER = 5.0f; // In seconds

    TitleParticle(Game *game, SDL_Texture *texture, const Vector2 &size);

    void OnUpdate(float deltaTime) override;

private:
    // Timers
    float mLiveTimer = LIVE_TIMER;

    // Component that draws the texture
    DrawTextureComponent *mDrawComponent = nullptr;

    // Component that moves the particle
    RigidBodyComponent *mRigidBodyComponent = nullptr;
};
