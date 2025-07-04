#include "ActionScreen.h"
#include "../Game.h"
#include "../Actors/Unit.h"

ActionScreen::ActionScreen(Game *game, const std::string &fontName)
    : UIScreen(game, fontName, true) {
    const Vector2 screenSize = Vector2(150, 150);
    const Vector2 screenPos = Vector2(mGame->GetWindowWidth() - 250, 100);
    AddImage("../Assets/UI/Shield.png", screenPos, screenSize);
    AddButton("Attack", screenPos + Vector2(25, 30), Vector2(100, 30), [this]() { mGame->SetupAttack(); },
              Vector2(6 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Items", screenPos + Vector2(25, 60), Vector2(100, 30), [this]() { mGame->ShowItems(); },
              Vector2(5 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Wait", screenPos + Vector2(25, 90), Vector2(100, 30), [this]() { mGame->GetSelectedUnit()->Wait(); },
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
}

void ActionScreen::HandleKeyPress(int key) {
    if (!mInteractive)
        return;
    if (key == SDLK_w) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    } else if (key == SDLK_s) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex++;
        if (mSelectedButtonIndex > static_cast<int>(mButtons.size()) - 1) {
            mSelectedButtonIndex = 0;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    } else if (key == SDLK_RETURN) {
        if (mSelectedButtonIndex >= 0 && mSelectedButtonIndex <= static_cast<int>(mButtons.size()) - 1) {
            mButtons[mSelectedButtonIndex]->OnClick();
            SetSelectedButtonIndex(0);
        }
    } else if (key == SDLK_b) {
        mGame->PopUI();
        if (mGame->GetGamePlayState() == Game::GamePlayState::ChoosingAction) {
            mGame->SetGamePlayState(Game::GamePlayState::MovingUnit);
            mGame->GetSelectedUnit()->SetPosition(mGame->GetSelectedUnit()->GetOldPosition());
            SetSelectedButtonIndex(0);
        }
    }
}
