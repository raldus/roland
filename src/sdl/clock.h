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
#ifndef ROLAND_CLOCK_H
#define ROLAND_CLOCK_H

#include "SDL.h"

namespace sdltk
{

//! Time measurement in ms
class Clock final
{

public:
    //! Standardconstructor
    Clock() : mLast(SDL_GetTicks()) {};
    //! Standarddestructor
    ~Clock() = default;

    //! Initialize the clock. Like "Set to 0".
    void init() {mLast = SDL_GetTicks();}
    //! Returns the time ellapsed sinze init() in ms
    uint elapsed() const {return SDL_GetTicks() - mLast;}

private:
    uint mLast;

};

}; // sdltk

#endif // ROLAND_CLOCK_H
