#include "UIFont.h"
#include <vector>
#include <SDL_image.h>

UIFont::UIFont(SDL_Renderer *renderer)
    : mRenderer(renderer) {
}

UIFont::~UIFont() {
}

bool UIFont::Load(const std::string &fileName) {
    // We support these font sizes
    std::vector<int> fontSizes = {
        8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
        34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60, 64, 68, 72
    };

    // Traverses the list of sizes and tries to load the font
    for (auto size: fontSizes) {
        TTF_Font *font = TTF_OpenFont(fileName.c_str(), size);
        if (font == nullptr) {
            SDL_Log("Failed to load font of size %d", size);
            return false;
        }
        mFontData.emplace(size, font);
    }

    return true;
}

void UIFont::Unload() {
    // Closes all loades fonts
    for (auto pair: mFontData)
        TTF_CloseFont(pair.second);
    mFontData.clear();
}

SDL_Texture *UIFont::RenderText(const std::string &text, const Vector3 &color,
                                int pointSize, unsigned wrapLength) {
    if (!mRenderer) {
        SDL_Log("Renderer is null. Can't Render Text!");
        return nullptr;
    }

    // Convert to SDL_Color
    SDL_Color sdlColor;

    // Swap red and blue so we get RGBA instead of BGRA
    sdlColor.b = static_cast<Uint8>(color.x * 255);
    sdlColor.g = static_cast<Uint8>(color.y * 255);
    sdlColor.r = static_cast<Uint8>(color.z * 255);
    sdlColor.a = 255;

    // Checks if the size is available
    auto it = mFontData.find(pointSize);
    if (it == mFontData.end()) {
        SDL_Log("Unsupported font size used!");
        return nullptr;
    }

    // Loads the surface
    SDL_Surface *fontSurface = TTF_RenderUTF8_Blended_Wrapped(it->second, text.c_str(), sdlColor, wrapLength);
    if (fontSurface == nullptr) {
        SDL_Log("Failed to create surface from font!");
        return nullptr;
    }

    // Creates texture from surface
    SDL_Texture *fontTexture = SDL_CreateTextureFromSurface(mRenderer, fontSurface);
    if (fontTexture == nullptr) {
        SDL_Log("Failed to create texture from font surface!");
        return nullptr;
    }

    return fontTexture;
}
