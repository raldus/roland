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
#ifndef PPI_H
#define PPI_H

#include "types.h"

//! The 8255 Parallel Peripheral Interface
//! Parallel Peripheral Interface\n\n @htmlinclude  8255.html
class Ppi
{

public:
    enum FlagB
    {
        Triumph        = 2,      ///< Manufacturer Triumph   (DIP-switch, do not combine with other Man.)
        Saisho         = 4 ,     ///< Manufacturer Saisho    (DIP-switch, do not combine with other Man.)
        Solavox        = 6 ,     ///< Manufacturer Solavox   (DIP-switch, do not combine with other Man.)
        Awa            = 8 ,     ///< Manufacturer Awa       (DIP-switch, do not combine with other Man.)
        Schneider      = 10 ,    ///< Manufacturer Schneider (DIP-switch, do not combine with other Man.)
        Orion          = 12 ,    ///< Manufacturer Orion     (DIP-switch, do not combine with other Man.)
        Amstrad        = 14,     ///< Manufacturer Amstrad   (DIP-switch, do not combine with other Man.)
        VSyncOccured   = 1,      ///< VSync (internal)
        Refresh50Hz    = 16,     ///< 50Hz instead of 60Hz refresh rate (DIP-switch)
        Expansion      = 32,     ///< No expansion Peripherals available (internal)
        PrnterNotReady = 64,     ///< Printer is not ready (internal)
        TapeRead       = 128     ///< Tape will be read (internal)
    };

    Ppi()  {init();}
    ~Ppi() {}

    void init();                                   ///< set initial values

    UBYTE portA()   {return mPortA;}               ///< get Port A
    UBYTE portB()   {return mPortB;}               ///< get Port B
    UBYTE portC()   {return mPortC;}               ///< get Port C
    UBYTE control() {return mControl;}             ///< get control bits
    UBYTE jumpers() {return mJumpers;}             ///< get jumpers

    void setA(UBYTE val)       {mPortA   = val;}   ///< set Port A
    void setB(UBYTE val)       {mPortB   = val;}   ///< set Port B
    void setC(UBYTE val)       {mPortC   = val;}   ///< set Port C
    void setControl(UBYTE val) {mControl = val;}   ///< set control bits
    void setJumpers(UBYTE val) {mJumpers = val;}   ///< set jumpers

    void addB(UBYTE flags)     {mPortB |= flags;}  ///< add a flag to Port B
    void removeB(UBYTE flags)  {mPortB &= ~flags;} ///< remove a flag from Port B

private:
    UBYTE mPortA;
    UBYTE mPortB;
    UBYTE mPortC;
    UBYTE mControl;

    UBYTE mJumpers;
};

#endif //PPI_H
