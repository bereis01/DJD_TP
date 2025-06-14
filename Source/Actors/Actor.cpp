#include "Actor.h"
#include "../Game.h"
#include "../Components/Component.h"
#include <algorithm>

Actor::Actor(Game *game)
    : mState(ActorState::Active)
      , mPosition(Vector2::Zero)
      , mScale(1.0f)
      , mRotation(0.0f)
      , mGame(game) {
    // Adds to the game
    mGame->AddActor(this);
}

Actor::~Actor() {
    // Removes from the game
    mGame->RemoveActor(this);

    // Deallocates components
    for (auto component: mComponents) {
        delete component;
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime) {
    // Only updates if active
    if (mState == ActorState::Active) {
        // Updates all components
        for (auto comp: mComponents) {
            if (comp->IsEnabled()) {
                comp->Update(deltaTime);
            }
        }

        // Updates itself
        OnUpdate(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime) {
}

void Actor::OnHorizontalCollision(const float minOverlap, AABBColliderComponent *other) {
}

void Actor::OnVerticalCollision(const float minOverlap, AABBColliderComponent *other) {
}

void Actor::Kill() {
}

void Actor::ProcessInput(const Uint8 *keyState) {
    // Only processes input if active
    if (mState == ActorState::Active) {
        // Processes input for all components
        for (auto comp: mComponents) {
            comp->ProcessInput(keyState);
        }

        // Processes own input
        OnProcessInput(keyState);
    }
}

void Actor::HandleKeyPress(const int key, const bool isPressed) {
    // Only processes key presses if active
    if (mState == ActorState::Active) {
        // Processes key presses for all components
        for (auto comp: mComponents) {
            //comp->HandleKeyPress(key, isPressed);
        }

        // Processes own key press
        OnHandleKeyPress(key, isPressed);
    }
}

void Actor::OnProcessInput(const Uint8 *keyState) {
}

void Actor::OnHandleKeyPress(const int key, const bool isPressed) {
}

void Actor::AddComponent(Component *c) {
    mComponents.emplace_back(c);
    std::sort(mComponents.begin(), mComponents.end(), [](Component *a, Component *b) {
        return a->GetUpdateOrder() < b->GetUpdateOrder();
    });
}
