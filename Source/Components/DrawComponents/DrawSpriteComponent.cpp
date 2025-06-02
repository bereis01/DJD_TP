#include "DrawSpriteComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawSpriteComponent::DrawSpriteComponent(class Actor *owner, const std::string &texturePath, const int width,
                                         const int height, const int drawOrder)
    : DrawComponent(owner, drawOrder)
      , mWidth(width)
      , mHeight(height) {
    // Loads spritesheet
    mSpriteSheetSurface = owner->GetGame()->LoadTexture(texturePath);
}

void DrawSpriteComponent::Draw(SDL_Renderer *renderer) {
    // Draws the sprite on the screen
    SDL_RendererFlip flip = mOwner->GetRotation() == 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    Vector2 ownerPosition = mOwner->GetPosition();
    Vector2 cameraPosition = mOwner->GetGame()->GetCameraPos();
    SDL_Rect spriteLocation = {
        static_cast<int>(ownerPosition.x - cameraPosition.x), static_cast<int>(ownerPosition.y - cameraPosition.y),
        mWidth, mHeight
    };
    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, nullptr, &spriteLocation, 0, nullptr, flip);
}
