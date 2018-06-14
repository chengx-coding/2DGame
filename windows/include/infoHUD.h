//#include <SDL2/SDL.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <list>
#include "ioMod.h"
#include "gamedata.h"

class InfoHUD {
public:
    static InfoHUD& getInstance();
    void addLine(const std::string&);
    void update();
    void draw() const;
private:
    SDL_Renderer* renderer;
    Uint8 alpha;
    int posX;
    int posY;
    int width;
    int numOfLines;
    int interval;
    int height;
    SDL_Rect rect;
    IOMod& iomod;
    std::list<std::string> lines;

    InfoHUD();
    InfoHUD(const InfoHUD &) = delete;
    InfoHUD& operator=(const InfoHUD&) = delete;
};
