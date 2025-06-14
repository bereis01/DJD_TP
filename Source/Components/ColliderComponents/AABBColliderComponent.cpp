#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

AABBColliderComponent::AABBColliderComponent(class Actor *owner, int dx, int dy, int w, int h,
                                             ColliderLayer layer, bool isStatic, int updateOrder)
    : Component(owner, updateOrder)
      , mOffset(Vector2((float) dx, (float) dy))
      , mIsStatic(isStatic)
      , mWidth(w)
      , mHeight(h)
      , mLayer(layer) {
    // Adds collider to game's list of colliders
    mOwner->GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent() {
    // Removes collider to game's list of colliders
    mOwner->GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const {
    return mOwner->GetPosition() + mOffset;
}

Vector2 AABBColliderComponent::GetMax() const {
    return GetMin() + Vector2(mWidth, mHeight);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent &b) const {
    // Gets A's reference points
    Vector2 aMin = GetMin();
    Vector2 aMax = GetMax();

    // Gets B's reference points
    Vector2 bMin = b.GetMin();
    Vector2 bMax = b.GetMax();

    // Checks if there is collision
    if ((aMax.x > bMin.x && aMin.x < bMax.x) && (aMax.y > bMin.y && aMin.y < bMax.y))
        return true;
    return false;
}

float AABBColliderComponent::GetMinVerticalOverlap(AABBColliderComponent *b) const {
    // Gets A's reference points
    Vector2 aMin = GetMin();
    Vector2 aMax = GetMax();

    // Gets B's reference points
    Vector2 bMin = b->GetMin();
    Vector2 bMax = b->GetMax();

    // Vertical intersections
    float top = Math::Abs(bMax.y - aMin.y);
    float bottom = Math::Abs(aMax.y - bMin.y);

    // Returns the smallest
    return top < bottom ? top : -bottom;
}

float AABBColliderComponent::GetMinHorizontalOverlap(AABBColliderComponent *b) const {
    // Gets A's reference points
    Vector2 aMin = GetMin();
    Vector2 aMax = GetMax();

    // Gets B's reference points
    Vector2 bMin = b->GetMin();
    Vector2 bMax = b->GetMax();

    // Vertical intersections
    float right = Math::Abs(aMax.x - bMin.x);
    float left = Math::Abs(bMax.x - aMin.x);

    // Returns the smallest
    return right < left ? -right : left;
}

float AABBColliderComponent::DetectHorizontalCollision(RigidBodyComponent *rigidBody) {
    // Do nothing if static
    if (mIsStatic) return false;

    // Orders the colliders based on the player's movement
    // Code was removed cause we wont use collision. Can be added.

    // Checks collision with all colliders
    auto colliders = mOwner->GetGame()->GetColliders();
    for (auto it = colliders.begin(); it != colliders.end(); it++) {
        AABBColliderComponent *targetCollider = *it;

        // Ignores itself
        if (!targetCollider->IsEnabled() || targetCollider == this)
            continue;

            // Checks the collision
        else if (Intersect(*targetCollider)) {
            // Resolves the collision (change the objects position, hence the rigid body)
            float minOverlap = GetMinHorizontalOverlap(*it);
            ResolveHorizontalCollisions(rigidBody, minOverlap);

            // Callback to do stuff if collided, after separation
            mOwner->OnHorizontalCollision(minOverlap, targetCollider);
            return minOverlap;
        }
    }

    return 0.0f;
}

float AABBColliderComponent::DetectVertialCollision(RigidBodyComponent *rigidBody) {
    // Do nothing if static
    if (mIsStatic) return false;

    // Orders the colliders based on the player's movement
    // Code was removed cause we wont use collision. Can be added.

    // Checks collision with all colliders
    auto colliders = mOwner->GetGame()->GetColliders();
    for (auto it = colliders.begin(); it != colliders.end(); it++) {
        AABBColliderComponent *targetCollider = *it;

        // Ignores itself
        if (!targetCollider->IsEnabled() || targetCollider == this)
            continue;

            // Checks the collision
        else if (Intersect(*targetCollider)) {
            // Resolves the collision (change the objects position, hence the rigid body)
            float minOverlap = GetMinVerticalOverlap(*it);
            ResolveVerticalCollisions(rigidBody, minOverlap);

            // Callback to do stuff if collided, after separation
            mOwner->OnVerticalCollision(minOverlap, targetCollider);
            return minOverlap;
        }
    }

    return 0.0f;
}

void AABBColliderComponent::ResolveHorizontalCollisions(RigidBodyComponent *rigidBody, const float minXOverlap) {
    // Adjusts position to not colliding
    Vector2 newPosition = mOwner->GetPosition();
    newPosition.x += minXOverlap;
    mOwner->SetPosition(newPosition);

    // Adjusts velocity to zero
    Vector2 newVelocity = rigidBody->GetVelocity();
    newVelocity.x = 0;
    rigidBody->SetVelocity(newVelocity);
}

void AABBColliderComponent::ResolveVerticalCollisions(RigidBodyComponent *rigidBody, const float minYOverlap) {
    // Adjusts position to not colliding
    Vector2 newPosition = mOwner->GetPosition();
    newPosition.y += minYOverlap;
    mOwner->SetPosition(newPosition);

    // Adjusts velocity to zero
    Vector2 newVelocity = rigidBody->GetVelocity();
    newVelocity.y = 0;
    rigidBody->SetVelocity(newVelocity);
}
