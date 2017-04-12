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
#ifndef SDLTK_CANVAS_STD_H
#define SDLTK_CANVAS_STD_H

#include "canvas.h"

namespace sdltk
{

    /** @author Fred Klaus development@fkweb.de */
    class CanvasStd : public Canvas
    {
    public:
        CanvasStd() ROLAND_DEFAULT
        virtual ~CanvasStd() ROLAND_DEFAULT

        virtual void point(const Point & pos);
        virtual void rect (const Rect  & rect);
        virtual void fill (const Rect  & rect);
        virtual void line (const Point & pos1, const Point & pos2);
        virtual void image(const Image & image, const Rect  & src,  const Rect  & dest);
        //virtual void write(Point & pos, String & text);

        virtual void setClipRect(const Rect & rect);
        virtual void clearClipRect();

    private:
        void hLine(Sint32 x, Sint32  y, Sint32  x2);
        void vLine(Sint32 x, Sint32  y, Sint32  y2);

        inline void putglyph(char *p, int Bpp, int pitch, int which);
    };

} // sdltk

#endif // SDLTK_CANVAS_STD_H
