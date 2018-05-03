#include "hud.h"
#include "renderContext.h"
#include "viewport.h"
#include "clock.h"


HUD& HUD::getInstance() {
    static HUD instance;
    return instance;
}

HUD::HUD() :
    renderer(RenderContext::getInstance()->getRenderer()),
    alpha(Gamedata::getInstance().getXmlInt("HUD/alpha")),
    posX(Gamedata::getInstance().getXmlInt("HUD/posX")),
    posY(Gamedata::getInstance().getXmlInt("HUD/posY")),
    width(Gamedata::getInstance().getXmlInt("HUD/width")),
    numOfLines(Gamedata::getInstance().getXmlInt("HUD/numOfLines")),
    interval(Gamedata::getInstance().getXmlInt("HUD/interval")),
    height( numOfLines * interval ),
    rect({posX, posY, width, height}),
    iomod(IOMod::getInstance()),
    otherLines()
{
    std::cout << "HUD is loaded" << '\n';
}

void HUD::addLine(const std::string& msg, int line) {
    if(line < 0) {
        line += numOfLines;
    }
    otherLines.insert(std::pair<int, std::string>(line, msg));
}

void HUD::update() {
    otherLines.clear();
}

void HUD::draw() const {
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
    for(int i = 0; i < numOfLines; i++) {
        if( Gamedata::getInstance().checkTag("HUD/line" + std::to_string(i)) ) {
            iomod.writeText(
                Gamedata::getInstance().getXmlStr("HUD/line" + std::to_string(i)),
                posX + 10, posY + i * interval,
                alpha);
        }
    }

    for(auto line : otherLines) {
        if(line.first > numOfLines)
            std::cout << "Don't have enough in HUD" << std::endl;
        iomod.writeText(line.second, posX + 10, posY + line.first * interval, alpha);
    }
}
