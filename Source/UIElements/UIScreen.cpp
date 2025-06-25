#include "UIScreen.h"
#include "../Game.h"

UIScreen::UIScreen(Game *game, const std::string &fontName)
    : mGame(game)
      , mPos(0.f, 0.f)
      , mSize(0.f, 0.f)
      , mState(UIState::Active)
      , mSelectedButtonIndex(-1) {
    // Adds the screen to the game's screen list
    mGame->PushUI(this);

    // Loads the font to be used
    mFont = mGame->LoadFont(fontName);
}

UIScreen::~UIScreen() {
    // Cleans allocated texts
    for (auto text: mTexts)
        delete text;
    mTexts.clear();

    // Cleans allocated buttons
    for (auto button: mButtons)
        delete button;
    mButtons.clear();

    // Cleans allocated images
    for (auto image: mImages)
        delete image;
    mImages.clear();
}

void UIScreen::Update(float deltaTime) {
}

void UIScreen::Draw(SDL_Renderer *renderer) {
    // Draws texts
    for (auto text: mTexts)
        text->Draw(renderer, mPos);

    // Draws buttons
    for (auto button: mButtons)
        button->Draw(renderer, mPos);

    // Draws images
    for (auto image: mImages)
        image->Draw(renderer, mPos);
}

void UIScreen::ProcessInput(const uint8_t *keys) {
}

void UIScreen::HandleKeyPress(int key) {
    switch (key) {
        default:
            break;
    }
}

void UIScreen::Close() {
    mState = UIState::Closing;
}

UIText *UIScreen::AddText(const std::string &name, const Vector2 &pos, const Vector2 &dims, const int pointSize,
                          const int unsigned wrapLength, const Vector3 &color) {
    // Creates the text and stores it in the respective attribute
    UIText *text = new UIText(name, mFont, pointSize, wrapLength, pos, dims, color);
    mTexts.emplace_back(text);
    return text;
}

UIButton *UIScreen::AddButton(const std::string &name, const Vector2 &pos, const Vector2 &dims,
                              std::function<void()> onClick) {
    // Creates a new button and stores it in the respective attribute
    UIButton *button = new UIButton(name, mFont, onClick, pos, dims, Vector3(30, 30, 30), 40, 1024, Vector2::Zero,
                                    Vector2(260, 20));
    mButtons.emplace_back(button);

    // Initializes the index if list is being first populated
    if (mButtons.size() == 1) {
        mSelectedButtonIndex = 0;
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    }

    return button;
}

UIImage *UIScreen::AddImage(const std::string &imagePath, const Vector2 &pos, const Vector2 &dims,
                            const Vector3 &color) {
    // Creates the image and stores it in the respective attribute
    UIImage *image = new UIImage(mGame->GetRenderer(), imagePath, pos, dims, color);
    mImages.emplace_back(image);
    return image;
}
