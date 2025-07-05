#include "TitleParticle.h"

TitleParticle::TitleParticle(Game *game, SDL_Texture *texture, const Vector2 &size) : Actor(game) {
    // Loads draw component for the texture
    mDrawComponent = new DrawTextureComponent(this, texture, size.x, size.y, 300);
}

void TitleParticle::OnUpdate(float deltaTime) {
    // Updates timer and deletes itself if it has finished
    if (mLiveTimer <= 0) {
        mDrawComponent->SetIsVisible(false);
        mState = ActorState::Destroy;
    } else {
        mLiveTimer -= deltaTime;

        // Fade in/out of texture
        if (mLiveTimer > 4) {
            mDrawComponent->SetAlpha((5 - mLiveTimer) * 255);
        } else if (mLiveTimer < 1) {
            mDrawComponent->SetAlpha((mLiveTimer) * 255);
        }
    }
}
