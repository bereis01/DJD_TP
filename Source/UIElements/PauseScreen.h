#pragma once
#include "UIScreen.h"


class PauseScreen : public UIScreen {
public:
    const Vector2 SCREEN_POS = Vector2((800 - 120) / 2, (640 - 90) / 2);
    const Vector2 SCREEN_SIZE = Vector2(120, 90);

    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 15.0f;
    const int WORD_OFFSET = 5.0f;
    const int CHAR_WIDTH = 10.0f;

    PauseScreen(Game *game, const std::string &fontName);

    void HandleKeyPress(int key) override;
};
