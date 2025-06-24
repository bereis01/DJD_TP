//
// Created by Lucas N. Ferreira on 28/05/25.
//

#include "UIImage.h"

UIImage::UIImage(const std::string &imagePath, SDL_Renderer* renderer, const Vector2 &pos, const Vector2 &size, const Vector3 &color)
    : UIElement(pos, size, color),
    mTexture(nullptr)
{
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return;
    }

    mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!mTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

UIImage::~UIImage()
{
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
    }
    mTexture = nullptr;
}

void UIImage::Draw(SDL_Renderer* renderer, const Vector2 &screenPos)
{
    if (!mTexture) {
        return;
    }
    SDL_Rect dstrect;
    dstrect.x = static_cast<int>((mPosition + screenPos).x);
    dstrect.y = static_cast<int>((mPosition + screenPos).y);
    dstrect.w = static_cast<int>(mSize.x);
    dstrect.h = static_cast<int>(mSize.y);

    SDL_RenderCopy(renderer, mTexture, nullptr, &dstrect);
}