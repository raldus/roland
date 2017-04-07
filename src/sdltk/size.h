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
#ifndef SDLTK_SIZE_H
#define SDLTK_SIZE_H

#include "SDL.h"

namespace sdltk
{

    //! \brief defines a planar size (width, height)
    //! \author Fred Klaus  development@fkweb.de
    class Size
    {
        public:
            //! Standard Constructor. Initializes everything to 0.
            Size() {}
            //! Constructor. Initializes everything to 0. @param w = width @param h = height
            Size(Uint16 w, Uint16 h) {mWidth = w; mHeight = h;}
            //! Copyconstructor. A deep copy will done, so it's save to init Size with itself.
            Size(const Size & s)     {mWidth = s.width(); mHeight = s.height();}

            //! Standard Destructor. Does nothing
            ~Size() {}

            //! overloaded operator = A deep copy will done, so it's save to assign Size to itself.
            Size & operator=(const Size & s)
                {if (this == &s) return *this; mWidth=s.width(); mHeight=s.height(); return *this;}
            //! overloaded operator == true, if width and height are the same
            bool operator==(const Size & s) const
                {return (mWidth == s.width() && mHeight == s.height()) ? true : false;}
            //! overloaded operator != true, if width and height are different
            bool operator!=(const Size & s) const
                {return (mWidth != s.width() || mHeight != s.height()) ? true : false;}

            //! initializes everything to 0.
            void clear() {mWidth = 0; mHeight = 0;}

            //! \return the width
            Uint16 width()  const {return mWidth;}
            //! \return the height
            Uint16 height() const {return mHeight;}

            void setWidth (Uint16 w) {mWidth  = w;}
            void setHeight(Uint16 h) {mHeight = h;}
            void set(const Size & s) {mWidth = s.width(); mHeight = s.height();}
            void set(Uint16 w, Uint16 h) {mWidth = w; mHeight = h;}
            void set(const SDL_Surface * surface) {mWidth = surface->w; mHeight = surface->h;}

        private:
            Uint16 mWidth;
            Uint16 mHeight;
    };

} // sdltk


#endif // SDLTK_SIZE_H
