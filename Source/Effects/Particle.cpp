#include "Particle.h"

Particle::Particle(Game *game, SDL_Texture *texture, const Vector2 &size) : Actor(game) {
    // Loads draw component for the texture
    mDrawComponent = new DrawTextureComponent(this, texture, size.x, size.y, 300);

    // Loads rigid body for movement
    // Applies some upward movement
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f, false);
    mRigidBodyComponent->ApplyForce(Vector2(0.0f, -1000.0f));
}

void Particle::OnUpdate(float deltaTime) {
    // Updates timer and deletes itself if it has finished
    if (mLiveTimer <= 0)
        mState = ActorState::Destroy;
    else
        mLiveTimer -= deltaTime;
}
