#pragma once
#include <SDL.h>
#include "../Component.h"

class DrawComponent : public Component {
public:
    // (Lower draw order corresponds with further back)
    DrawComponent(Actor *owner, int drawOrder = 100);

    virtual ~DrawComponent();

    virtual void Draw(SDL_Renderer *renderer);

    bool IsVisible() const { return mIsVisible; }
    void SetIsVisible(const bool isVisible) { mIsVisible = isVisible; }

    int GetDrawOrder() const { return mDrawOrder; }

protected:
    bool mIsVisible;
    int mDrawOrder;
};
