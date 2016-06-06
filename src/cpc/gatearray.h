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
#ifndef CPC_GATEARRAY_H
#define CPC_GATEARRAY_H

#include "types.h"

namespace cpcx
{

//! Gate Array\n\n @htmlinclude gatearray.html
//! \author Fred Klaus
class GateArray final
{

    public:
        GateArray()  {init();}
        ~GateArray() = default;  ///< does nothing

        void init();

        tUBYTE  pen()               const {return mPen;}
        tUBYTE* ink()                     {return mInk;}
        tUBYTE  ink(tUBYTE num)     const {return mInk[num];}
        tUDWORD palette(tUBYTE num) const {return mPalette[num];}

        tUBYTE romConfig()         const {return mRomConfig;}
        tUBYTE ramConfig()         const {return mRamConfig;}
        tUBYTE ramBank()           const {return mRamBank;}
        tUBYTE upperRom()          const {return mUpperRom;}
        tUBYTE counter()           const {return mCounter;}
        tUBYTE mode()              const {return mMode;}
        tUBYTE requestedMode()     const {return mRequestedMode;}
        bool  interrupt()          const {return mInterrupt;}
        tUBYTE intDelay()          const {return mIntDelay;}
        tUBYTE slCount()           const {return mSlCount;}

        void setPen(tUBYTE pen)               {mPen = pen;}
        //void setInk(tUBYTE* ink) {for (int i=0; i<17; i++) mInk[i] = ink[i];}
        void setInk(tUBYTE ink)               {mInk[mPen] = ink;}
        void setInk(tUBYTE num, tUBYTE ink)    {mInk[num]  = ink;}

        void setPalette(tUBYTE num, uint col) {mPalette[num]  = col;}
        void setPalette(uint col)            {mPalette[mPen] = col;}

        void setRomConfig(tUBYTE romcfg)      {mRomConfig = romcfg;}
        void setRamConfig(tUBYTE ramcfg)      {mRamConfig = ramcfg;}
        void setRamBank  (tUBYTE ramcfg)      {mRamBank   = ramcfg;}
        void setUpperRom (tUBYTE ramcfg)      {mUpperRom  = ramcfg;}

        void setMode(tUBYTE mode)             {mMode = mode;}
        void setRequestedMode(tUBYTE mode)    {mRequestedMode = mode;}

        void setInterrupt(bool enabled)      {mInterrupt = enabled;}
        void setIntDelay (tUBYTE value)       {mIntDelay  = value;}
        void setSlCount  (tUBYTE value)       {mSlCount   = value;}

    private:
        tUBYTE mRomConfig;
        tUBYTE mRamConfig;
        tUBYTE mRamBank;
        tUBYTE mUpperRom;
        tUBYTE mCounter;
        tUBYTE mMode;
        tUBYTE mRequestedMode;
        tUBYTE mSlCount;

        tUBYTE   mPen;
        tUBYTE   mInk[17];
        tUDWORD  mPalette[17];

        bool  mInterrupt;
        tUBYTE mIntDelay;

    };

}; // cpc

#endif // CPC_GATEARRAY_H
