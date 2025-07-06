#pragma once
#include "DrawComponent.h"

class DrawAOEComponent : public DrawComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawAOEComponent(Unit *owner, const std::string &type, Vector2 pos, int radius, int drawOrder = 100);

    void SetColor(Vector3 color) { mColor = color; }
    void SetAlpha(int alpha) { mAlpha = alpha; }
    void SetEnabled(bool enabled) { mEnabled = enabled; }
    void SetRadius(int radius) { mRadius = radius; }

    void Draw(SDL_Renderer *renderer) override;

protected:
    Vector2 mPosition;
    int mRadius;
    Vector3 mColor = Vector3(255.0f, 255.0f, 255.0f);
    bool mEnabled = false;
    int mAlpha = 50;

    Unit *mUnit;
    std::string mType;
};
