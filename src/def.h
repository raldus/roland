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
#include <chrono>
#include <cstdint>

//! Number of CPU cycles per frame = 4MHz divided by 50Hz
static const uint32_t CYCLE_COUNT_INIT  = 80000;
static const uint8_t  TAPE_VOLUME       = 32;
static const uint8_t  MIN_SPEED_SETTING = 2;
static const uint8_t  MAX_SPEED_SETTING = 32;
static const uint8_t  DEF_SPEED_SETTING = 4;
static const uint16_t CPC_SCR_WIDTH     = 1024;     //!< max width
static const uint16_t CPC_SCR_HEIGHT    = 312;      //!< max height
static const uint16_t CPC_VISIBLE_SCR_WIDTH  = 384; //!< visible: 4+40+4 * 8
static const uint16_t CPC_VISIBLE_SCR_HEIGHT = 270; //!< visible: 5+25+4 * 8  \TODO original is 272

using std::boolalpha;

inline double elapsed()
{

    static std::chrono::high_resolution_clock::time_point epoch;
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - epoch;
    // you need to be explicit about your timestamp type
    return std::chrono::duration_cast<std::chrono::duration<double> >(elapsed).count() / 100.0;
}

#ifdef ROLAND_VERBOSE_COLOR
  #define ARST "\x1B[0m"
  #define ARED "\x1B[31m"
  #define AGRN "\x1B[0;32m"
  #define AYEL "\x1B[33m"
  #define ABLU "\x1B[34m"
  #define AMAG "\x1B[35m"
  #define ACYN "\x1B[36m"
  #define AWHT "\x1B[37m"
#else
  #define ARST ""
  #define ARED ""
  #define AGRN ""
  #define AYEL ""
  #define ABLU ""
  #define AMAG ""
  #define ACYN ""
  #define AWHT ""
#endif


#define OUT(a,b,c) << std::fixed << elapsed() << '.' \
    << ABLU << std::right << std::setw(16) << std::setfill('.') \
    << (std::string(__FILE__).substr((std::string(__FILE__).rfind('/')) + 1)) \
    << ":" << std::setw(5) << std::right << std::to_string(__LINE__) << "..." << ARST \
    << std::setw(16) << std::left << a \
    << std::setw(24) << std::left << b << std::setfill(' ') << c << ARST << "\n";

#define EOUT(a,b,c) std::cerr.precision(9); std::cerr OUT(a,b,c)

#ifdef VERBOSE
    #define IOUT(a,b,c) std::cout.precision(9); std::cout OUT(a,b,c)
#else
    #define IOUT(a,b,c)
#endif

#ifdef DEBUG
    #define DOUT(a,b,c) std::cout.precision(9); std::cout OUT(a,b,c)
    #define DBG(a) a
#else
    #define DOUT(a,b,c)
    #define DBG(a)
    #define PREOUT
#endif


#endif // ROLAND_DEF_H
