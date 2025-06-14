#include "../Actors/Actor.h"
#include "RigidBodyComponent.h"
#include "ColliderComponents/AABBColliderComponent.h"

RigidBodyComponent::RigidBodyComponent(class Actor *owner, float mass, float friction, bool applyGravity,
                                       int updateOrder)
    : Component(owner, updateOrder)
      , mMass(mass)
      , mApplyGravity(applyGravity)
      , mApplyFriction(true)
      , mFrictionCoefficient(friction)
      , mVelocity(Vector2::Zero)
      , mAcceleration(Vector2::Zero) {
}

void RigidBodyComponent::ApplyForce(const Vector2 &force) {
    // Applies a force to the acceleration
    mAcceleration += force * (1.f / mMass);
}

void RigidBodyComponent::Update(float deltaTime) {
    // Applies gravity acceleration
    if (mApplyGravity) {
        ApplyForce(Vector2::UnitY * GRAVITY);
    }

    // Applies friction
    if (mApplyFriction && Math::Abs(mVelocity.x) > 0.05f) {
        ApplyForce(Vector2::UnitX * -mFrictionCoefficient * mVelocity.x);
    }

    // Euler Integration (changes velocity according to acceleration)
    mVelocity += mAcceleration * deltaTime;
    mVelocity.x = Math::Clamp<float>(mVelocity.x, -MAX_SPEED_X, MAX_SPEED_X);
    mVelocity.y = Math::Clamp<float>(mVelocity.y, -MAX_SPEED_Y, MAX_SPEED_Y);
    if (Math::NearZero(mVelocity.x, 1.0f)) {
        mVelocity.x = 0.f;
    }

    // Updates position, verifying collision in the process
    auto collider = mOwner->GetComponent<AABBColliderComponent>();

    if (mVelocity.x != 0.0f) {
        mOwner->SetPosition(Vector2(mOwner->GetPosition().x + mVelocity.x * deltaTime,
                                    mOwner->GetPosition().y));

        // If there is a collider, detects horizontal collision
        if (collider) {
            collider->DetectHorizontalCollision(this);
        }
    }

    if (mVelocity.y != 0.0f) {
        mOwner->SetPosition(Vector2(mOwner->GetPosition().x,
                                    mOwner->GetPosition().y + mVelocity.y * deltaTime));

        // If there is a collider, detects horizontal collision
        if (collider) {
            collider->DetectVertialCollision(this);
        }
    }

    // Nullifies acceleration after used
    mAcceleration.Set(0.f, 0.f);
}
