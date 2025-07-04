#pragma once
#include "../Component.h"
#include "../../Utils/Math.h"
#include "../RigidBodyComponent.h"

enum class ColliderLayer {
    Player,
    Enemy,
    Blocks
};

class AABBColliderComponent : public Component {
public:
    AABBColliderComponent(class Actor *owner, int dx, int dy, int w, int h,
                          ColliderLayer layer, bool isStatic = false, int updateOrder = 10);

    ~AABBColliderComponent();

    // Detects intersection and collision
    bool Intersect(const AABBColliderComponent &b) const;

    float DetectHorizontalCollision(RigidBodyComponent *rigidBody);

    float DetectVertialCollision(RigidBodyComponent *rigidBody);

    // Gets edges of hit box
    Vector2 GetMin() const;

    Vector2 GetMax() const;

    // Returns the corresponding layer
    ColliderLayer GetLayer() const { return mLayer; }

private:
    // Calculates minimum overlay between hitboxes
    float GetMinVerticalOverlap(AABBColliderComponent *b) const;

    float GetMinHorizontalOverlap(AABBColliderComponent *b) const;

    // Resolves collision between elements
    void ResolveHorizontalCollisions(RigidBodyComponent *rigidBody, float minOverlap);

    void ResolveVerticalCollisions(RigidBodyComponent *rigidBody, float minOverlap);

    // Attributes
    Vector2 mOffset;
    int mWidth;
    int mHeight;
    bool mIsStatic;

    ColliderLayer mLayer;
};
