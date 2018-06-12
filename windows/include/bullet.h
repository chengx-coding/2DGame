#ifndef BULLET__H
#define BULLET__H
#include <string>
#include "multisprite.h"

class ExplodingSprite;

class Bullet : public MultiSprite {
public:
    Bullet(const std::string&, const Vector2f&, const Vector2f&, float);
    ~Bullet();
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;


    virtual void update(Uint32 ticks);
    virtual void draw() const;

    void setPlayerPos(const Vector2f& p) {
        playerPos = p;
    }

    void setTarget(const Vector2f& t);
    void setVelocityToTarget(float v);

    bool isActivated() {
        return activated;
    }

    bool isExplode() const {
        return explosion;
    }

    void reset();
    void explode();

private:
    Vector2f playerPos;
    Vector2f target;
    float velocity;
    bool activated;
    ExplodingSprite* explosion;

    float distance(float x1, float y1, float x2, float y2);

    //Bullet& operator=(const Bullet&);

};
#endif
