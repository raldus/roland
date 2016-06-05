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
#ifndef MEMMAN_H
#define MEMMAN_H

#include "def.h"
#include "gatearray.h"
#include "z80.h"

/** @author Fred Klaus */
class MemMan final
{

public:
    MemMan(Z80* z80 = nullptr, GateArray* gatearray = nullptr,
            const tSTRING & cpcrom = "", const tSTRING & amsdos = "");
    ~MemMan() = default;

    enum RamSize : tUWORD {ram64=64, ram128=128, ram256=256, ram512=512};

    enum Error   : tUBYTE {ErrRamSize=1, ErrMemory=2, ErrCpcRom=4, ErrAmsdos=8};

    int init(tUWORD ramsize = 128, const tSTRING & cpcrom = "", const tSTRING & amsdos = "");
    int init(Z80* z80, GateArray* gatearray);

    inline void initBanking();
    void memoryManager();

    inline void toggleLowerRom();
    inline void toggleUpperRom();

    tUBYTE* memBankConfig(tUBYTE bank, tUBYTE seg) {return mMemBankConfig[bank][seg];}
    tUBYTE* rom(int bank) {return mRom[bank];}

    tUBYTE* upperRom() {return mUpperRom;}
    tUBYTE* lowerRom() {return mLowerRom;}

    tUBYTE* base() {return mMemBankConfig[0][0];}

    bool openRom(int idx, const tSTRING & filename);
    bool openCpcRom(const tSTRING & filename);

private:
    GateArray* mGateArray;
    Z80* mZ80;

    tUBYTE* mRam;
    tUBYTE* mRom[256];
    tUBYTE  mCpcRom[2*16384];
    tUBYTE* mMemBankConfig[8][4];

    tUBYTE* mUpperRom;
    tUBYTE* mLowerRom;

    int mRamSize;

};

inline void MemMan::toggleUpperRom()
{

    if (!(mGateArray->romConfig() & 0x08))
    {
        if ((mGateArray->upperRom() == 0) || (mRom[mGateArray->upperRom()] == 0))
        {
            mZ80->setMembank_read(3, mUpperRom);
        }
        else
        {
            mZ80->setMembank_read(3, mRom[mGateArray->upperRom()]);
        }
    }
}

inline void MemMan::toggleLowerRom()
{
    if (!(mGateArray->romConfig() & 0x04))
    {
        mZ80->setMembank_read(0, mLowerRom);
    }
}

#endif
