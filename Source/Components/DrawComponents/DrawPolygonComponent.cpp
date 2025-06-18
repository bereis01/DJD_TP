#include "DrawPolygonComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawPolygonComponent::DrawPolygonComponent(Actor *owner, Vector2 pos, Vector2 size, int drawOrder)
    : DrawComponent(owner, drawOrder)
      , mPosition(pos)
      , mSize(size) {
}

void DrawPolygonComponent::Draw(SDL_Renderer *renderer) {
    // Sets draw color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha);

    // Gets positions
    Vector2 pos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

    // Draws rect
    SDL_Rect fillRect = {
        static_cast<int>(pos.x + mPosition.x - cameraPos.x), static_cast<int>(pos.y + mPosition.y - cameraPos.y),
        static_cast<int>(mSize.x), static_cast<int>(mSize.y)
    };
    SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha); // 2nd line of code in question
    SDL_RenderFillRect(renderer, &fillRect);
}
