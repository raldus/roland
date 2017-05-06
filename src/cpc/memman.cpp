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
#include "memman.h"

#include <memory>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::string;

namespace cpcx
{

    MemMan::MemMan(Z80 *z80, GateArray *gatearray, const string &cpcrom,
                   const string &amsdos)
    {
        mGateArray = gatearray;
        mZ80 = z80;

        mRam = 0;
        mRamSize = 0;
        mLowerRom = mCpcRom;
        mUpperRom = mCpcRom + 0x4000;

        for (int i = 0; i < 256; i++)
            mRom[i] = 0;

        init();
    }

    int MemMan::init(Z80 *z80, GateArray *gatearray)
    {
        mGateArray = gatearray;
        mZ80 = z80;
        return init();
    }

    int MemMan::init(tUWORD ramsize, const string & cpcrom, const string & amsdos)
    {
        int ret = 0;
        mRamSize = ramsize;
        if ((ramsize < 64) || (ramsize > 572))
            return ErrRamSize; // @todo (type6128 < 128kb) ram error.
        if (ramsize % 64)
            return ErrRamSize;

        if (mRam)
            delete[] mRam;
        mRam = new tUBYTE[mRamSize * 1024];
        if (!mRam)
            return ErrMemory;
        memset(mRam, 0, mRamSize * 1024);

        for (int i = 0; i < 256; i++)
        {
            if (mRom[i])
                delete[] mRom[i];
            mRom[i] = 0;
        }

        if (!openCpcRom(cpcrom))
            ret |= ErrCpcRom;
        if (!openRom(7, amsdos))
            ret |= ErrAmsdos;
        if (ret)
            return ret;

        initBanking();

        // toggleRam();
        return ret;
    }

    bool MemMan::openRom(int idx, const string &filename)
    {
        if (filename.empty())
            return false;
#ifdef _WIN32
        ifstream in(filename.c_str(), std::ios::binary); // @todo size checking !
#else
        ifstream in(filename.c_str());
#endif
        if (!in)
        {
           EOUT("openRom", "ROM", "not found");
            return false;
        }
        if (mRom[idx])
        {
            delete[] mRom[idx];
            mRom[idx] = 0;
        }
        mRom[idx] = new tUBYTE[16384]; // @todo all new statements should be checked
                                      // for enough mem!!
        in.read((char *)mRom[idx], 16384);
        in.close();
        return true;
    }

    bool MemMan::openCpcRom(const string &filename)
    {
        if (filename.empty())
            return false;
#ifdef _WIN32
        ifstream in(filename.c_str(), std::ios::binary); // @todo size checking !
#else
        ifstream in(filename.c_str());
#endif
        if (!in)
        {
            EOUT("openCpcRom", "CPC ROM", "not found");
            return false;
        }
        memset(mCpcRom, 0, 2 * 16384);
        in.read((char *)mCpcRom, 2 * 16384);
        in.close();
        return true;
    }

    inline void MemMan::initBanking()
    {
        tUBYTE *rom0, *rom1, *rom2, *rom3, *rom4, *rom5, *rom6, *rom7;
        tUBYTE *rambank;

        rom0 = mRam;
        rom1 = mRam + 1 * 16384;
        rom2 = mRam + 2 * 16384;
        rom3 = mRam + 3 * 16384;

        IOUT("initBanking", "RAM Bank:", (int) mGateArray->ramBank());

        rambank = mRam + ((mGateArray->ramBank() + 1) * (4 * 16384)); // 64 KB
        rom4 = rambank;
        rom5 = rambank + 1 * 16384;
        rom6 = rambank + 2 * 16384;
        rom7 = rambank + 3 * 16384;

        mMemBankConfig[0][0] = rom0;
        mMemBankConfig[0][1] = rom1;
        mMemBankConfig[0][2] = rom2;
        mMemBankConfig[0][3] = rom3;

        mMemBankConfig[1][0] = rom0;
        mMemBankConfig[1][1] = rom1;
        mMemBankConfig[1][2] = rom2;
        mMemBankConfig[1][3] = rom7;

        mMemBankConfig[2][0] = rom4;
        mMemBankConfig[2][1] = rom5;
        mMemBankConfig[2][2] = rom6;
        mMemBankConfig[2][3] = rom7;

        mMemBankConfig[3][0] = rom0;
        mMemBankConfig[3][1] = rom3;
        mMemBankConfig[3][2] = rom2;
        mMemBankConfig[3][3] = rom7;

        mMemBankConfig[4][0] = rom0;
        mMemBankConfig[4][1] = rom4;
        mMemBankConfig[4][2] = rom2;
        mMemBankConfig[4][3] = rom3;

        mMemBankConfig[5][0] = rom0;
        mMemBankConfig[5][1] = rom5;
        mMemBankConfig[5][2] = rom2;
        mMemBankConfig[5][3] = rom3;

        mMemBankConfig[6][0] = rom0;
        mMemBankConfig[6][1] = rom6;
        mMemBankConfig[6][2] = rom2;
        mMemBankConfig[6][3] = rom3;

        mMemBankConfig[7][0] = rom0;
        mMemBankConfig[7][1] = rom7;
        mMemBankConfig[7][2] = rom2;
        mMemBankConfig[7][3] = rom3;
    }

    void MemMan::memoryManager()
    {
        tUBYTE membank;
        if (mRamSize == 64) // 64KB of RAM?
        {
            membank = 0;                 // no expansion memory
            mGateArray->setRamConfig(0); // the only valid configuration is 0
        }
        else
        {
            membank = ((mGateArray->ramConfig() >> 3) & 7); // extract expansion memory bank
            if (((membank + 1) * 64) > mRamSize) // @todo selection is beyond available memory? ### +2 ###
            {
                membank = 0; // force default mapping
            }
        }
        if (membank != mGateArray->ramBank()) // requested bank is different from the active one?
        {
            IOUT("memoryManager", "RAM Bank:", (int) membank);
            mGateArray->setRamBank(membank);
            initBanking();
        }

        mZ80->setMembank_read(0, mMemBankConfig[mGateArray->ramConfig() & 7][0]);
        mZ80->setMembank_read(1, mMemBankConfig[mGateArray->ramConfig() & 7][1]);
        mZ80->setMembank_read(2, mMemBankConfig[mGateArray->ramConfig() & 7][2]);
        mZ80->setMembank_read(3, mMemBankConfig[mGateArray->ramConfig() & 7][3]);

        mZ80->setMembank_write(0, mMemBankConfig[mGateArray->ramConfig() & 7][0]);
        mZ80->setMembank_write(1, mMemBankConfig[mGateArray->ramConfig() & 7][1]);
        mZ80->setMembank_write(2, mMemBankConfig[mGateArray->ramConfig() & 7][2]);
        mZ80->setMembank_write(3, mMemBankConfig[mGateArray->ramConfig() & 7][3]);

        toggleUpperRom();
        toggleLowerRom();
    }

} // cpc
