#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawTextureComponent.h"

class TitleParticle : public Actor {
public:
    static const int LIVE_TIMER = 5.0f; // In seconds

    TitleParticle(Game *game, SDL_Texture *texture, const Vector2 &size, float timer = LIVE_TIMER,
                  float fadeInTimer = 1.0f, float fadeOutTimer = 1.0f, bool setTimer = true);

    void OnUpdate(float deltaTime) override;

private:
    // Timers
    bool mSetTimer;
    float mTimer;
    float mLiveTimer;
    float mFadeInTimer;
    float mFadeOutTimer;

    // Component that draws the texture
    DrawTextureComponent *mDrawComponent = nullptr;

    // Component that moves the particle
    RigidBodyComponent *mRigidBodyComponent = nullptr;
};
