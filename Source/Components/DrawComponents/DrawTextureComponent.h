#pragma once
#include "DrawComponent.h"

class DrawTextureComponent : public DrawComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawTextureComponent(Actor *owner, SDL_Texture *texture, int width = 0, int height = 0,
                         int drawOrder = 100);

    ~DrawTextureComponent();

    void Draw(SDL_Renderer *renderer) override;

protected:
    // Texture to be drawn
    SDL_Texture *mTexture;

    int mWidth;
    int mHeight;
};
