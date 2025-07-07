#include "TitleParticle.h"

TitleParticle::TitleParticle(Game *game, SDL_Texture *texture, const Vector2 &size, float timer, float fadeInTimer,
                             float fadeOutTimer, bool setTimer, Vector2 offset) : Actor(game),
    mTimer(timer),
    mLiveTimer(timer),
    mFadeInTimer(fadeInTimer),
    mFadeOutTimer(fadeOutTimer),
    mSetTimer(setTimer),
    mSize(size),
    mOffset(offset) {
    // Loads draw component for the texture
    mDrawComponent = new DrawTextureComponent(this, texture, size.x, size.y, 300);
}

void TitleParticle::OnUpdate(float deltaTime) {
    // Updates position based on camera
    SetPosition(Vector2((mGame->GetWindowWidth() - mSize.x) / 2 + mGame->GetCameraPos().x,
                        (mGame->GetWindowHeight() - mSize.y) / 2 + mGame->GetCameraPos().y) + mOffset);

    // Updates timer and deletes itself if it has finished
    if (mLiveTimer <= 0.1f) {
        if (mSetTimer) {
            mDrawComponent->SetIsVisible(false);
            mState = ActorState::Destroy;
        }
    } else {
        mLiveTimer -= deltaTime;

        // Fade in/out of texture
        if (mLiveTimer > (mTimer - mFadeInTimer)) {
            mDrawComponent->SetAlpha(((mTimer - mLiveTimer) / mFadeInTimer) * 255);
        } else if (mLiveTimer < mFadeOutTimer) {
            mDrawComponent->SetAlpha((mLiveTimer / mFadeOutTimer) * 255);
        }
    }
}
