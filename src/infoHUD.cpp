#include "infoHUD.h"
#include "renderContext.h"
#include "viewport.h"
#include "clock.h"


InfoHUD& InfoHUD::getInstance() {
    static InfoHUD instance;
    return instance;
}

InfoHUD::InfoHUD() :
    renderer(RenderContext::getInstance()->getRenderer()),
    alpha(Gamedata::getInstance().getXmlInt("InfoHUD/alpha")),
    posX(Gamedata::getInstance().getXmlInt("InfoHUD/posX")),
    posY(Gamedata::getInstance().getXmlInt("InfoHUD/posY")),
    width(Gamedata::getInstance().getXmlInt("InfoHUD/width")),
    numOfLines(Gamedata::getInstance().getXmlInt("InfoHUD/numOfLines")),
    interval(Gamedata::getInstance().getXmlInt("InfoHUD/interval")),
    height( numOfLines * interval ),
    rect({posX, posY, width, height}),
    iomod(IOMod::getInstance()),
    lines()
{
    std::cout << "InfoHUD is loaded" << '\n';
}

void InfoHUD::addLine(const std::string& msg) {
    lines.push_back(msg);
}

void InfoHUD::update() {
    lines.clear();
}

void InfoHUD::draw() const {
    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, alpha );

    // Render rect
    SDL_RenderFillRect( renderer, &rect );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 255, 0, 255/2, alpha );
    SDL_RenderDrawRect( renderer, &rect );

    // Instructions
    iomod.writeText("Tracking: "+ \
                    Viewport::getInstance().getObjectToTrack()->getName(),
                    posX + 10, posY,
                    {0, 100, 150, alpha});
    iomod.writeText("FPS: " + \
                    std::to_string(Clock::getInstance().getFps()),
                    posX + 250, posY,
                    {30, 200, 150, alpha});

    int i = 1;
    for(auto line : lines) {
        //static int i = 1;
        iomod.writeText(line,
                        posX + 10, posY + i * interval,
                        alpha);
        i++;
    }
}
