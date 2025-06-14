#include "DrawComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawComponent::DrawComponent(Actor *owner, int drawOrder)
    : Component(owner)
      , mDrawOrder(drawOrder)
      , mIsVisible(true) {
    // Adds itself to game
    mOwner->GetGame()->AddDrawable(this);
}

DrawComponent::~DrawComponent() {
    // Removes itself from game
    mOwner->GetGame()->RemoveDrawable(this);
}

void DrawComponent::Draw(SDL_Renderer *renderer) {
}
