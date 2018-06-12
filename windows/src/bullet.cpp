#include <cmath>
#include <random>
#include <functional>
#include "bullet.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"
#include "sound.h"

float Bullet::distance(float x1, float y1, float x2, float y2) {
    float x = x1-x2;
    float y = y1-y2;
    return hypot(x, y);
}

Bullet::Bullet(const std::string& name,
               const Vector2f& pos,
               const Vector2f& t,
               float vel) :
    MultiSprite(name, pos),
    playerPos(pos),
    target(t),
    velocity(vel),
    activated(true),
    explosion(nullptr)
{
    setTarget(t);
    setVelocityToTarget(vel);
}

// Bullet::Bullet(const Bullet& b) :
//     MultiSprite(b),
//     playerPos(b.playerPos),
//     target(b.target),
//     velocity(b.velocity),
//     activated(b.activated),
//     explosion(b.explosion)
// {
// }


Bullet::~Bullet() {
    if(explosion) delete explosion;
}

void Bullet::setTarget(const Vector2f& t) {
    target = t;
    if(target[0] > worldWidth - getScaledWidth()) {
        target[0] = worldWidth - getScaledWidth();
    }
    if(target[0] < 0) {
        target[0] = 0;
    }
    if(target[1] > worldWidth - getScaledHeight()) {
        target[1] = worldWidth - getScaledHeight();
    }
    if(target[1] < 0) {
        target[1] = 0;
    }
    // std::cout << "target: " << target << '\n';
}

void Bullet::setVelocityToTarget(float v) {
    velocity = v;
    float d = distance(getX(), getY(), target[0], target[1]);
    float vx = (target[0] - getX()) / d * velocity;
    float vy = (target[1] - getY()) / d * velocity;
    setVelocity(Vector2f(vx, vy));
}

void Bullet::reset() {
    activated = true;
}

void Bullet::explode() {
    if ( !explosion ) {
        Sprite sprite(getName(),
                      getPosition(), getVelocity(),
                      images[currentFrame]);
        explosion = new ExplodingSprite(sprite);
        SDLSound::getInstance()[3];
    }
}

void Bullet::draw() const {
    //std::cout << "bullet draw" << '\n';
    if ( explosion ) {
        explosion->draw();
        return;
    }
    MultiSprite::draw();
}

void Bullet::update(Uint32 ticks) {
    if ( explosion ) {
        explosion->update(ticks);
        if ( explosion->chunkCount() == 0 ) {
            delete explosion;
            explosion = NULL;
            activated = false;
            //reset();
        }
        return;
    }

    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    Vector2f pos = getPosition();
    if(distance(pos[0], pos[1], target[0], target[1]) <=
       distance(incr[0], incr[1], 0, 0)) {
        activated = false;
        return;
    }

    MultiSprite::update(ticks);
}
