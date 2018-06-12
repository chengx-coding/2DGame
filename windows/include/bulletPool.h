#ifndef BULLETPOOL__H
#define BULLETPOOL__H
#include <iostream>
#include <string>
#include <list>
#include "vector2f.h"
#include "drawable.h"

class Bullet;

class BulletPool {
public:
    BulletPool(const std::string&);
    ~BulletPool();
    BulletPool(const BulletPool&) = delete;
    BulletPool& operator=(const BulletPool&) = delete;

    std::list<Bullet*>& getBulletList() {
        return bulletList;
    }
    std::list<Bullet*>& getFreeList() {
        return freeList;
    }

    void shoot(const Vector2f&, const Vector2f&, float);

    void update(Uint32);
    void draw() const;
private:
    std::string name;
    std::list<Bullet*> bulletList;
    std::list<Bullet*> freeList;
};

#endif
