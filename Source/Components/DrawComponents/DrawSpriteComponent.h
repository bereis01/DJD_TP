#pragma once
#include "DrawComponent.h"
#include <string>

class DrawSpriteComponent : public DrawComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawSpriteComponent(Actor *owner, const std::string &texturePath, int width = 0, int height = 0,
                        int widthOffset = 0, int heightOffset = 0, int drawOrder = 100);

    virtual ~DrawSpriteComponent();

    void Draw(SDL_Renderer *renderer) override;

protected:
    // Sprite as a texture
    SDL_Texture *mSpriteSheetSurface;

    int mWidth;
    int mHeight;

    int mWidthOffset;
    int mHeightOffset;
};
