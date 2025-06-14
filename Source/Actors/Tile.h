#pragma once
#include <string>
#include "Actor.h"

class Tile : public Actor {
public:
    Tile(Game *game, const std::string &texturePath, bool isStatic = true);

    void OnUpdate(float deltaTime) override;

private:
    AABBColliderComponent *mColliderComponent;
    RigidBodyComponent *mRigidBodyComponent;
};
