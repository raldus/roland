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

#include "compspec.h"
#include "types.h"

namespace cpcx
{

    //! \author Fred Klaus
    class Colours final
    {

    public:
        Colours() : mDepth(16), mIntensity(10), mMonitor(0) {};
        ~Colours() ROLAND_DEFAULT

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

        static const float  mColour[32][3];
        static const double mGreen [32];

    };

} // cpc

#endif // CPC_COLOURS_H

