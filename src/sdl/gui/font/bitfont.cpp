// DGen/SDL v1.14+
// by Joe Groff
// How's my programming? E-mail <joe@pknet.com>

/* DGen's font renderer.
 * I hope it's pretty well detached from the DGen core, so you can use it in
 * any other SDL app you like. */

/* Also note that these font renderers do no error detection, and absolutely
 * NO clipping whatsoever, so try to keep the glyphs on-screen. Thank you :-)
 */

#include "bitfont.h"
#include "bitglyph.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>


namespace sdltk
{
    inline void BitFont::putglyph(char *p, int Bpp, int pitch, int which)
    {
        int *glyph = BitGlyph::glyph(which);
        int x = 0, i;

        for(; *glyph != -1; ++glyph)
        {
            p += (((x += *glyph) >> 3) * pitch); x &= 7;
            for(i = 0; i < Bpp; ++i) p[(x * Bpp) + i] = 0xff;
        }
    }

    void BitFont::write(SDL_Surface *surf, int x, int y, int num)
    {
        char msg[15];
        sprintf(msg, "%3d", (int) num);
        write(surf, x, y, msg);
    }

    void BitFont::write(SDL_Surface *surf, int x, int y, const char *message, int len)
    {
        int pitch = surf->pitch, Bpp = surf->format->BytesPerPixel;
        char *p = (char*)surf->pixels + (pitch * y) + (Bpp * x);

        if(SDL_MUSTLOCK(surf))
            if(SDL_LockSurface(surf) < 0)
            {
                fprintf(stderr, "Font: Couldn't lock screen: %s!", SDL_GetError());
                return;
            }

        if (len==0)
        {
            for(; *message; p += (8 * Bpp), ++message)
                putglyph(p, Bpp, pitch, *message);
        }
        else
        {
            for(; len > 0; p += (8 * Bpp), ++message, --len)
                putglyph(p, Bpp, pitch, *message);
        }

        if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
    }

} //namespace sdltk
