#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
// #include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "infoHUD.h"
#include "sound.h"
#include "menuEngine.h"

class CollisionStrategy;
class SmartSprite;
class SmartMultiSprite;
class Bullet;
class Player;
class MultiSprite;

class Engine {
public:
    Engine ();
    ~Engine ();
    bool play();
    // void switchSprite();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    const RenderContext* rc;
    const IOMod& io;
    Clock& clock;

    SDL_Renderer * const renderer;
    World ground;
    World mountain;
    World sky;
    Viewport& viewport; //note: &

    Player* player;
    std::vector<SmartMultiSprite*> clouds;
    std::vector<SmartMultiSprite*> explodedClouds;
    //std::vector<SmartMultiSprite*> swords;
    //std::vector<Bullet*> clouds;
    std::vector<Bullet*> swords;

    std::vector<MultiSprite*> cloudsLayer1;
    std::vector<MultiSprite*> cloudsLayer2;

    std::vector<CollisionStrategy*> strategies;
    int currentStrategy;
    bool collision;
    bool makeVideo;
    bool godmodeOn;

    bool hudOn;
    HUD& hud; //note: &
    InfoHUD& infoHUD; //note: &
    SDLSound& sound;

    MenuEngine menuEngine;

    void draw() const;
    void update(Uint32);
    void checkEnd() const;
    //void printScales() const;
    void checkForCollisions();
    void checkForExplosions();
};
