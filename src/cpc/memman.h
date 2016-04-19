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

#include "types.h"
#include "gatearray.h"
#include "z80.h"

#include <string>

using std::string;

/** @author Fred Klaus */
class MemMan
{

public:
    MemMan(Z80* z80=0, GateArray* gatearray=0, const string & cpcrom="", const string & amsdos="");
    ~MemMan() {}

    enum RamSize {ram64=64, ram128=128, ram256=256, ram512=512};

    enum Error {ErrRamSize=1, ErrMemory=2, ErrCpcRom=4, ErrAmsdos=8};

    int init(int ramsize=128, const string & cpcrom="", const string & amsdos="");
    int init(Z80* z80, GateArray* gatearray);
    
    inline void initBanking();
    void memoryManager();

    inline void toggleLowerRom();
    inline void toggleUpperRom();

    UBYTE* memBankConfig(UBYTE bank, UBYTE seg) {return mMemBankConfig[bank][seg];}
    UBYTE* rom(int bank) {return mRom[bank];}

    UBYTE* upperRom() {return mUpperRom;}
    UBYTE* lowerRom() {return mLowerRom;}

    UBYTE* base() {return mMemBankConfig[0][0];}

    bool openRom(int idx, const string & filename);
    bool openCpcRom(const string & filename);

private:
    GateArray* mGateArray;
    Z80* mZ80;

    UBYTE* mRam;
    UBYTE* mRom[256];
    UBYTE  mCpcRom[2*16384];
    UBYTE* mMemBankConfig[8][4];

    UBYTE* mUpperRom;
    UBYTE* mLowerRom;

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
