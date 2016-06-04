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
#ifndef ROLAND_DEF_H
#define ROLAND_DEF_H

#include "types.h"
#include "config.h"

#include <iostream>
#include <iomanip>
#include <stdint.h>

//! Number of CPU cycles per frame = 4MHz divided by 50Hz
static constexpr uint32_t CYCLE_COUNT_INIT  = 80000;
static constexpr uint8_t  TAPE_VOLUME       = 32;
static constexpr uint8_t  MIN_SPEED_SETTING = 2;
static constexpr uint8_t  MAX_SPEED_SETTING = 32;
static constexpr uint8_t  DEF_SPEED_SETTING = 4;
static constexpr uint16_t CPC_SCR_WIDTH  = 1024;        //!< max width
static constexpr uint16_t CPC_SCR_HEIGHT = 312;         //!< max height
static constexpr uint16_t CPC_VISIBLE_SCR_WIDTH  = 384; //!< visible: 4+40+4 * 8
static constexpr uint16_t CPC_VISIBLE_SCR_HEIGHT = 270; //!< visible: 5+25+4 * 8  \TODO original is 272

using std::boolalpha;

#define EOUT(a,b,c) std::cerr << std::setw(12) << std::setfill(' ') \
    << std::left << a << "(E) " << std::setw(40) << std::setfill('.') \
    << std::left << b << c << "\n";

#ifdef VERBOSE
    #define IOUT(a,b,c) std::cout << std::setw(12) << std::setfill(' ') \
        << std::left << a << std::setw(40) << std::setfill('.') \
        << std::left << b  << c << "\n";
#else
    #define IOUT(a,b,c)
#endif

#ifdef DEBUG
    #define DOUT(a,b,c)
#else
    #define DOUT(a,b,c)
    #define PREOUT
#endif


#endif // ROLAND_DEF_H
