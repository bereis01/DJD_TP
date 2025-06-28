#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class TurnScreen : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int WORD_OFFSET = 5.0f;
    const int CHAR_WIDTH = 10.0f;

    TurnScreen(Game *game, const std::string &fontName);

    ~TurnScreen();

    void ChangeTurn();

    UIText *mText;
};
