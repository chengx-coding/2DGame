#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "sprite.h"

class SmartSprite : public Sprite {
public:
    SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
    SmartSprite(const SmartSprite&);
    virtual ~SmartSprite() {
    }

    virtual void update(Uint32 ticks);
    void setPlayerPos(const Vector2f& p) {
        playerPos = p;
    }

    void scaleVelocity(float scale, Uint32 ticks);

private:
    enum MODE {NORMAL, EVADE};
    Vector2f playerPos;
    int playerWidth;
    int playerHeight;
    MODE currentMode;
    float safeDistance;
    Uint32 duration;
    float vScale;

    void goLeft();
    void goRight();
    void goUp();
    void goDown();

    float distance(float x1, float y1, float x2, float y2);

};
#endif
