#pragma once
#include <string>
#include <vector>
#include "../Utils/Math.h"
#include "UIText.h"
#include "UIButton.h"
#include "UIImage.h"

class UIScreen {
public:
    // Tracks if the UI is active or closing
    enum class UIState {
        Active,
        Closing
    };

    UIScreen(class Game *game, const std::string &fontName, bool isInteractive = false);

    virtual ~UIScreen();

    // UIScreen subclasses can override these
    virtual void Update(float deltaTime);

    virtual void Draw(SDL_Renderer *renderer);

    virtual void ProcessInput(const uint8_t *keys);

    virtual void HandleKeyPress(int key);

    // Set state to closing
    void Close();

    // Get state of UI screen
    UIState GetState() const { return mState; }

    // Set state of UI screen
    void SetState(UIState state) { mState = state; }

    // Game getter
    class Game *GetGame() { return mGame; }

    // Add a button to this screen
    UIButton *AddButton(const std::string &name, const Vector2 &pos, const Vector2 &dims,
                        std::function<void()> onClick, const Vector2 &textSize);

    UIText *AddText(const std::string &name, const Vector2 &pos, const Vector2 &dims, int pointSize = 40,
                    int unsigned wrapLength = 1024, const Vector3 &color = Color::White);

    UIImage *AddImage(const std::string &imagePath, const Vector2 &pos, const Vector2 &dims,
                      const Vector3 &color = Color::White);

    void SetSelectedButtonIndex(int index);

    bool IsInteractive() const { return mInteractive; }

protected:
    // Sets the mouse mode to relative or not
    Game *mGame;
    UIFont *mFont;

    // Configure positions
    Vector2 mPos;
    Vector2 mSize;

    // State
    UIState mState;
    bool mInteractive;

    // List of buttons, texts, and images
    int mSelectedButtonIndex;
    std::vector<UIButton *> mButtons;
    std::vector<UIText *> mTexts;
    std::vector<UIImage *> mImages;
};
