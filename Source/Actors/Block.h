#pragma once
#include <string>
#include "Actor.h"

class Block : public Actor {
public:
    Block(Game *game, const std::string &texturePath, bool isStatic = true);

    void OnUpdate(float deltaTime) override;

private:
    AABBColliderComponent *mColliderComponent;
    RigidBodyComponent *mRigidBodyComponent;
};
