#pragma once
#include "../Game.h"
#include "../Audio/AudioSystem.h"

class MenuScreen : public UIScreen {
public:
    const int BUTTON_TIMER = 5.0f;

    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 15.0f;
    const int WORD_OFFSET = 5.0f;
    const int CHAR_WIDTH = 10.0f;

    MenuScreen(Game *game, const std::string &fontName);

    void Update(float deltaTime) override;

    void HandleKeyPress(int key) override;

private:
    float mButtonTimer = BUTTON_TIMER;
};
