#include "AnimatedParticle.h"

AnimatedParticle::AnimatedParticle(Game *game, const std::string &type, const Vector2 &size) : Actor(game) {
    // Loads draw component for the texture
    if (type == "Death") {
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Effects/Death.png",
                                                   "../Assets/Sprites/Effects/Death.json", 300);
        mDrawComponent->AddAnimation("Default", {0, 1, 2, 3, 4});
    } else if (type == "Heal") {
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Effects/Heal.png",
                                                   "../Assets/Sprites/Effects/Heal.json", 300);
        mDrawComponent->AddAnimation("Default", {0, 1, 2, 3});
    }
    mDrawComponent->SetAnimation("Default");
    mDrawComponent->SetAnimFPS(10.0f);
}

void AnimatedParticle::OnUpdate(float deltaTime) {
    // Updates timer and deletes itself if it has finished
    if (mLiveTimer <= 0)
        mState = ActorState::Destroy;
    else
        mLiveTimer -= deltaTime;
}
