#include <cmath>
#include <random>
#include <functional>
#include "smartMultiSprite.h"
#include "gamedata.h"
#include "renderContext.h"

float SmartMultiSprite::distance(float x1, float y1, float x2, float y2) {
    float x = x1-x2;
    float y = y1-y2;
    return hypot(x, y);
}

void SmartMultiSprite::goLeft()  {
    setVelocityX( -abs(getVelocityX()) );
}
void SmartMultiSprite::goRight() {
    setVelocityX( fabs(getVelocityX()) );
}
void SmartMultiSprite::goUp()    {
    setVelocityY( -fabs(getVelocityY()) );
}
void SmartMultiSprite::goDown()  {
    setVelocityY( fabs(getVelocityY()) );
}


SmartMultiSprite::SmartMultiSprite(const std::string& name, const Vector2f& pos,
                                   int w, int h) :
    MultiSprite(name, 50),
    playerPos(pos),
    playerWidth(w),
    playerHeight(h),
    currentMode(NORMAL),
    safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
    duration(0),
    vScale(1),
    explosion()
{
}


// SmartMultiSprite::SmartMultiSprite(const SmartMultiSprite& s) :
//     MultiSprite(s),
//     playerPos(s.playerPos),
//     playerWidth(s.playerWidth),
//     playerHeight(s.playerHeight),
//     currentMode(s.currentMode),
//     safeDistance(s.safeDistance),
//     duration(s.duration),
//     vScale(s.vScale)
// {
// }

SmartMultiSprite::~SmartMultiSprite() {
    if(explosion) delete explosion;
}


void SmartMultiSprite::scaleVelocity(float scale, Uint32 ticks) {
    vScale = scale;
    setVelocity( getVelocity() * vScale );
    duration = ticks;
}

void SmartMultiSprite::explode() {
    if ( !explosion ) {
        Sprite sprite(getName(),
                      getPosition(), getVelocity(),
                      getImage());
        explosion = new ExplodingSprite(sprite);
    }
}

void SmartMultiSprite::draw() const {
    if ( explosion ) {
        explosion->draw();
        return;
    }
    MultiSprite::draw();
}

void SmartMultiSprite::update(Uint32 ticks) {
    if ( explosion ) {
        explosion->update(ticks);
        if ( explosion->chunkCount() == 0 ) {
            delete explosion;
            explosion = NULL;
            setPosition(Vector2f(0,0));
        }
        return;
    }

    MultiSprite::update(ticks);
    float x= getX()+getImage()->getWidth()/2;
    float y= getY()+getImage()->getHeight()/2;
    float ex= playerPos[0]+playerWidth/2;
    float ey= playerPos[1]+playerHeight/2;
    float distanceToEnemy = distance( x, y, ex, ey );

    if  ( currentMode == NORMAL ) {
        if(distanceToEnemy < safeDistance) {
            currentMode = EVADE;
            if(fabs(vScale - 1) <= 0.0001 ) {
                scaleVelocity(
                    Gamedata::getInstance().getXmlFloat(getName()+"/speedScale"),
                    Gamedata::getInstance().getXmlInt(getName()+"/speedScaleTicks"));
            }
        }
    }
    else if  ( currentMode == EVADE ) {
        if(distanceToEnemy > safeDistance) currentMode = NORMAL;
        else {
            if ( x < ex ) goLeft();
            if ( x > ex ) goRight();
            if ( y < ey ) goUp();
            if ( y > ey ) goDown();
        }
    }
    if(duration > 0) {
        if(duration <= ticks) {
            duration = 0;
            setVelocity(getVelocity() * (1 / vScale));
            vScale = 1;
        }
        else {
            duration -= ticks;
        }
    }
}
