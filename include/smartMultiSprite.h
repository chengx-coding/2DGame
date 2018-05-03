#ifndef SMARTMULTISPRITE__H
#define SMARTMULTISPRITE__H
#include <string>
#include "multisprite.h"
#include "explodingSprite.h"

class SmartMultiSprite : public MultiSprite {
public:
    SmartMultiSprite(const std::string&, const Vector2f &pos, int w, int h);
    // SmartMultiSprite(const SmartMultiSprite &);
    virtual ~SmartMultiSprite();
    SmartMultiSprite(const SmartMultiSprite&) = delete;
    SmartMultiSprite& operator=(const SmartMultiSprite&) = delete;

    virtual void draw() const;
    virtual void update(Uint32 ticks);

    void setPlayerPos(const Vector2f& p) {
        playerPos = p;
    }

    void scaleVelocity(float scale, Uint32 ticks);

    void explode();
    bool isExplode() const {
        return explosion;
    }

private:
    enum MODE {NORMAL, EVADE};
    Vector2f playerPos;
    int playerWidth;
    int playerHeight;
    MODE currentMode;
    float safeDistance;
    Uint32 duration;
    float vScale;

    ExplodingSprite* explosion;

    void goLeft();
    void goRight();
    void goUp();
    void goDown();

    float distance(float x1, float y1, float x2, float y2);

};
#endif
