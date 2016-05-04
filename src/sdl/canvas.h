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
#ifndef SDLGUIDRAW_H
#define SDLGUIDRAW_H

#include "color.h"
#include "point.h"
#include "rect.h"
#include "size.h"
#include "image.h"
#include "imagefont.h"

#include <string>
#include <cstdlib>
#include <algorithm>


namespace sdltk
{

    /** @author Fred Klaus development@fkweb.de */
    class Canvas
    {
    public:
        Canvas();
        virtual ~Canvas();

        virtual void begin() {}
        virtual void end()   {}

        virtual void point(const Point & pos) = 0;
        virtual void line (const Point & pos1, const Point & pos2) = 0;
        virtual void rect (const Rect  & rect) = 0;
        virtual void fill (const Rect  & rect) = 0;
        virtual void image(const Image & image, const Rect & src, const Rect & dest) = 0;
        virtual void setColor(const Color & color) {mColor = color;}
        virtual void setClipRect(const Rect & rect) {}
        virtual void clearClipRect() {}


        void setFont(const string & fname, const string & glyphs);
        void setSurface(SDL_Surface* surface) {mSurface = surface;}

        void point(const Point * pos)  {point(*pos);}
        void line (const Point * pos1, Point * pos2) {line(*pos1, *pos2);}
        void rect (const Rect  * rec)  {rect(*rec);}
        void fill (const Rect  * rec)  {fill(*rec);}
        void image(const Image * img, const Point & pos) {image(*img, pos);}

        inline void image(const Image & img, const Point & pos);
        inline void write(const Point & pos, const string & text);

        inline const Size &   textSize(const string & text);
        inline const Uint16   textHeight();
        inline const string & numberToText(Sint32 num, Uint8 base=10);

        ImageFont * font() {return &mFont;}

        const Color & color() {return mColor;}

    protected:
        SDL_Surface * mSurface;
        Size          mTextSize;
        Color         mColor;
        ImageFont     mFont;
        string        mNumber;
        Rect          mClipRect;
    };


    inline void Canvas::image(const Image & img, const Point & pos)
    {
        Rect src(0, 0, img.size());
        Rect dst(pos, img.size());

        image(img, src, dst);
    }

    inline void Canvas::write(const Point & pos, const string & text)
    {
        Rect src, dest;
        Point p(pos);
        for (Uint16 i=0; i < text.length(); i++)
        {
            src = mFont.glyph(text[i]);
            dest.set(p, src.size());
            image(mFont.image(), src,  dest);
            p.setX(dest.x() + src.width() + mFont.spacing());
        }
    }

    inline const Size & Canvas::textSize(const string & text)
    {
        Rect rect;
        mTextSize.clear();
        for (Uint16 i=0; i < text.length(); i++)
        {
            rect  = mFont.glyph(text[i]);
            mTextSize.setWidth(mTextSize.width() + rect.width() + mFont.spacing());
        }
        mTextSize.setWidth(mTextSize.width() - mFont.spacing());
        mTextSize.setHeight(rect.height());
        return mTextSize;
    }

    inline const Uint16 Canvas::textHeight()
    {
        Rect rect = mFont.glyph(' ');
        return rect.height();
    }

    inline const string & Canvas::numberToText(Sint32 num, Uint8 base)
    {
        mNumber.clear();

        const Uint8 kMaxDigits = 35;

        mNumber.reserve(kMaxDigits); // Pre-allocate enough space.

        // check that the base if valid
        if (base < 2 || base > 16) return mNumber;

        int quotient = num;

        // Translating number to string with base:
        do
        {
            mNumber += "0123456789abcdef"[std::abs(quotient % base)];
            quotient /= base;
        }
        while (quotient);

        // Append the negative sign for base 10
        if ( num < 0 && base == 10) mNumber += '-';

        std::reverse(mNumber.begin(), mNumber.end());

        return mNumber;
    }

} // sdltk

#endif // SDLGUIDRAW_H
