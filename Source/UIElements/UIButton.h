#pragma once
#include <functional>
#include <string>
#include "UIFont.h"
#include "UIText.h"
#include "UIElement.h"
#include "../Utils/Math.h"

class UIButton : public UIElement {
public:
    UIButton(const std::string &text, UIFont *font, std::function<void()> onClick,
             const Vector2 &pos, const Vector2 &size, const Vector3 &color = Vector3(30, 30, 30),
             int pointSize = 40, unsigned wrapLength = 1024,
             const Vector2 &textPos = Vector2::Zero,
             const Vector2 &textSize = Vector2(80.0f, 20.0f),
             const Vector3 &textColor = Color::White);

    ~UIButton();

    // Set the name of the button
    void SetText(const std::string &text) {mText.SetText(text);}
    void SetTextSize(Vector2 size) {mText.SetSize(size);}

    void Draw(SDL_Renderer *renderer, const Vector2 &screenPos) override;

    void SetHighlighted(bool sel) { mHighlighted = sel; }
    bool GetHighlighted() const { return mHighlighted; }
    void SetFunction(std::function<void()> onClick) {mOnClick = onClick;}

    // Returns true if the point is within the button's bounds
    bool ContainsPoint(const Vector2 &pt) const;

    // Called when button is clicked
    void OnClick();

private:
    // Callback funtion
    std::function<void()> mOnClick;

    // Button name
    UIText mText;

    // Check if the button is highlighted
    bool mHighlighted;
};
