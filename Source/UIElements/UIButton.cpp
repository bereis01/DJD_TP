//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIButton.h"

UIButton::UIButton(const std::string& text, class UIFont* font, std::function<void()> onClick,
                    const Vector2& pos, const Vector2& size, SDL_Renderer* renderer, const Vector3& color,
                    int pointSize , unsigned wrapLength,
                    const Vector2 &textPos, const Vector2 &textSize, const Vector3& textColor)
        :UIElement(pos, size, color)
        ,mOnClick(onClick)
        ,mHighlighted(false)
        ,mText(text, font, pointSize, wrapLength, textPos, textSize, textColor)
{

}

UIButton::~UIButton()
{

}


void UIButton::Draw(SDL_Renderer *renderer, const Vector2 &screenPos)
{
    SDL_Rect titleQuad;
    titleQuad.x = static_cast<int>((mPosition + screenPos).x);
    titleQuad.y = static_cast<int>((mPosition + screenPos).y);
    titleQuad.w = static_cast<int>(mSize.x);
    titleQuad.h = static_cast<int>(mSize.y);

    if (mHighlighted) {
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &titleQuad);
    }

    Vector2 pos = Vector2::Zero;
    pos.x = mPosition.x + (mSize.x - mText.GetSize().x) * 0.5f;
    pos.y = mPosition.y + (mSize.y - mText.GetSize().y) * 0.5f;
    mText.SetPosition(pos);
    mText.Draw(renderer, screenPos);
}

void UIButton::OnClick()
{
    if (mOnClick != nullptr) {
        mOnClick();
    }
}
