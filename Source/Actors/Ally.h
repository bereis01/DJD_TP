#pragma once
#include "Unit.h"
#include "../Components/DrawComponents/DrawAOEComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

class Ally : public Unit {
public:
    Ally(Game *game, const std::string &unitType, Stats stats);

    ~Ally();

    void OnUpdate(float deltaTime);

private:
    // Draw components for AOE fields
    DrawAOEComponent *mMovementAOE;
    DrawAOEComponent *mAttackAOE;
    DrawPolygonComponent *mTypeIndicator;

    // Life bar
    DrawPolygonComponent *mRedBar;
    DrawPolygonComponent *mGreenBar;
};
