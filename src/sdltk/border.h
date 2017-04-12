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
#ifndef SDLTK_BORDER_H
#define SDLTK_BORDER_H

#include "compspec.h"
#include "def.h"
#include "SDL.h"

namespace sdltk
{

    //! A Border represented as 8 Rects. 4 sides and 4 corners.
    class Border
    {

    public:
        enum BorderPos
        {
            bpTop          = 0, //!< top side
            bpBottom       = 1, //!< bottom side
            bpLeft         = 2, //!< left side
            bpRight        = 3, //!< right side
            bpLeftTop      = 4, //!< left-top corner
            bpLeftBottom   = 5, //!< left-bottom corner
            bpRightTop     = 6, //!< right-top corner
            bpRightBottom  = 7, //!< right-bottom corner
        };

        //! Standard constructor. Initializes mColor.
        Border() : mColor(0xffffffff) {};
        //! Default destructor
        ~Border() ROLAND_DEFAULT

        //! Sets the Border color
        void setColor(uint color);
        //! Set the Border size & thickness
        void setUniSize(uint width, uint height, uint thickness);

        //! The paint command for the gui
        void paint(SDL_Surface * surf);

    protected:
        uint     mColor;
        SDL_Rect mBorder[8];
    };

} // sdltk

#endif //  SDLTK_BORDER_H
