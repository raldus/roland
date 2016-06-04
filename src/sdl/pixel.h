/***************************************************************************
 *   Copyright (C) by Fred Klaus                                           *
 *       development@fkweb.de                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SDLTK_PIXEL_H
#define SDLTK_PIXEL_H

#include "color.h"
#include "SDL.h"

namespace sdltk
{
    class Pixel
    {

    public:
        Pixel()  = delete;
        ~Pixel() = delete;

        //! Checks a pixels color of an SDL_Surface.
        //! @param surface an SDL_Surface where to check for a pixel color.
        //! @param x the x coordinate on the surface.
        //! @param y the y coordinate on the surface.
        //! @return a color of a pixel.
        inline static const Color get(SDL_Surface* surface, Sint16  x, Sint16  y)
        {
            Sint32  bpp = surface->format->BytesPerPixel;

            SDL_LockSurface(surface);

            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

            Uint32  color = 0;

            switch(bpp)
            {
            case 1:
                color = *p;
                break;

            case 2:
                color = *(Uint16 *)p;
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    color = p[0] << 16 | p[1] << 8 | p[2];
                }
                else
                {
                    color = p[0] | p[1] << 8 | p[2] << 16;
                }
                break;

            case 4:
                color = *(Uint32 *)p;
                break;
            }

            Uint8  r,g,b,a;

            SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
            SDL_UnlockSurface(surface);

            return Color(r,g,b,a);
        }

         //! Puts a pixel on an SDL_Surface.
         //!
         //! @param x the x coordinate on the surface.
         //! @param y the y coordinate on the surface.
         //! @param color the color the pixel should be in.
        inline static void put(SDL_Surface* surface, Sint32  x, Sint32  y, const Color& color)
        {
            Sint32  bpp = surface->format->BytesPerPixel;

            SDL_LockSurface(surface);

            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

            Uint32 pixel = SDL_MapRGB(surface->format, color.r(), color.g(), color.b());

            switch(bpp)
            {
            case 1:
                *p = pixel;
                break;

            case 2:
                *(Uint16 *)p = pixel;
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                }
                else
                {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;

            case 4:
                *(Uint32 *)p = pixel;
                break;
            }

            SDL_UnlockSurface(surface);
        }

         //! Blends two 32 bit colors together.
         //! @param src the source color.
         //! @param dst the destination color.
         //! @param a alpha.
        inline static Uint32 alpha32(Uint32  src, Uint32  dst, Uint8  a)
        {
            Uint32  b = ((src & 0xff) * a + (dst & 0xff) * (255 - a)) >> 8;
            Uint32  g = ((src & 0xff00) * a + (dst & 0xff00) * (255 - a)) >> 8;
            Uint32  r = ((src & 0xff0000) * a + (dst & 0xff0000) * (255 - a)) >> 8;

            return (b & 0xff) | (g & 0xff00) | (r & 0xff0000);
        }

         //! Blends two 16 bit colors together.
         //! @param src the source color.
         //! @param dst the destination color.
         //! @param a alpha.
        inline static Uint16 alpha16(Uint16 src, Uint16 dst, Uint8  a, const SDL_PixelFormat *f)
        {
            Uint32  b = ((src & f->Rmask) * a + (dst & f->Rmask) * (255 - a)) >> 8;
            Uint32  g = ((src & f->Gmask) * a + (dst & f->Gmask) * (255 - a)) >> 8;
            Uint32  r = ((src & f->Bmask) * a + (dst & f->Bmask) * (255 - a)) >> 8;

            return (Uint16)((b & f->Rmask) | (g & f->Gmask) | (r & f->Bmask));
        }

        /*
        typedef struct{
            SDL_Palette *palette;
            Uint8  BitsPerPixel;
            Uint8  BytesPerPixel;
            Uint32 Rmask, Gmask, Bmask, Amask;
            Uint8  Rshift, Gshift, Bshift, Ashift;
            Uint8  Rloss, Gloss, Bloss, Aloss;
            Uint32 colorkey;
            Uint8  alpha;
        } SDL_PixelFormat;
        */

         //! Puts a pixel on an SDL_Surface with alpha
         //! @param x the x coordinate on the surface.
         //! @param y the y coordinate on the surface.
         //! @param color the color the pixel should be in.
        inline static void putAlpha(SDL_Surface* surface, Sint32  x, Sint32  y, const Color& color)
        {
            Uint32 bpp = surface->format->BytesPerPixel;

            SDL_LockSurface(surface);

            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

            Uint32 pixel = SDL_MapRGB(surface->format, color.r(), color.g(), color.b());

            switch(bpp)
            {
            case 1:
                *p = pixel;
                break;

            case 2:
                *(Uint16 *)p = alpha16(pixel, *(Uint32 *)p, color.a(), surface->format);
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    Uint32  r = (p[0] * (255 - color.a()) + color.r() * color.a()) >> 8;
                    Uint32  g = (p[1] * (255 - color.a()) + color.g() * color.a()) >> 8;
                    Uint32  b = (p[2] * (255 - color.a()) + color.b() * color.a()) >> 8;

                    p[2] = b;
                    p[1] = g;
                    p[0] = r;
                }
                else
                {
                    Uint32  r = (p[2] * (255 - color.a()) + color.r() * color.a()) >> 8;
                    Uint32  g = (p[1] * (255 - color.a()) + color.g() * color.a()) >> 8;
                    Uint32  b = (p[0] * (255 - color.a()) + color.b() * color.a()) >> 8;

                    p[0] = b;
                    p[1] = g;
                    p[2] = r;
                }
                break;

            case 4:
                *(Uint32 *)p = alpha32(pixel, *(Uint32 *)p, color.a());
                break;
            }

            SDL_UnlockSurface(surface);
        }

    };

} // sdltk

#endif // SDLTK_PIXEL_H
