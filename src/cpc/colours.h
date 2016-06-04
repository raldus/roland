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
#ifndef CPC_COLOURS_H
#define CPC_COLOURS_H

#include "types.h"

//! \author Fred Klaus
class Colours final
{

public:
    Colours() : mDepth(16), mIntensity(10), mMonitor(0) {};
    ~Colours() = default;

    void init();

    void setIntensity(uint intensity) {mIntensity = intensity; init();}
    void setDepth    (uint depth)     {mDepth     = depth;     init();}
    void setMonitor  (uint monitor)   {mMonitor   = monitor;   init();}

    tUDWORD get(int num) const {return mTable[num];}

private:
    uint mDepth;
    uint mIntensity;
    uint mMonitor;

    tUDWORD mTable [32];

    static constexpr float mColour[32][3] =
    {
        {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5},
        {0.0, 0.0, 0.5}, {1.0, 0.0, 0.5}, {0.0, 0.5, 0.5}, {1.0, 0.5, 0.5},
        {1.0, 0.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0},
        {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 0.5, 0.0}, {1.0, 0.5, 1.0},
        {0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 1.0},
        {0.5, 0.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 1.0, 0.0}, {0.5, 1.0, 1.0},
        {0.5, 0.0, 0.0}, {0.5, 0.0, 1.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 1.0}
    };

    static constexpr double mGreen[32] =
    {
        0.5647, 0.5647, 0.7529, 0.9412, 0.1882, 0.3765, 0.4706, 0.6588,
        0.3765, 0.9412, 0.9098, 0.9725, 0.3451, 0.4078, 0.6275, 0.6902,
        0.1882, 0.7529, 0.7216, 0.7843, 0.1569, 0.2196, 0.4392, 0.5020,
        0.2824, 0.8471, 0.8157, 0.8784, 0.2510, 0.3137, 0.5333, 0.5961
    };

};

#endif // CPC_COLOURS_H
