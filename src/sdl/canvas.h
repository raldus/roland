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
#ifndef SDLTK_CANVAS_H
#define SDLTK_CANVAS_H

#include "types.h"
#include "image.h"
#include "imagefont.h"
#include <cstdlib>
#include <algorithm>


namespace sdltk
{

    class Color;
    class Point;
    class Rect;
    class Size;

    //! \author Fred Klaus development@fkweb.de */
    class Canvas
    {

    public:
        Canvas() = default;
        virtual ~Canvas() = default;

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

        void setFont(const String & fname, const String & glyphs);
        void setSurface(SDL_Surface * const surface) {mSurface = surface;}

        void point(Point * const pos)  {point(*pos);}
        void line (Point * const pos1, Point * const pos2) {line(*pos1, *pos2);}
        void rect (Rect  * const rec)  {rect(*rec);}
        void fill (Rect  * const rec)  {fill(*rec);}
        void image(Image * const img, const Point & pos) {image(*img, pos);}

        void image(const Image & img, const Point & pos);
        void write(const Point & pos, const String & text);

        const Size & textSize(const String & text);
        const Uint16 textHeight() const;

        const ImageFont & font()  const {return mFont;}
        const Color     & color() const {return mColor;}

        uint width() {return mSurface ? mSurface->w : 320;}

    protected:
        SDL_Surface * mSurface;
        Size          mTextSize;
        Color         mColor;
        ImageFont     mFont;
        String   mNumber;
        Rect          mClipRect;
    };


    inline void Canvas::image(const Image & img, const Point & pos)
    {
        Rect src(0, 0, img.size());
        Rect dst(pos, img.size());

        image(img, src, dst);
    }

    inline void Canvas::write(const Point & pos, const String & text)
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

    inline const Size & Canvas::textSize(const String & text)
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

    inline const Uint16 Canvas::textHeight() const
    {
        Rect rect = mFont.glyph(' ');
        return rect.height();
    }

}; // sdltk

#endif // SDLTK_CANVAS_H
