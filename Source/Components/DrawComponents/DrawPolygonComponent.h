#pragma once
#include "DrawComponent.h"

class DrawPolygonComponent : public DrawComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawPolygonComponent(Actor *owner, Vector2 pos, Vector2 size, int drawOrder = 100);

    void SetColor(Vector3 color) { mColor = color; }
    void SetAlpha(int alpha) { mAlpha = alpha; }

    void Draw(SDL_Renderer *renderer) override;

protected:
    Vector2 mPosition;
    Vector2 mSize;
    Vector3 mColor = Vector3(255.0f, 255.0f, 255.0f);
    int mAlpha = 255;
};
