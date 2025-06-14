#include "DrawPolygonComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawPolygonComponent::DrawPolygonComponent(class Actor *owner, std::vector<Vector2> &vertices, int drawOrder)
    : DrawComponent(owner, drawOrder)
      , mVertices(vertices) {
}

void DrawPolygonComponent::Draw(SDL_Renderer *renderer, const Vector3 &color) {
    // Set draw color to passed color
    SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);

    // Gets positions
    Vector2 pos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

    // Render vertices as lines
    for (int i = 0; i < mVertices.size() - 1; i++) {
        SDL_RenderDrawLine(renderer, pos.x + mVertices[i].x - cameraPos.x,
                           pos.y + mVertices[i].y - cameraPos.y,
                           pos.x + mVertices[i + 1].x - cameraPos.x,
                           pos.y + mVertices[i + 1].y - cameraPos.y);
    }

    // Close geometry
    SDL_RenderDrawLine(renderer, pos.x + mVertices[mVertices.size() - 1].x - cameraPos.x,
                       pos.y + mVertices[mVertices.size() - 1].y - cameraPos.y,
                       pos.x + mVertices[0].x - cameraPos.x,
                       pos.y + mVertices[0].y - cameraPos.y);
}
