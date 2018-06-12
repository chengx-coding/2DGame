#include "bulletPool.h"
#include "bullet.h"
#include "infoHUD.h"


BulletPool::BulletPool(const std::string& n) :
    name(n),
    bulletList(),
    freeList()
{
}

BulletPool::~BulletPool() {
    for(auto bullet : bulletList) {
        delete bullet;
    }
    for(auto bullet : freeList) {
        delete bullet;
    }
}

void BulletPool::shoot(const Vector2f& pos,
                       const Vector2f& target,
                       float vel) {
    if(freeList.empty()) {
        bulletList.push_back(new Bullet(name, pos, target, vel));
        //std::cout << "freeList is empty" << std::endl;
    }
    else {
        //std::cout << "freeList isn't empty" << std::endl;
        Bullet* bullet = freeList.front();
        freeList.pop_front();
        bullet->reset();
        bullet->setTarget(target);
        //bullet.setVelocity(vel);
        bullet->setPosition(pos);
        bullet->setVelocityToTarget(vel);
        bulletList.push_back(bullet);
    }
}

void BulletPool::update(Uint32 ticks) {
    std::list<Bullet*>::iterator iter = bulletList.begin();
    while(iter != bulletList.end()) {
        (*iter)->update(ticks);
        if((*iter)->isActivated() == false) {
            freeList.push_back(*iter);
            iter = bulletList.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

void BulletPool::draw() const {
    for(auto bullet : bulletList) {
        bullet->draw();
    }
    InfoHUD::getInstance().addLine("Free list: " +
                                   std::to_string(freeList.size()));
    InfoHUD::getInstance().addLine("Bullet list: " +
                                   std::to_string(bulletList.size()));

}
