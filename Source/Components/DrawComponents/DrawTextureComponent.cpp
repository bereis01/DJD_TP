#include "DrawTextureComponent.h"
#include "../../Actors/Actor.h"

DrawTextureComponent::DrawTextureComponent(Actor *owner, SDL_Texture *texture, const int width,
                                           const int height, const int drawOrder)
    : DrawComponent(owner, drawOrder)
      , mWidth(width)
      , mHeight(height)
      , mTexture(texture) {
}

DrawTextureComponent::~DrawTextureComponent() {
    // Destroys the texture
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

void DrawTextureComponent::Draw(SDL_Renderer *renderer) {
    // Do not draw if not visible
    if (!mIsVisible)
        return;

    // Gets positions
    Vector2 pos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

    // Creates the destiny rect positions and size
    SDL_Rect dstRect = {
        static_cast<int>(pos.x - cameraPos.x),
        static_cast<int>(pos.y - cameraPos.y),
        mWidth,
        mHeight
    };

    // Applies flip if applicable
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (mOwner->GetRotation() == Math::Pi) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    // Renders the texture
    SDL_SetTextureAlphaMod(mTexture, mAlpha);
    SDL_RenderCopyEx(renderer, mTexture, nullptr, &dstRect, mOwner->GetRotation(), nullptr, flip);
}
