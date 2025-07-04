#pragma once
#include "../Game.h"
#include <SDL2/SDL_stdinc.h>

class Component {
public:
    // Constructor (the lower the update order, the earlier the component updates)
    Component(Actor *owner, int updateOrder = 100);

    // Destructor
    ~Component();

    // Update this component by delta time
    virtual void Update(float deltaTime);

    // Process input for this component (if needed)
    virtual void ProcessInput(const Uint8 *keyState);

    int GetUpdateOrder() const { return mUpdateOrder; }
    Actor *GetOwner() const { return mOwner; }

    Game *GetGame() const;

    void SetEnabled(const bool enabled) { mIsEnabled = enabled; };
    bool IsEnabled() const { return mIsEnabled; };

protected:
    // Owning actor
    Actor *mOwner;

    // Update order
    int mUpdateOrder;

    bool mIsEnabled;
};
