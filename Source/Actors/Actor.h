#pragma once
#include <vector>
#include <SDL2/SDL_stdinc.h>
#include "../Utils/Math.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

enum class ActorState {
    Active,
    Paused,
    Destroy
};

class Actor {
public:
    Actor(Game *game);

    virtual ~Actor();

    // Update function called from Game (not overridable)
    void Update(float deltaTime);

    // ProcessInput function called from Game (not overridable)
    void ProcessInput(const Uint8 *keyState);

    // HandleKeyPress function called from Game (not overridable)
    void HandleKeyPress(const int key, const bool isPressed);

    // Position getter/setter
    const Vector2 &GetPosition() const { return mPosition; }
    void SetPosition(const Vector2 &pos) { mPosition = pos; }

    Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

    // Scale getter/setter
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; }

    // Rotation getter/setter
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; }

    // State getter/setter
    ActorState GetState() const { return mState; }
    void SetState(ActorState state) { mState = state; }

    // Game getter
    class Game *GetGame() { return mGame; }

    // Returns component of type T, or null if doesn't exist
    template<typename T>
    T *GetComponent() const {
        for (auto c: mComponents) {
            T *t = dynamic_cast<T *>(c);
            if (t != nullptr) {
                return t;
            }
        }

        return nullptr;
    }

    // Game specific

    // Any actor-specific collision code (overridable)
    virtual void OnHorizontalCollision(float minOverlap, AABBColliderComponent *other);

    virtual void OnVerticalCollision(float minOverlap, AABBColliderComponent *other);

    virtual void Kill();

protected:
    Game *mGame;

    // Any actor-specific update code (overridable)
    virtual void OnUpdate(float deltaTime);

    virtual void OnProcessInput(const Uint8 *keyState);

    virtual void OnHandleKeyPress(const int key, const bool isPressed);

    // Actor's state
    ActorState mState;

    // Transform
    Vector2 mPosition;
    float mScale;
    float mRotation;

    // Components
    std::vector<Component *> mComponents;

    // Game specific

private:
    friend class Component;

    // Adds component to Actor (this is automatically called
    // in the component constructor)
    void AddComponent(Component *c);
};
