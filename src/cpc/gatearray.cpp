/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus                                      *
 *   frednet@web.de                                                        *
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
#include "gatearray.h"

void GateArray::init()
{
    mPen       = 0;
    mRomConfig = 0;
    mRamConfig = 0;
    mRamBank   = 0;
    mUpperRom  = 0;
    mCounter   = 0;
    mSlCount   = 0;
    mIntDelay  = 0;
    mInterrupt = true;

    mMode          = 1;
    mRequestedMode = 1;

    for(UBYTE i=0; i<17; i++) mInk[i]=0;
    for(UBYTE i=0; i<17; i++) mPalette[i]=0;
}


