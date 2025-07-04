#include "TurnScreen.h"
#include "../Game.h"

TurnScreen::TurnScreen(Game *game, const std::string &fontName)
    : UIScreen(game, fontName) {
    // Puts itself in game stack
    mGame->PushUI(this);

    // Draws the turn indicator
    const Vector2 size = Vector2(130, 40);
    const Vector2 pos = Vector2(((mGame->GetWindowWidth() - size.x) / 2), 0);
    AddImage("../Assets/UI/PlainBackground.png", pos, size);
    mText = AddText("Player Turn", pos + Vector2(10, 10), Vector2(11 * CHAR_WIDTH, WORD_HEIGHT));
}

void TurnScreen::ChangeTurn() {
    // Defines size and position
    const Vector2 size = Vector2(130, 40);
    const Vector2 pos = Vector2(((mGame->GetWindowWidth() - size.x) / 2), 0);

    // Changes the text according to gameplay state
    if (mGame->GetGamePlayState() == Game::GamePlayState::EnemyTurn)
        mText->SetText("Enemy Turn", pos + Vector2(15, 10), Vector2(10 * CHAR_WIDTH, WORD_HEIGHT));
    else
        mText->SetText("Player Turn", pos + Vector2(10, 10), Vector2(11 * CHAR_WIDTH, WORD_HEIGHT));
}
