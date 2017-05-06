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

#include "compspec.h"
#include "types.h"

namespace cpcx
{

    //! The 8255 Parallel Peripheral Interface
    //! Parallel Peripheral Interface\n\n @htmlinclude  8255.html
    class Ppi ROLAND_FINAL
    {

    public:
        enum FlagB //!< (DIP-switch, do not combine manufacturer)
        {
            Triumph        = 2,      ///< Manufacturer Triumph
            Saisho         = 4 ,     ///< Manufacturer Saisho
            Solavox        = 6 ,     ///< Manufacturer Solavox
            Awa            = 8 ,     ///< Manufacturer Awa
            Schneider      = 10 ,    ///< Manufacturer Schneider
            Orion          = 12 ,    ///< Manufacturer Orion
            Amstrad        = 14,     ///< Manufacturer Amstrad
            VSyncOccured   = 1,      ///< VSync (internal)
            Refresh50Hz    = 16,     ///< 50Hz instead of 60Hz refresh rate (DIP-switch)
            Expansion      = 32,     ///< No expansion Peripherals available (internal)
            PrnterNotReady = 64,     ///< Printer is not ready (internal)
            TapeRead       = 128     ///< Tape will be read (internal)
        };

        Ppi()  {init();}
        ~Ppi() ROLAND_DEFAULT

        void init();                                   ///< set initial values

        tUBYTE portA()   const {return mPortA;}               ///< get Port A
        tUBYTE portB()   const {return mPortB;}               ///< get Port B
        tUBYTE portC()   const {return mPortC;}               ///< get Port C
        tUBYTE control() const {return mControl;}             ///< get control bits
        tUBYTE jumpers() const {return mJumpers;}             ///< get jumpers

        void setA(tUBYTE val)       {mPortA   = val;}   ///< set Port A
        void setB(tUBYTE val)       {mPortB   = val;}   ///< set Port B
        void setC(tUBYTE val)       {mPortC   = val;}   ///< set Port C
        void setControl(tUBYTE val) {mControl = val;}   ///< set control bits
        void setJumpers(tUBYTE val) {mJumpers = val;}   ///< set jumpers

        void addB(tUBYTE flags)     {mPortB |= flags;}  ///< add a flag to Port B
        void removeB(tUBYTE flags)  {mPortB &= ~flags;} ///< remove a flag from Port B

    private:
        tUBYTE mPortA;
        tUBYTE mPortB;
        tUBYTE mPortC;
        tUBYTE mControl;

        tUBYTE mJumpers;
    };

} // cpc

#endif // CPC_PPI_H
