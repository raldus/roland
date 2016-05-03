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
#ifndef CPC_H
#define CPC_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "types.h"
#include "prefs.h"

#ifdef ENABLE_RAZE
#include "raze.h"
#endif

#include "z80.h"
#include "crtc.h"
#include "ppi.h"
#include "fdc.h"
#include "psg.h"
#include "gatearray.h"
#include "memman.h"
#include "keyboard.h"
#include "colours.h"
#include "sound.h"
#include "vdu.h"


#define MIN_SPEED_SETTING 2
#define MAX_SPEED_SETTING 32
#define DEF_SPEED_SETTING 4

// number of CPU cycles per frame = 4MHz divided by 50Hz
#define CYCLE_COUNT_INIT 80000

#define mPsg_write \
{ \
   tUBYTE control = mPsg.control() & 0xc0; /* isolate PSG control bits */ \
   if (control == 0xc0) { /* latch address? */ \
      mPsg.setSelected(mPsg_data); /* select new PSG register */ \
} else if (control == 0x80) { /* write? */ \
      if (mPsg.selected() < 16) { /* valid register? */ \
         mSound.setAYRegister(mPsg.selected(), mPsg_data); \
} \
} \
}



/** @author Fred Klaus */
class Cpc
{

public:
    enum CpcType {cpc464=0, cpc664=1, cpc6128=2};
    enum RamSize {ram64=64, ram128=128, ram256=256, ram512=512};
    //enum Monitor {colour=0, green=1, grey=2};


    Cpc(Prefs* prefs);
    ~Cpc() {}

    int init(Prefs* prefs=0);

    void setSpeed(uint value) {mSpeed=value;}

    uint    speed()   {return mSpeed;}
    //Monitor monitor() {return mMonitor;}

    tUBYTE z80_in_handler (tREGPAIR port); //@todo change This !!
    void  z80_out_handler(tREGPAIR port, tUBYTE value);
    void waitstates();


    Z80       & z80()       {return mZ80;}
    Ppi       & ppi()       {return mPpi;}
    Fdc       & fdc()       {return mFdc;}
    Psg       & psg()       {return mPsg;}
    Vdu       & vdu()       {return mVdu;}
    Crtc      & crtc()      {return mCrtc;}
    Sound     & sound()     {return mSound;}
    MemMan    & memman()    {return mMemman;}
    Colours   & colours()   {return mColours;}
    Keyboard  & keyboard()  {return mKeyboard;}
    GateArray & gatearray() {return mGatearray;}

private:
    CpcType   mCpcType;
    //Monitor  mMonitor;

    uint      mSpeed;
    uint      mBpp;

    Prefs*    mPrefs;

    Z80       mZ80; //@todo change this !!!!!!!!!!!!!
    Crtc      mCrtc;
    Ppi       mPpi;
    Fdc       mFdc;
    Psg       mPsg;
    GateArray mGatearray;
    Keyboard  mKeyboard;
    Colours   mColours;
    MemMan    mMemman;
    Sound     mSound;
    Vdu       mVdu;

};

#endif
