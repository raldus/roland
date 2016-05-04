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
#include "drawstd.h"

#include "sdlpixel.h"
#include "bitglyph.h"

#ifndef ABS
    #define ABS(x) ((x)<0?-(x):(x))
#endif

namespace sdltk
{

    DrawStd::DrawStd() : Draw()
    {}

    DrawStd::~DrawStd()
    {}
    
    void DrawStd::setClipRect(const Rect & rect)
    {
        mClipRect = rect;
        SDL_SetClipRect(mSurface, &rect);
    }
    
    void DrawStd::clearClipRect() 
    {
        mClipRect.set(0, 0, mSurface->w, mSurface->h);
        SDL_SetClipRect(mSurface, 0);
    }

    void DrawStd::point(const Point& pos)
    {
        if (mColor.a())
        {
            Pixel::putAlpha(mSurface, pos.x(), pos.y(), mColor);
        }
        else
        {
            Pixel::put(mSurface, pos.x(), pos.y(), mColor);
        }
    }

    void DrawStd::rect(const Rect & rect)
    {
        Sint32  x  = rect.x();
        Sint32  x2 = rect.x() + rect.width()  - 1;
        Sint32  y  = rect.y();
        Sint32  y2 = rect.y() + rect.height() - 1;

        hLine(x, y, x2);
        hLine(x, y2, x2);

        vLine(x, y, y2);
        vLine(x2, y, y2);
    }

    void DrawStd::fill(const Rect & rect)
    {
        if (mColor.a())
        {
            int x1 = rect.x();
            int y1 = rect.y();
            int x2 = rect.x() + rect.width();
            int y2 = rect.y() + rect.height();
            int y;

            for (y = y1; y <= y2; y++)
            {
                hLine(x1, y, x2);
            }

        }
        else
        {
            Uint32 color = SDL_MapRGB(mSurface->format, mColor.r(), mColor.g(), mColor.b());
            SDL_FillRect(mSurface, (Rect*) &rect, color); ///@todo (Rect*) ??
        }
    }

    void DrawStd::line(const Point & pos1, const Point & pos2)
    {
        Point p1(pos1);
        Point p2(pos2);
        
        if (p1.x() == p2.x())
        {
            vLine(p1.x(), p1.y(), p2.y());
            return;
        }
        if (p1.y() == p2.y())
        {
            hLine(p1.x(), p1.y(), p2.x());
            return;
        }

        /*
        ClipRectangle top = mClipStack.top();
        p1.x() += top.xOffset;
        p1.y() += top.yOffset;
        p2.x() += top.xOffset;
        p2.y() += top.yOffset;
        */

        // Draw a line with Bresenham

        Sint32  dx = ABS(p2.x() - p1.x());
        Sint32  dy = ABS(p2.y() - p1.y());

        if (dx > dy)
        {
            if (p1.x() > p2.x())
            {
                // swap p1.x(), p2.x()
                p1.qX(p2);
                p2.qX(p1);
                p1.qX(p2);

                // swap p1.y(), p2.y()
                p1.qY(p2);
                p2.qY(p1);
                p1.qY(p2);
            }

            if (p1.y() < p2.y())
            {
                Sint32  y = p1.y();
                Sint32  p = 0;

                for (Sint32  x = p1.x(); x <= p2.x(); x++)
                {
                    if (mClipRect.inside(x, y))
                    {
                        if (mColor.a())
                        {
                            Pixel::putAlpha(mSurface, x, y, mColor);
                        }
                        else
                        {
                            Pixel::put(mSurface, x, y, mColor);
                        }
                    }

                    p += dy;

                    if (p * 2 >= dx)
                    {
                        y++;
                        p -= dx;
                    }
                }
            }
            else
            {
                Sint32  y = p1.y();
                Sint32  p = 0;

                for (Sint32  x = p1.x(); x <= p2.x(); x++)
                {
                    if (mClipRect.inside(x, y))
                    {
                        if (mColor.a())
                        {
                            Pixel::putAlpha(mSurface, x, y, mColor);
                        }
                        else
                        {
                            Pixel::put(mSurface, x, y, mColor);
                        }
                    }

                    p += dy;

                    if (p * 2 >= dx)
                    {
                        y--;
                        p -= dx;
                    }
                }
            }
        }
        else
        {
            if (p1.y() > p2.y())
            {
                // swap p1.y(), p2.y()
                p1.qY(p2);
                p2.qY(p1);
                p1.qY(p2);
                
                // swap p1.x(), p2.x()
                p1.qX(p2);
                p2.qX(p1);
                p1.qX(p2);
            }

            if (p1.x() < p2.x())
            {
                Sint32  x = p1.x();
                Sint32  p = 0;

                for (Sint32  y = p1.y(); y <= p2.y(); y++)
                {
                    if (mClipRect.inside(x, y))
                    {
                        if (mColor.a())
                        {
                            Pixel::putAlpha(mSurface, x, y, mColor);
                        }
                        else
                        {
                            Pixel::put(mSurface, x, y, mColor);
                        }
                    }

                    p += dx;

                    if (p * 2 >= dy)
                    {
                        x++;
                        p -= dy;
                    }
                }
            }
            else
            {
                Sint32  x = p1.x();
                Sint32  p = 0;

                for (Sint32  y = p1.y(); y <= p2.y(); y++)
                {
                    //if (top.isPointInRect(x, y))
                    //{
                        if (mColor.a())
                        {
                            Pixel::putAlpha(mSurface, x, y, mColor);
                        }
                        else
                        {
                            Pixel::put(mSurface, x, y, mColor);
                        }
                    //}

                    p += dx;

                    if (p * 2 >= dy)
                    {
                        x--;
                        p -= dy;
                    }
                }
            }
        }
    }

