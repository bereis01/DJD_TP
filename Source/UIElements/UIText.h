#pragma once
#include <string>
#include <SDL.h>
#include "../Utils/Math.h"
#include "UIElement.h"

class UIText : public UIElement {
public:
    UIText(const std::string &text, class UIFont *font, int pointSize = 40, unsigned wrapLength = 1024,
           const Vector2 &pos = Vector2::Zero, const Vector2 &size = Vector2(100.0f, 20.0f),
           const Vector3 &color = Color::White);

    ~UIText();

    void SetText(const std::string &name);

    void SetText(const std::string &name, const Vector2 &pos, const Vector2 &size);

    void Draw(SDL_Renderer *renderer, const Vector2 &screenPos) override;

protected:
    std::string mText;
    UIFont *mFont;
    SDL_Texture *mTextTexture;

    unsigned int mPointSize;
    unsigned int mWrapLength;
};
