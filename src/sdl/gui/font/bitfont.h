#ifndef BITFONT_H
#define BITFONT_H

#include "canvas.h"
#include "image.h"
#include "bitfont.h"
#include "SDL.h"

namespace sdltk
{

    class BitFont
    {
    public:
        BitFont() {}
        ~BitFont() {}

        void write(SDL_Surface *surf, int x, int y, const char *message, int len=0);
        void write(SDL_Surface *surf, int x, int y, int num);

    private:
        inline void putglyph(char *p, int Bpp, int pitch, int which);

    };


} //namespace sdltk

#endif //BITFONT_H
