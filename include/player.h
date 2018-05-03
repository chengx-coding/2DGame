#ifndef PLAYER__H
#define PLAYER__H

#include <SDL.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include "drawable.h"
#include "vector2f.h"
#include "bulletPool.h"
#include "sound.h"

class ExplodingSprite;

class Player : public Drawable {
public:
    Player(const std::string&, const std::string&);
    // Player(const Player&);
    virtual ~Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    virtual void draw() const;
    virtual void update(Uint32 ticks);

    virtual const Image* getImage() const {
        return walkFrames[currentFrame];
    }
    int getScaledWidth()  const {
        return getScale()*walkFrames[currentFrame]->getWidth();
    }
    int getScaledHeight()  const {
        return getScale()*walkFrames[currentFrame]->getHeight();
    }
    // const Vector2f getCenterPosition() const {
    //     return
    // }
    BulletPool& getBullets() {
        return bullets;
    }
    virtual const SDL_Surface* getSurface() const {
        return walkFrames[currentFrame]->getSurface();
    }
    bool isRight() const {
        return ((motionState & 1) == 0);
    }
    bool isJump() const {
        return (motionState & 4);
    }
    bool isCrouch() const {
        return (motionState & 8) && (!isJump());
    }
    bool isWalk() const {
        return (motionState & 2) && (!isCrouch());
    }
    bool isIdle() const {
        return (!isWalk());
    }
    bool isShoot() const {
        return (motionState & 16);
    }
    bool isExplode() const {
        return explosion;
    }

    void setMotion(int s) {
        motionState = s;
    }

    void right();
    void left();
    void up();
    void down();
    void jump();
    void crouch();
    void stop();

    void shoot();

    void explode();
    void reset();


    void attach( Drawable* o ) {
        observers.push_back(o);
    }
    void detach( Drawable* o );


private:
    std::vector<Image *> walkFrames;
    Image* idleFrame;
    Image* jumpFrame;
    Image* crouchFrame;
    Image* shootFrame;
    Image* jumpShootFrame;
    // 0b0000 (shoot, crouch, jump, walk, oirentation)
    Uint8 motionState;
    Uint8 lastMotionState;

    unsigned currentFrame;
    unsigned numberOfWalkFrames;
    unsigned frameInterval;
    unsigned shootInterval;
    float timeSinceLastFrame;
    float currentShootTime;
    int worldWidth;
    int worldHeight;

    Vector2f initialVelocity;
    Vector2f initialPos;
    float jumpHeight;
    float acceleration;
    std::list<Drawable*> observers;
    ExplodingSprite* explosion;
    std::string bulletName;
    BulletPool bullets;
    SDLSound& sound;

    void advanceFrame(Uint32 ticks);
    void advanceShoot(Uint32 ticks);

    // Player& operator=(const Player&);
};

#endif
