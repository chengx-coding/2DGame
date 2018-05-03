#include <SDL_image.h>
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

IOMod& IOMod::getInstance() {
    static IOMod instance;
    return instance;
}

IOMod::~IOMod() {
    TTF_CloseFont(font);
    TTF_Quit();
}

IOMod::IOMod() :
    init(TTF_Init()),
    renderer( RenderContext::getInstance()->getRenderer() ),
    font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                      Gamedata::getInstance().getXmlInt("font/size"))),
    textColor({0xff, 0, 0, 0})
{
    if ( init == -1 ) {
        throw std::string("error: Couldn't init font");
    }
    if (font == NULL) {
        throw std::string("error: font not found");
    }
    textColor.r = Gamedata::getInstance().getXmlInt("font/red");
    textColor.g = Gamedata::getInstance().getXmlInt("font/green");
    textColor.b = Gamedata::getInstance().getXmlInt("font/blue");
    textColor.a = Gamedata::getInstance().getXmlInt("font/alpha");
}

SDL_Texture* IOMod::readTexture(const std::string& filename) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
    if ( texture == NULL ) {
        throw std::string("Couldn't load ") + filename;
    }
    return texture;
}

SDL_Surface* IOMod::readSurface(const std::string& filename) {
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if ( !surface ) {
        throw std::string("Couldn't load ") + filename;
    }
    return surface;
}

void IOMod::writeText(const std::string& msg, int x, int y) const {
    writeText(msg, x, y, textColor);
}

void IOMod::writeText(const std::string& msg, int x, int y, Uint8 alpha) const {
    writeText(msg, x, y, {textColor.r, textColor.g, textColor.b, alpha});
}

void IOMod::writeText(const std::string& msg, int x, int y, SDL_Color fontColor) const {
    // SDL_Surface* surface =
    //     TTF_RenderText_Solid(font, msg.c_str(), fontColor);

    // SDL_Surface* surface =
    //     TTF_RenderText_Blended(font, msg.c_str(), fontColor);
    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(font, msg.c_str(), fontColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureAlphaMod(texture, fontColor.a); //set the alpha mod
    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect dst = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}
