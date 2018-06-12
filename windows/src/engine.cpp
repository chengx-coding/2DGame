#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "sprite.h"
#include "multisprite.h"
// #include "smartSprite.h"
#include "smartMultiSprite.h"
#include "bullet.h"
#include "player.h"
#include "collisionStrategy.h"


Engine::~Engine() {
    delete player;
    for(auto cloud : clouds) {
        delete cloud;
    }
    // for(auto sword : swords) {
    //     delete sword;
    // }
    for(auto cloud : explodedClouds) {
        delete cloud;
    }
    for(CollisionStrategy* strategy : strategies) {
        delete strategy;
    }

    for(MultiSprite* cloud : cloudsLayer1) {
        delete cloud;
    }

    for(MultiSprite* cloud : cloudsLayer2) {
        delete cloud;
    }

    std::cout << "Game End" << std::endl;
}

Engine::Engine() :
    rc( RenderContext::getInstance() ),
    io( IOMod::getInstance() ),
    clock( Clock::getInstance() ),
    renderer( rc->getRenderer() ),
    ground("Ground", Gamedata::getInstance().getXmlInt("Ground/factor") ),
    mountain("Mountain", Gamedata::getInstance().getXmlInt("Mountain/factor") ),
    sky("Sky", Gamedata::getInstance().getXmlInt("Sky/factor") ),
    viewport( Viewport::getInstance() ),
    player( new Player("XCheng", "Sword") ),
    clouds(),
    explodedClouds(),
    swords(),
    cloudsLayer1(),
    cloudsLayer2(),
    strategies(),
    currentStrategy( 1 ),
    collision( false ),
    makeVideo( false ),
    godmodeOn( false ),
    hudOn( true ),
    hud( HUD::getInstance() ),
    infoHUD( InfoHUD::getInstance() ),
    sound(SDLSound::getInstance())
    ,menuEngine()
{
    clock.startClock();
    Vector2f pos = player->getPosition();
    int w = player->getScaledWidth();
    int h = player->getScaledHeight();

    int n = Gamedata::getInstance().getXmlInt("Cloud/number");
    for(int i = 0; i < n; i++) {
        clouds.push_back(new SmartMultiSprite("Cloud", pos, w, h));
        player->attach(clouds[i]);
    }

    for(int i = 0; i < 4 * n; i++)
    {
        MultiSprite* cloud1 = new MultiSprite("Cloud", 50);
        float scale = Gamedata::getInstance().getRandFloat(0.2, 0.4);
        cloud1->setScale(scale);
        cloud1->setVelocity(scale * cloud1->getVelocity());
        cloudsLayer1.push_back(cloud1);
    }

    for(int i = 0; i < 1 * n; i++)
    {
        MultiSprite* cloud2 = new MultiSprite("Cloud", 50);
        float scale = Gamedata::getInstance().getRandFloat(0.4, 0.6);
        cloud2->setScale(scale);
        cloud2->setVelocity(scale * cloud2->getVelocity());
        cloudsLayer2.push_back(cloud2);
    }

    strategies.push_back(new RectangularCollisionStrategy);
    strategies.push_back(new PerPixelCollisionStrategy);
    strategies.push_back(new MidPointCollisionStrategy);

    Viewport::getInstance().setObjectToTrack(player);
    std::cout << "Loading complete" << std::endl;
}

void Engine::checkEnd() const {
    if ( clouds.size() == 0 && explodedClouds.size() == 0) {
        SDL_Rect rect = {400, 300, 480, 120};
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 200 );
        SDL_RenderFillRect( renderer, &rect );
        io.writeText("YOU WIN !", 570, 320,
                     {0, 0, 255, 255});
        io.writeText("Press R to Restart the game", 480, 380,
                     {0, 128, 128, 255});
        clock.pause();
    }
    else if (collision && !player->isExplode()) {
        SDL_Rect rect = {400, 300, 480, 120};
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 200 );
        SDL_RenderFillRect( renderer, &rect );
        io.writeText("YOU LOSE !", 570, 320,
                     {0, 255, 0, 255});
        io.writeText("Press R to Restart the game", 480, 380,
                     {0, 128, 128, 255});
        clock.pause();
    }
}

