#include "DrawAOEComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawAOEComponent::DrawAOEComponent(Actor *owner, Vector2 pos, int radius, int drawOrder)
    : DrawComponent(owner, drawOrder)
      , mPosition(pos)
      , mRadius(radius) {
}

void DrawAOEComponent::Draw(SDL_Renderer *renderer) {
    // Returns if disabled
    if (!mEnabled)
        return;

    // Sets draw color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha);

    // Gets positions
    Vector2 ownerPos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();
    Vector2 pos = Vector2(static_cast<int>((ownerPos.x + mPosition.x - cameraPos.x) / Game::TILE_SIZE),
                          static_cast<int>((ownerPos.y + mPosition.y - cameraPos.y) / Game::TILE_SIZE));

    // Algorithm that traverses all the tiles in radius manhattan distance
    for (int radius = 1; radius <= mRadius; radius++) {
        for (int offset = 0; offset < radius; offset++) {
            int invOffset = radius - offset;

            // Draws the tiles
            int x = pos.x + offset;
            int y = pos.y + invOffset;
            SDL_Rect fillRect = {x * Game::TILE_SIZE, y * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha); // 2nd line of code in question
            SDL_RenderFillRect(renderer, &fillRect);

            x = pos.x + invOffset;
            y = pos.y - offset;
            fillRect = {x * Game::TILE_SIZE, y * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha); // 2nd line of code in question
            SDL_RenderFillRect(renderer, &fillRect);

            x = pos.x - offset;
            y = pos.y - invOffset;
            fillRect = {x * Game::TILE_SIZE, y * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha); // 2nd line of code in question
            SDL_RenderFillRect(renderer, &fillRect);

            x = pos.x - invOffset;
            y = pos.y + offset;
            fillRect = {x * Game::TILE_SIZE, y * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, mColor.x, mColor.y, mColor.z, mAlpha); // 2nd line of code in question
            SDL_RenderFillRect(renderer, &fillRect);
        }
    }
}
