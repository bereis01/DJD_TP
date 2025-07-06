#include "UIImage.h"

UIImage::UIImage(SDL_Renderer *renderer, const std::string &imagePath, const Vector2 &pos, const Vector2 &size,
                 const Vector3 &color)
    : UIElement(pos, size, color),
      mTexture(nullptr) {
    SDL_Surface *surface = IMG_Load(imagePath.c_str());
    if (!surface)
        SDL_Log("Failed to load image: %s", IMG_GetError());

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    SDL_FreeSurface(surface);

    mTexture = texture;
}

UIImage::~UIImage() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

void UIImage::SetImage(SDL_Renderer* renderer, const std::string& imagePath) {
    SDL_Surface *surface = IMG_Load(imagePath.c_str());
    if (!surface)
        SDL_Log("Failed to load image: %s", IMG_GetError());

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    SDL_FreeSurface(surface);

    mTexture = texture;
}


void UIImage::Draw(SDL_Renderer *renderer, const Vector2 &screenPos) {
    if (!mTexture)
        return;

    if (!mIsVisible)
        return;

    SDL_Rect imageQuad = {
        static_cast<int>(mPosition.x + screenPos.x), static_cast<int>(mPosition.y + screenPos.y),
        static_cast<int>(mSize.x), static_cast<int>(mSize.y)
    };

    SDL_RenderCopy(renderer, mTexture, NULL, &imageQuad);
}