void Engine::draw() const {
    infoHUD.addLine("Remaining enemies: " + std::to_string(clouds.size()));
    infoHUD.addLine("God mode: " +
                    (godmodeOn ? std::string("On") : std::string("Off")));

    sky.draw();

    for(auto cloud : cloudsLayer1) {
        cloud->draw();
    }

    mountain.draw();

    for(auto cloud : cloudsLayer2) {
        cloud->draw();
    }

    ground.draw();

    if(!(collision && !player->isExplode())) {
        player->draw();
    }

    for(auto cloud : clouds) {
        cloud->draw();
    }
    for(auto cloud : explodedClouds) {
        cloud->draw();
    }

    viewport.draw();
    strategies[currentStrategy]->draw();

    if(hudOn) {
        hud.draw();
        infoHUD.draw();
    }

    checkEnd();

    SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
    auto it = clouds.begin();
    while ( it != clouds.end() ) {
        if ( strategies[currentStrategy]->execute(*player, **it) ) {
            if(godmodeOn) break;
            if(!(player->isCrouch()) && !((*it)->isExplode())) {
                player->explode();
                collision = true;
                break;
            }
            else it++;
        }
        else it++;
    }
    std::list<Bullet*>& bulletList = player->getBullets().getBulletList();

    auto it1 = clouds.begin();
    bool cloudDestroyed = false;
    while ( it1 != clouds.end() ) {
        //std::cout << "- it1: " << static_cast<void*>(*it1) << '\n';
        cloudDestroyed = false;
        auto it2 = bulletList.begin();
        while ( it2 != bulletList.end() ) {
            //std::cout << "it2: " << static_cast<void*>(*it2) << '\n';
            if ( strategies[currentStrategy]->execute(**it1, **it2) ) {
                if(!((*it1)->isExplode()) && !((*it2)->isExplode())) {
                    (*it2)->explode();
                    (*it1)->explode();

                    player->detach(*it1);
                    //delete (*it1);
                    explodedClouds.push_back(*it1);
                    it1 = clouds.erase(it1);

                    cloudDestroyed = true;
                    break;
                }
                //else it2++;
            }
            //else it2++;
            it2++;
        }
        if(!cloudDestroyed) it1++;
    }
}

void Engine::checkForExplosions() {
    auto iter = explodedClouds.begin();
    while(iter != explodedClouds.end()) {
        if(!(*iter)->isExplode()) {
            delete (*iter);
            iter = explodedClouds.erase(iter);
        }
        else iter++;
    }
}

void Engine::update(Uint32 ticks) {
    checkForCollisions();

    player->update(ticks);

    for(auto cloud : clouds) {
        cloud->update(ticks);
    }
    for(auto cloud : explodedClouds) {
        cloud->update(ticks);
    }
    checkForExplosions();

    for(auto cloud : cloudsLayer1) {
        cloud->update(ticks);
    }
    for(auto cloud : cloudsLayer2) {
        cloud->update(ticks);
    }

    ground.update();
    mountain.update();
    sky.update();

    viewport.update(); // always update viewport last
    hud.update();
    infoHUD.update();
}


bool Engine::play() {
    SDL_Event event;
    const Uint8* keystate;
    bool done = false;
    Uint32 ticks = clock.getElapsedTicks();
    FrameGenerator frameGen;

    while ( !done ) {
        // The next loop polls for events, guarding against key bounce:
        while ( SDL_PollEvent(&event) ) {
            keystate = SDL_GetKeyboardState(NULL);
            if (event.type ==  SDL_QUIT) { done = true; break; }
            if(event.type == SDL_KEYDOWN) {
                if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
                    done = true;
                    break;
                }
                if ( keystate[SDL_SCANCODE_P] ) {
                    if ( clock.isPaused() ) clock.unpause();
                    else clock.pause();
                }
                if ( keystate[SDL_SCANCODE_R] ) {
                    clock.unpause();
                    return true;
                }
                if ( keystate[SDL_SCANCODE_C] ) {
                    currentStrategy = (1 + currentStrategy) % strategies.size();
                }
                if ( keystate[SDL_SCANCODE_M] ) {
                    clock.pause();
                    menuEngine.play();
                    clock.unpause();
                }
                if (keystate[SDL_SCANCODE_G]) {
                    godmodeOn = !godmodeOn;
                    player->crouch();
                }
                if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
                    std::cout << "Initiating frame capture" << std::endl;
                    makeVideo = true;
                }
                else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
                    std::cout << "Terminating frame capture" << std::endl;
                    makeVideo = false;
                }
                if (keystate[SDL_SCANCODE_F1]) {
                    hudOn = !hudOn;
                }
                if (keystate[SDL_SCANCODE_SPACE]) {
                    player->shoot();
                }
                if (keystate[SDL_SCANCODE_E]) {
                    // static_cast<Player*>(player)->down();
                    static_cast<Player*>(player)->explode();
                }
            }
        }

        // In this section of the event loop we allow key bounce:

        ticks = clock.getElapsedTicks();
        if ( ticks > 0 ) {
            clock.incrFrame();

            // movement control
            if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
                static_cast<Player*>(player)->left();
            }
            if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
                static_cast<Player*>(player)->right();
            }
            if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
                // static_cast<Player*>(player)->up();
                static_cast<Player*>(player)->jump();
            }
            if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
                // static_cast<Player*>(player)->down();
                static_cast<Player*>(player)->crouch();
            }

            draw();
            update(ticks);
            if ( makeVideo ) {
                frameGen.makeFrame();
            }
        }
    }
    return false;
}
