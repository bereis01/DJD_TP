#include "ActionScreen.h"
#include "../Game.h"
#include "../Actors/Unit.h"
#include "../Audio/AudioSystem.h"

ActionScreen::ActionScreen(Game *game, const std::string &fontName)
    : UIScreen(game, fontName, true) {
    const Vector2 screenSize = Vector2(100, 90);
    const Vector2 screenPos = Vector2(mGame->GetWindowWidth() - 250, 100);
    AddImage("../Assets/UI/PlainBackground.png", screenPos, screenSize);
    AddButton("Attack", screenPos + Vector2(0, 0), Vector2(100, 30), [this]() { mGame->SetupAttack(); },
              Vector2(6 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Items", screenPos + Vector2(0, 30), Vector2(100, 30), [this]() { mGame->ShowItems(); },
              Vector2(5 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Wait", screenPos + Vector2(0, 60), Vector2(100, 30), [this]() { mGame->GetSelectedUnit()->Wait(); },
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));

    // Controls
    std::string controlsText = "Press [W, S] to navigate";
    AddText(controlsText,
            Vector2((screenPos.x + (screenSize.x - (controlsText.size() * CHAR_WIDTH)) / 2),
                    (screenPos.y + screenSize.y + 5)), Vector2(CHAR_WIDTH * controlsText.size(), WORD_HEIGHT),
            POINT_SIZE);

    controlsText = "Press [ENTER] to confirm";
    AddText(controlsText,
            Vector2((screenPos.x + (screenSize.x - (controlsText.size() * CHAR_WIDTH)) / 2),
                    (screenPos.y + screenSize.y + 5 + 1.5 * WORD_HEIGHT)),
            Vector2(CHAR_WIDTH * controlsText.size(), WORD_HEIGHT),
            POINT_SIZE);

    controlsText = "Press [B] to return";
    AddText(controlsText,
            Vector2((screenPos.x + (screenSize.x - (controlsText.size() * CHAR_WIDTH)) / 2),
                    (screenPos.y + screenSize.y + 5 + 3 * WORD_HEIGHT)),
            Vector2(CHAR_WIDTH * controlsText.size(), WORD_HEIGHT),
            POINT_SIZE);
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

        // Plays audio
        mGame->GetAudio()->PlaySound("Confirm.ogg");
    } else if (key == SDLK_b) {
        mGame->PopUI();
        if (mGame->GetGamePlayState() == Game::GamePlayState::ChoosingAction) {
            mGame->SetGamePlayState(Game::GamePlayState::MovingUnit);
            mGame->GetSelectedUnit()->SetPosition(mGame->GetSelectedUnit()->GetOldPosition());
            SetSelectedButtonIndex(0);
        }

        // Plays audio
        mGame->GetAudio()->PlaySound("StatsClose.ogg");
    }
}