    void DrawStd::image(const Image & image, const Rect& src, const Rect& dest)
    {
        Image* img = (Image*) &image;
        Rect s(src);
        Rect d(dest);
        SDL_BlitSurface(img->surface(), &s, mSurface, &d);
    }

    void DrawStd::hLine(Sint32 x, Sint32 y, Sint32  x2)
    {
        //ClipRectangle top = mClipStack.top();
        //x  += top.xOffset;
        //y  += top.yOffset;
        //x2 += top.xOffset;

        if (y < mClipRect.y() || y >= mClipRect.y() + mClipRect.height()) return;

        if (x > x2)
        {
            x  ^= x2;
            x2 ^= x;
            x  ^= x2;
        }

        
        if (mClipRect.x() > x)
        {
            if (mClipRect.x() > x2)
            {
                return;
            }
            x = mClipRect.x();
        }

        if (mClipRect.x() + mClipRect.width() <= x2)
        {
            if (mClipRect.x() + mClipRect.width() <= x)
            {
                return;
            }
            x2 = mClipRect.x() + mClipRect.width() -1;
        }
        

        Uint32  bpp = mSurface->format->BytesPerPixel;

        SDL_LockSurface(mSurface);

        Uint8 *p = (Uint8 *)mSurface->pixels + y * mSurface->pitch + x * bpp;

        Uint32 pixel = SDL_MapRGB(mSurface->format, mColor.r(), mColor.g(), mColor.b());

        switch(bpp)
        {
        case 1:
            {
                for (;x <= x2; ++x)
                {
                    *(p++) = pixel;
                }
            }
            break;

        case 2:
            {
                Uint16* q = (Uint16*)p;
                for (;x <= x2; ++x)
                {
                    *(q++) = pixel;
                }
            }
            break;

        case 3:
            {
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    for (;x <= x2; ++x)
                    {
                        p[0] = (pixel >> 16) & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = pixel & 0xff;
                        p += 3;
                    }
                }
                else
                {
                    for (;x <= x2; ++x)
                    {
                        p[0] = pixel & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = (pixel >> 16) & 0xff;
                        p += 3;
                    }
                }
            }
            break;

        case 4:
            {
                Uint32* q = (Uint32*)p;
                for (;x <= x2; ++x)
                {
                    if (mColor.a())
                    {
                        *q = Pixel::alpha32(pixel, *q, mColor.a());
                        q++;
                    }
                    else
                    {
                        *(q++) = pixel;
                    }
                }
            }
            break;

        } // end switch

        SDL_UnlockSurface(mSurface);
    }

    void DrawStd::vLine(Sint32 x, Sint32 y, Sint32  y2)
    {
        //ClipRectangle top = mClipStack.top();
        //x  += top.xOffset;
        //y  += top.yOffset;
        //y2 += top.yOffset;

        if (x < mClipRect.x() || x >= mClipRect.x() + mClipRect.width()) return;

        if (y > y2)
        {
            y  ^= y2;
            y2 ^= y;
            y  ^= y2;
        }

        
        if (mClipRect.y() > y)
        {
            if (mClipRect.y() > y2)
            {
                return;
            }
            y = mClipRect.y();
        }

        if (mClipRect.y() + mClipRect.height() <= y2)
        {
            if (mClipRect.y() + mClipRect.height() <= y)
            {
                return;
            }
            y2 = mClipRect.y() + mClipRect.height() - 1;
        }
        

        Uint32  bpp = mSurface->format->BytesPerPixel;

        SDL_LockSurface(mSurface);

        Uint8 *p = (Uint8 *)mSurface->pixels + y * mSurface->pitch + x * bpp;

        Uint32 pixel = SDL_MapRGB(mSurface->format, mColor.r(), mColor.g(), mColor.b());

        switch(bpp)
        {
        case 1:
            {
                for (;y <= y2; ++y)
                {
                    *p = pixel;
                    p += mSurface->pitch;
                }
            }
            break;

        case 2:
            {
                for (;y <= y2; ++y)
                {
                    *(Uint16*)p = pixel;
                    p += mSurface->pitch;
                }
            }
            break;

        case 3:
            {
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    for (;y <= y2; ++y)
                    {
                        p[0] = (pixel >> 16) & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = pixel & 0xff;
                        p += mSurface->pitch;
                    }
                }
                else
                {
                    for (;y <= y2; ++y)
                    {
                        p[0] = pixel & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = (pixel >> 16) & 0xff;
                        p += mSurface->pitch;
                    }
                }
            }
            break;

        case 4:
            {
                for (;y <= y2; ++y)
                {
                    if (mColor.a())
                    {
                        *(Uint32*)p = Pixel::alpha32(pixel,*(Uint32*)p,mColor.a());
                    }
                    else
                    {
                        *(Uint32*)p = pixel;
                    }
                    p += mSurface->pitch;
                }
            }
            break;
        } // end switch

        SDL_UnlockSurface(mSurface);
    }
    
    /*
    void DrawStd::write(Point & pos, string & text)
    {
        Rect src, dest;
        for (Uint16 i=0; i < text.length(); i++)
        {
            src = mFont.glyph(text[i]);
            dest.set(pos.x(), pos.y(), src.width(), src.height());
            image(mFont.image(), src,  dest);
            pos.setX(dest.x()+src.width());
        }
    }*/

}
