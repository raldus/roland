/***************************************************************************
 *   Copyright (C) 2005-2013 by Fred Klaus <development@fkweb.de>          *
 *                                                                         *
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
#ifndef SDLGUIDRAWSTD_H
#define SDLGUIDRAWSTD_H

#include "draw.h"
#include <string>

using std::string;

namespace sdltk
{

    /** @author Fred Klaus development@fkweb.de */
    class DrawStd : public Draw
    {
    public:
        DrawStd();
        virtual ~DrawStd();

        virtual void point(const Point & pos);
        virtual void rect (const Rect  & rect);
        virtual void fill (const Rect  & rect);
        virtual void line (const Point & pos1, const Point & pos2);
        virtual void image(const Image & image, const Rect  & src,  const Rect  & dest);
        //virtual void write(Point & pos, string & text);
        
        virtual void setClipRect(const Rect & rect);
        virtual void clearClipRect();

    private:        
        void hLine(Sint32 x, Sint32  y, Sint32  x2);
        void vLine(Sint32 x, Sint32  y, Sint32  y2);
        
        inline void putglyph(char *p, int Bpp, int pitch, int which);
    };

} //namespace sdltk

#endif //SDLGUIDRAWSTD_H
