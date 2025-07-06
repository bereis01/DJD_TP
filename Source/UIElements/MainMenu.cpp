#include "MainMenu.h"
#include "../Game.h"
#include "../Actors/Unit.h"
#include "../Audio/AudioSystem.h"

MainMenu::MainMenu(Game *game, const std::string &fontName)
    : UIScreen(game, fontName, true) {
    const Vector2 screenSize = Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight());
    AddImage("../Assets/UI/AymrLogo.png", Vector2::Zero, screenSize * 0.8);
    AddButton("Start Game",  screenSize * 0.5 + Vector2(0, screenSize.y / 4), Vector2(100, 30), [this]() { mGame->SetGameScene(Game::GameScene::Level1, Game::TRANSITION_TIME, false); },
              Vector2(10 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Quit", screenSize * 0.5 + Vector2(0, screenSize.y / 4 + BUTTON_OFFSET), Vector2(100, 30), [this]() { mGame->Quit(); },
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
}

void MainMenu::HandleKeyPress(int key) {
    if (!mInteractive)
        return;
    if (key == SDLK_w) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    } else if (key == SDLK_s) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex++;
        if (mSelectedButtonIndex > static_cast<int>(mButtons.size()) - 1) {
            mSelectedButtonIndex = 0;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    } else if (key == SDLK_RETURN) {
        if (mSelectedButtonIndex >= 0 && mSelectedButtonIndex <= static_cast<int>(mButtons.size()) - 1) {
            mButtons[mSelectedButtonIndex]->OnClick();
            SetSelectedButtonIndex(0);
        }
    }
}
