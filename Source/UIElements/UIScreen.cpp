// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "../Game.h"
#include "UIFont.h"
#include "../Actors/Unit.h"

UIScreen::UIScreen(Game* game, const std::string& fontName, bool isInteractive)
	:mGame(game)
	,mPos(0.f, 0.f)
	,mSize(0.f, 0.f)
	,mState(UIState::Active)
    ,mSelectedButtonIndex(-1)
    ,mInteractive(isInteractive)
{
    mFont = mGame->LoadFont(fontName);
}

UIScreen::~UIScreen()
{
    for (auto text : mTexts) {
        delete text;
    }
    mTexts.clear();

    for (auto button : mButtons) {
        delete button;
    }
    mButtons.clear();

    for (auto image : mImages) {
        delete image;
    }
    mImages.clear();
}

void UIScreen::Update(float deltaTime)
{
	
}

void UIScreen::Draw(SDL_Renderer *renderer)
{

    for (auto image : mImages) {
        image->Draw(renderer, mPos);
    }

    for (auto button : mButtons) {
        button->Draw(renderer, mPos);
    }

    for (auto text : mTexts) {
        text->Draw(renderer, mPos);
    }
}

void UIScreen::ProcessInput(const uint8_t* keys)
{

}

void UIScreen::HandleKeyPress(int key)
{
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
        }
    } else if (key == SDLK_b) {
        mGame->GetUIStack().pop_back();
        if (mGame->GetGamePlayState() == Game::GamePlayState::ChoosingAction) {
            mGame->SetGamePlayState(Game::GamePlayState::UnitSelected);
            mGame->GetSelectedUnit()->SetPosition(mGame->GetSelectedUnit()->GetOldPosition());
            SetSelectedButtonIndex(0);
        }
    }
}

void UIScreen::Close()
{
	mState = UIState::Closing;
}

UIText* UIScreen::AddText(const std::string &name, const Vector2 &pos, const Vector2 &dims, const int pointSize, const int unsigned wrapLength)
{
    UIText *t = new UIText(name, mFont, pointSize, wrapLength, pos, dims);
    mTexts.emplace_back(t);
    return t;
}

UIButton* UIScreen::AddButton(const std::string& name, const Vector2 &pos, const Vector2& dims, std::function<void()> onClick, SDL_Renderer* renderer)
{
    auto *b = new UIButton(name, mFont, onClick, pos, dims, renderer);
    mButtons.emplace_back(b);

    if (mButtons.size() == 1) {
        mSelectedButtonIndex = 0;
        b->SetHighlighted(true);
    }
    return b;
}

UIImage* UIScreen::AddImage(const std::string &imagePath, const Vector2 &pos, const Vector2 &dims, SDL_Renderer* renderer, const Vector3 &color)
{
    auto *img = new UIImage(imagePath, renderer, pos, dims, color);
    mImages.emplace_back(img);
    return img;
}

void UIScreen::SetSelectedButtonIndex(int index)
{
    mButtons[mSelectedButtonIndex]->SetHighlighted(false);
    mSelectedButtonIndex = index;
    mButtons[mSelectedButtonIndex]->SetHighlighted(true);
}