#ifndef FONT_H
#define FONT_H

#include "SDL.h"

class Font
{

public:
    Font() {}
    ~Font() {}

    void write(SDL_Surface *surf, int x, int y, const char *message, int len=0);
    void write(SDL_Surface *surf, int x, int y, int num);

private:
    inline void putglyph(char *p, int Bpp, int pitch, int which);

};

#endif // FONT_H
