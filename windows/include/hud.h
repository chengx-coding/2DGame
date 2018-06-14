//#include <SDL2/SDL.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "ioMod.h"
#include "gamedata.h"

class HUD {
public:
    static HUD& getInstance();
    void addLine(const std::string&, int);
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
    std::map<int, std::string> otherLines;

    HUD();
    HUD(const HUD &) = delete;
    HUD& operator=(const HUD&) = delete;
};
