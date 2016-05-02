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
#ifndef SDLGUIPOINT_H
#define SDLGUIPOINT_H

#include "SDL.h"

namespace sdltk
{
    /// Defines a 2D point (x,y)
    /** @author Fred Klaus */
    class Point
    {

    public:
        /** Standard Constructor. Initializes x and y to 0. */
        Point() {}
        /** Constructor. @param x = Coordinate x @param y = Coordinate y */
        Point(Sint16 x, Sint16 y)    {mX = x; mY = y;}
        /** Copyconstructor. It is save to init Point with itself. */
        Point(const Point & p) {mX = p.x(); mY = p.y();}

        /** Standard Destructor. Does nothing */
        ~Point() {}

        /** Overloaded operator =. A deep copy will done, so it's save to assign Point to itself. */
        Point & operator=(const Point & p)
        {if (this == &p) return *this; mX=p.x(); mY=p.y(); return *this;}

        bool operator==(const Point & p) const
            {return (mX == p.x() && mY == p.y()) ? true : false;}

        bool operator!=(const Point & p) const
            {return (mX != p.x() || mY != p.y()) ? true : false;}

        Sint16 x() const {return mX;}
        Sint16 y() const {return mY;}

        void setX(Sint16 x)       {mX = x;}
        void setY(Sint16 y)       {mY = y;}
        void set(const Point & p) {mX = p.x(); mY = p.y();}
        void set (Sint16 x, Sint16 y) {mX = x; mY = y;}

        void qX(const Point & p) {mX ^= p.x();}
        void qY(const Point & p) {mY ^= p.y();}

    private:
        Sint16 mX;
        Sint16 mY;
    };

} //namespace sdltk


#endif //SDLGUIPOINT_H
