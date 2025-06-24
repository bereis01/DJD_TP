//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIText.h"
#include "UIFont.h"

UIText::UIText(const std::string &text, class UIFont* font, int pointSize, const unsigned wrapLength,
               const Vector2 &pos, const Vector2 &size, const Vector3 &color)
   :UIElement(pos, size, color)
   ,mFont(font)
   ,mPointSize(pointSize)
   ,mWrapLength(wrapLength)
   ,mTextTexture(nullptr)
{
    SetText(text);
}

UIText::~UIText()
{

}

void UIText::SetText(const std::string &text)
{
    if (mTextTexture != nullptr) {
        SDL_DestroyTexture(mTextTexture);
        mTextTexture = nullptr;
    }
    mTextTexture = mFont->RenderText(text, mColor, static_cast<int>(mPointSize), mWrapLength);
    mText = text;
}

void UIText::Draw(SDL_Renderer *renderer, const Vector2 &screenPos)
{
    SDL_Rect titleQuad;
    titleQuad.x = static_cast<int>((mPosition + screenPos).x);
    titleQuad.y = static_cast<int>((mPosition + screenPos).y);
    titleQuad.w = static_cast<int>(mSize.x);
    titleQuad.h = static_cast<int>(mSize.y);

    SDL_RenderCopyEx(renderer, mTextTexture, nullptr, &titleQuad, 0, nullptr, SDL_FLIP_NONE);
}