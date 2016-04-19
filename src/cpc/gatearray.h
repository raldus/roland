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
#ifndef GATEARRAY_H
#define GATEARRAY_H

#include "types.h"

/// Gate Array
/** Gate Array\n\n @htmlinclude gatearray.html
    @author Fred Klaus */
class GateArray
{

public:
    GateArray()  {init();}
    ~GateArray() {}           ///< does nothing

    void init();

    UBYTE  pen()              {return mPen;}
    UBYTE* ink()              {return mInk;}
    UBYTE  ink(UBYTE num)     {return mInk[num];}
    UDWORD palette(UBYTE num) {return mPalette[num];}

    UBYTE romConfig()         {return mRomConfig;}
    UBYTE ramConfig()         {return mRamConfig;}
    UBYTE ramBank()           {return mRamBank;}
    UBYTE upperRom()          {return mUpperRom;}
    UBYTE counter()           {return mCounter;}
    UBYTE mode()              {return mMode;}
    UBYTE requestedMode()     {return mRequestedMode;}
    bool  interrupt()         {return mInterrupt;}
    UBYTE intDelay()          {return mIntDelay;}
    UBYTE slCount()           {return mSlCount;}

    void setPen(UBYTE pen)               {mPen = pen;}
    //void setInk(UBYTE* ink) {for (int i=0; i<17; i++) mInk[i] = ink[i];}
    void setInk(UBYTE ink)               {mInk[mPen] = ink;}
    void setInk(UBYTE num, UBYTE ink)    {mInk[num]  = ink;}

    void setPalette(UBYTE num, uint col) {mPalette[num]  = col;}
    void setPalette(uint col)            {mPalette[mPen] = col;}

    void setRomConfig(UBYTE romcfg)      {mRomConfig = romcfg;}
    void setRamConfig(UBYTE ramcfg)      {mRamConfig = ramcfg;}
    void setRamBank  (UBYTE ramcfg)      {mRamBank   = ramcfg;}
    void setUpperRom (UBYTE ramcfg)      {mUpperRom  = ramcfg;}

    void setMode(UBYTE mode)             {mMode = mode;}
    void setRequestedMode(UBYTE mode)    {mRequestedMode = mode;}

    void setInterrupt(bool enabled)      {mInterrupt = enabled;}
    void setIntDelay (UBYTE value)       {mIntDelay  = value;}
    void setSlCount  (UBYTE value)       {mSlCount   = value;}

private:
    UBYTE mRomConfig;
    UBYTE mRamConfig;
    UBYTE mRamBank;
    UBYTE mUpperRom;
    UBYTE mCounter;
    UBYTE mMode;
    UBYTE mRequestedMode;
    UBYTE mSlCount;
    
    UBYTE   mPen;
    UBYTE   mInk[17];
    UDWORD  mPalette[17];

    bool  mInterrupt;
    UBYTE mIntDelay;

};

#endif
