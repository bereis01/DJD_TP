#include "UIText.h"
#include "UIFont.h"

UIText::UIText(const std::string &text, class UIFont *font, int pointSize, const unsigned wrapLength,
               const Vector2 &pos, const Vector2 &size, const Vector3 &color)
    : UIElement(pos, size, color)
      , mFont(font)
      , mPointSize(pointSize)
      , mWrapLength(wrapLength)
      , mTextTexture(nullptr) {
    // Sets initial text
    SetText(text);
}

UIText::~UIText() {
}

void UIText::SetText(const std::string &text) {
    // Cleans texture if allocated
    if (mTextTexture != nullptr) {
        SDL_DestroyTexture(mTextTexture);
        mTextTexture = nullptr;
    }

    // Sets the text and initializes the corresponding texture
    mText = text;
    mTextTexture = mFont->RenderText(mText, mColor, mPointSize, mWrapLength);
}

void UIText::Draw(SDL_Renderer *renderer, const Vector2 &screenPos) {
    // Draws the text texture in the given relative position
    SDL_Rect titleQuad = {
        static_cast<int>(mPosition.x + screenPos.x), static_cast<int>(mPosition.y + screenPos.y),
        static_cast<int>(mSize.x), static_cast<int>(mSize.y)
    };
    SDL_RenderCopyEx(renderer, mTextTexture, NULL, &titleQuad, 0, NULL, SDL_FLIP_NONE);
}
