#include "MenuScreen.h"
#include "../Audio/AudioSystem.h"

MenuScreen::MenuScreen(Game *game, const std::string &fontName, SoundHandle handle)
    : UIScreen(game, fontName, true), mMusicHandle(handle) {
}

void MenuScreen::Update(float deltaTime) {
    if (mButtonTimer > 0) {
        mButtonTimer -= deltaTime;
    } else {
        // Creates interactive buttons
        std::string startButton = "START";
        AddButton(startButton, Vector2((mGame->GetWindowWidth() - 200) / 2, 450), Vector2(100, 30),
                  [this]() { mGame->SetGameScene(Game::GameScene::Level1); },
                  Vector2(startButton.size() * CHAR_WIDTH, WORD_HEIGHT));

        std::string quitButton = "QUIT";
        AddButton(quitButton, Vector2((mGame->GetWindowWidth() - 200) / 2 + 100, 450), Vector2(100, 30),
                  [this]() { mGame->Quit(); },
                  Vector2(quitButton.size() * CHAR_WIDTH, WORD_HEIGHT));

        // Creates version text
        std::string version = "version 1.0.0";
        AddText(version, Vector2(5, mGame->GetWindowHeight() - WORD_HEIGHT - 5),
                Vector2(version.size() * CHAR_WIDTH, WORD_HEIGHT));
    }
}

void MenuScreen::HandleKeyPress(int key) {
    // Returns if not interactive
    if (!mInteractive || mButtonTimer > 0)
        return;

    if (key == SDLK_a) {
        // Changes selected button
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    } else if (key == SDLK_d) {
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

        // Stops menu music
        mGame->GetAudio()->StopSound(mMusicHandle);

        // Plays audio
        mGame->GetAudio()->PlaySound("Confirm.ogg");
    }
}
