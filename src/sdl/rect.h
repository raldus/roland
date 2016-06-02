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
#ifndef SDLTK_RECT_H
#define SDLTK_RECT_H

#include "point.h"
#include "size.h"
#include "SDL.h"

namespace sdltk
{

    //! Defines a rect (x, y, width, height)
    //! author Fred Klaus
    class Rect : public SDL_Rect
    {

    public:
        //! Standard Constructor.
        Rect() {}
        Rect(const Point & origin, const Size & size)
        {   SDL_Rect::x = origin.x();
            SDL_Rect::y = origin.y();
            SDL_Rect::w = size.width();
            SDL_Rect::h = size.height();
        }
        //Rect(const Point & origin, Sint16 width, Sint16 height) {mOrigin=origin; mSize.set(width, height);}
        //Rect(Sint16 x, Sint16 y, const Size & size)             {mOrigin.set(x, y); mSize=size;}
        Rect(Sint16 x, Sint16 y, Uint16 w, Uint16 h)  {SDL_Rect::x = x; SDL_Rect::y = y; SDL_Rect::w = w; SDL_Rect::h = h;}

        Rect(Sint16 x, Sint16 y, const Size & size)  {SDL_Rect::x = x; SDL_Rect::y = y; SDL_Rect::w = size.width(); SDL_Rect::h = size.height();}

        Rect(SDL_Rect* rect) {SDL_Rect::x = rect->x; SDL_Rect::y = rect->y; SDL_Rect::w = rect->w; SDL_Rect::h = rect->h;}

        //! Copyconstructor. A deep copy will done, so it's save to init Rect with itself.
        Rect(const Rect & rect) {SDL_Rect::x = rect.x(); SDL_Rect::y = rect.y(); SDL_Rect::w = rect.width(); SDL_Rect::h = rect.height();}


        //! Standard Destructor. Does nothing
        ~Rect() {}

        void set(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {SDL_Rect::x = x; SDL_Rect::y = y; SDL_Rect::w = w; SDL_Rect::h = h;}
        //void set(const Rect  & rect)  {mOrigin = rect.origin(); mSize=rect.size();}
        void set(const Point & pos)  {SDL_Rect::x = pos.x(); SDL_Rect::y = pos.y();}
        void set(const Size  & size) {SDL_Rect::w = size.width(); SDL_Rect::h = size.height();}
        void set(const Point & pos, const Size  & size)  {SDL_Rect::x = pos.x(); SDL_Rect::y = pos.y(); SDL_Rect::w = size.width(); SDL_Rect::h = size.height();}

        void setOrigin(Sint16 x, Sint16 y)   {SDL_Rect::x = x; SDL_Rect::y = y;}
        void setOrigin(const Point & origin) {SDL_Rect::x = origin.x(); SDL_Rect::y = origin.y();}

        void setPos(Sint16 x, Sint16 y)      {SDL_Rect::x = x; SDL_Rect::y = y;}
        void setPos(const Point & pos)       {SDL_Rect::x = pos.x(), SDL_Rect::y = pos.y();}

        void setSize(Uint16 w, Uint16 h)     {SDL_Rect::w = w; SDL_Rect::h = h;}
        void setSize(const Size & size)      {SDL_Rect::w = size.width(); SDL_Rect::h = size.height();}

        void setX(Sint16 x)      {SDL_Rect::x = x;}
        void setY(Sint16 y)      {SDL_Rect::y = y;}
        void setWidth (Uint16 w) {SDL_Rect::w = w;}
        void setHeight(Uint16 h) {SDL_Rect::h = h;}

        Point origin() const {return Point(SDL_Rect::x, SDL_Rect::y);}
        Point pos()    const {return Point(SDL_Rect::x, SDL_Rect::y);}
        Size  size()   const {return Size(SDL_Rect::w, SDL_Rect::h);}

        Sint16 x()      const {return SDL_Rect::x;}
        Sint16 y()      const {return SDL_Rect::y;}
        Uint16 width()  const {return SDL_Rect::w;}
        Uint16 height() const {return SDL_Rect::h;}
        Sint16 x2()     const {return SDL_Rect::x + SDL_Rect::w;}
        Sint16 y2()     const {return SDL_Rect::y + SDL_Rect::h;}

        inline bool inside(const Point & p)    const;
        inline bool inside(Uint16 x, Uint16 y) const;


        // Overloaded operator =. A deep copy will done, so it's save to assign Rect to itself.
        //Rect & operator=(const Rect & r)
        //  {if (this == &r) return *this; mOrigin=r.origin(); mSize=r.size(); return *this;}
        // Overloaded operator ==. Comparsion between Rects are save.
        //bool operator==(const Rect & r) const
        //  {return (mSize == r.size() && mOrigin == r.pos() ) ? true : false;}
        // Overloaded operator !=. Comparsion between Rects are save.
        //bool operator!=(const Rect & r) const
        //  {return (mSize != r.size() && mOrigin != r.pos() ) ? true : false;}
    };

    inline bool Rect::inside(const Point & p) const
    {
        if ((p.x() >= SDL_Rect::x)
         && (p.y() <= SDL_Rect::y)
         && (p.x() <= SDL_Rect::x + SDL_Rect::w)
         && (p.y() >= SDL_Rect::y + SDL_Rect::h)) return true;
        else return false;
    }

    inline bool Rect::inside(Uint16 x, Uint16 y) const
    {
        if ((x >= SDL_Rect::x)
         && (y >= SDL_Rect::y)
         && (x <= SDL_Rect::x + SDL_Rect::w)
         && (y <= SDL_Rect::y + SDL_Rect::h)) return true;
        else return false;
    }

} // sdltk

#endif // SDLTK_RECT_H
