#ifndef IOMOD__H
#define IOMOD__H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class IOMod {
public:
    static IOMod& getInstance();
    ~IOMod();
    SDL_Texture* readTexture(const std::string& filename);
    SDL_Surface* readSurface(const std::string& filename);
    void writeText(const std::string&, int, int) const;
    void writeText(const std::string&, int, int, Uint8) const;
    void writeText(const std::string&, int, int, SDL_Color) const;
    SDL_Renderer* getRenderer() const {
        return renderer;
    }
private:
    int init;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color textColor;
    IOMod();
    IOMod(const IOMod&);
    IOMod& operator=(const IOMod&);
};

#endif
