#include "PauseScreen.h"
#include "../Game.h"

PauseScreen::PauseScreen(Game *game, const std::string &fontName): UIScreen(
    game, fontName, true) {
    AddImage("../Assets/UI/PlainBackground.png", SCREEN_POS, SCREEN_SIZE);

    std::string returnButton = "RETURN";
    AddButton(returnButton, SCREEN_POS + Vector2(10, 10),
              Vector2(100, 30),
              [this]() {
                  mGame->TogglePause();
              },
              Vector2(returnButton.size() * CHAR_WIDTH, WORD_HEIGHT));

    std::string quitButton = "QUIT";
    AddButton(quitButton, SCREEN_POS + Vector2(10, 50),
              Vector2(100, 30),
              [this]() {
                  mGame->Quit();
              },
              Vector2(quitButton.size() * CHAR_WIDTH, WORD_HEIGHT));
}

void PauseScreen::HandleKeyPress(int key) {
    // Returns if not interactive
    if (!mInteractive)
        return;

    if (key == SDLK_w) {
        // Changes selected button
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    } else if (key == SDLK_s) {
        // Changes selected button
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

        // Plays audio
        mGame->GetAudio()->PlaySound("Unpause.ogg");
    }
}
