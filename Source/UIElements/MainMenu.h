#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class MainMenu : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 15.0f;
    const int WORD_OFFSET = 5.0f;
    const int CHAR_WIDTH = 10.0f;
    const int BUTTON_OFFSET = 50.0f;

    MainMenu(Game *game, const std::string &fontName);

    void HandleKeyPress(int key) override;
};
