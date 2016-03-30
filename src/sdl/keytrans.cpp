/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus   *
 *   frednet@web.de   *
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
#include "keytrans.h"

#ifdef _WIN32
    UBYTE KeyTrans::mTable[320] =
    {
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // BKSPC   TAB                             ENTER
           0x97,   0x84,   0xff,   0xff,   0xff,   0x22,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //                         ESC
           0xff,   0xff,   0xff,   0x82,   0xff,   0xff,   0xff,   0xff,
        // SPACE           3                               1
           0x57,   0xff,   0x71,   0xff,   0xff,   0xff,   0x80,   0xff, // 39
        // 5                               ,       -       .       /
           0x61,   0xff,   0xff,   0xff,   0x47,   0x31,   0x37,   0x36,
        // 0       1       2       3       4       5       6       7
           0x40,   0x80,   0x81,   0x71,   0x70,   0x61,   0x60,   0x51,
        // 8       9               ;               =        >
           0x50,   0x41,   0xff,   0x35,   0xff,   0x30,   0xff,   0xff,
        //   @
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 79
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //                         [       \       ]               8
           0xff,   0xff,   0xff,   0x32,   0x23,   0x21,   0xff,   0x50,
        //         A       B       C       D       E       F       G
           0xff,   0x85,   0x66,   0x76,   0x75,   0x72,   0x65,   0x64,
        // H       I       J       K       L       M       N       O
           0x54,   0x43,   0x55,   0x45,   0x44,   0x46,   0x56,   0x42,
        // P       Q       R       S       T       U       V       W
           0x33,   0x83,   0x62,   0x74,   0x63,   0x52,   0x67,   0x73, // 119
        // X       Y       Z                                      DEL/CLR
           0x77,   0x53,   0x87,   0xff,   0xff,   0xff,   0xff,   0x20,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 159
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // BKSPC   TAB                             ENTER
           0x97,   0x84,   0xff,   0xff,   0xff,   0x22,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 199
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // 0                                                       9
           0x40,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0x41,
        // 7       2
           0x51,   0x81,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 239
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // NP0     NP1     NP2     NP3     NP4     NP5     NP6     NP7
           0x17,   0x15,   0x16,   0x05,   0x24,   0x14,   0x04,   0x12,
        // NP8     NP9     NP.     NP/     NP*     NP-     NP+     NPENT
           0x13,   0x03,   0x07,   0xff,   0xff,   0xff,   0xff,   0x06,
        //         CURUP   CURDN   CURRT   CURLF   INS     HOME    END
           0xff,   0x00,   0x02,   0x01,   0x10,   0x20,   0xff,   0xff, // 279
        // PGUP    PGDWN   F1      F2      F3      F4      F5      F6
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // F7      F8      F9      F10     F11     F12
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //                                 NUMLK   CPSLK   SCRLK   RSHFT
           0xff,   0xff,   0xff,   0xff,   0xff,   0x86,   0xff,   0x25,
        // LSHFT   RCTRL   LCTRL           LALT
           0x25,   0xff,   0x27,   0xff,   0x11,   0xff,   0xff,   0xff,
        //         RALT                    SYSRQ
           0xff,   0x26,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff
    };

#else // *** Linux

    UBYTE KeyTrans::mTable[320] =
    {
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // BKSPC   TAB                             ENTER
           0x97,   0x84,   0xff,   0xff,   0xff,   0x22,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //                         ESC
           0xff,   0xff,   0xff,   0x82,   0xff,   0xff,   0xff,   0xff,
        // SPACE           3                               1
           0x57,   0xff,   0x71,   0xff,   0xff,   0xff,   0x80,   0xff, // 39
        // 5                               ,       -       .       /
           0x61,   0xff,   0xff,   0xff,   0x47,   0x31,   0x37,   0x36,
        // 0       1       2       3       4       5       6       7
           0x40,   0x80,   0x81,   0x71,   0x70,   0x61,   0x60,   0x51,
        // 8       9               ;               =        >
           0x50,   0x41,   0xff,   0x35,   0xff,   0x30,   0xff,   0xff,
        //   @
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 79
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //                         [       \       ]               8
           0xff,   0xff,   0xff,   0x32,   0x23,   0x21,   0xff,   0x50,
        //         A       B       C       D       E       F       G
           0xff,   0x85,   0x66,   0x76,   0x75,   0x72,   0x65,   0x64,
        // H       I       J       K       L       M       N       O
           0x54,   0x43,   0x55,   0x45,   0x44,   0x46,   0x56,   0x42,
        // P       Q       R       S       T       U       V       W
           0x33,   0x83,   0x62,   0x74,   0x63,   0x52,   0x67,   0x73, // 119
        // X       Y       Z                                      DEL/CLR
           0x77,   0x53,   0x87,   0xff,   0xff,   0xff,   0xff,   0x20,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 159
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // BKSPC   TAB                             ENTER
           0x97,   0x84,   0xff,   0xff,   0xff,   0x22,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 199
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // 0                                                       9
           0x40,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0x41,
        // 7       2
           0x51,   0x81,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff, // 239
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // NP0     NP1     NP2     NP3     NP4     NP5     NP6     NP7
           0x17,   0x15,   0x16,   0x05,   0x24,   0x14,   0x04,   0x12,
        // NP8     NP9     NP.     NP/     NP*     NP-     NP+     NPENT
           0x13,   0x03,   0x07,   0xff,   0xff,   0xff,   0xff,   0x06,
        //         CURUP   CURDN   CURRT   CURLF   INS     HOME    END
           0xff,   0x00,   0x02,   0x01,   0x10,   0x11,   0xff,   0xff, // 279
        // PGUP    PGDWN   F1      F2      F3      F4      F5      F6
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        // F7      F8      F9      F10     F11     F12
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,
        //                                 NUMLK   CPSLK   SCRLK   RSHFT
           0xff,   0xff,   0xff,   0xff,   0xff,   0x86,   0xff,   0x25,
        // LSHFT   RCTRL   LCTRL           LALT
           0x25,   0xff,   0x27,   0xff,   0x11,   0xff,   0xff,   0xff,
        //         RALT                    SYSRQ
           0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff,   0xff
    };
#endif

/*
INS = Copycurs - 277 = 0x11 (0x20)
DEL/CLR = CLR  - 127 = 0x20 (0xff)
*/


KeyTrans::KeyTrans()
{
    // @todo load this from config file.
    mJoyAlloc[0].joy = 0x90; // Up
    mJoyAlloc[1].joy = 0x91; // Down
    mJoyAlloc[2].joy = 0x92; // Left
    mJoyAlloc[3].joy = 0x93; // Right
    mJoyAlloc[4].joy = 0x94; // Btn0
    mJoyAlloc[5].joy = 0x95; // Btn1

    mJoyAlloc[0].key = 273;
    mJoyAlloc[1].key = 274;
    mJoyAlloc[2].key = 276;
    mJoyAlloc[3].key = 275;
    mJoyAlloc[4].key = 308;
    mJoyAlloc[5].key = 311;

    mJoyEnabled = false;
    mSeqIndex = 0;
    mSequence[0].keyval = 0;

    for (int i=0; i<6; i++)
    {
        mJoyAlloc[i].orig = mTable[mJoyAlloc[i].key];
    }

#ifdef _WIN32 // patches for german windows
        mTable[121]=0x87;
        mTable[122]=0x53;
#else // patches for german linux
        mTable[252]=0x32;
    #endif
}

bool KeyTrans::toggleJoystick()
{
    if (mJoyEnabled) mJoyEnabled=false;
    else mJoyEnabled=true;

    // @todo change this ... for now curserkeys.
    if (mJoyEnabled)
    {
        for (int i=0; i<6; i++)
        {
            mJoyAlloc[i].orig = mTable[mJoyAlloc[i].key];
            mTable[mJoyAlloc[i].key] = mJoyAlloc[i].joy;
        }
    }
    else
    {
        for (int i=0; i<6; i++)
        {
            mTable[mJoyAlloc[i].key] = mJoyAlloc[i].orig;
        }
    }
    return mJoyEnabled;
}

void KeyTrans::init(Language lang)
{
    // @todo Patch to other languages
}

UBYTE KeyTrans::get(SDL_Event & event)
{
    if (event.key.keysym.sym >= 320)
    {
        return 0xff;
    }
    else
    {
        if (event.key.keysym.mod & KMOD_MODE)
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:

                    if (event.key.keysym.sym == SDLK_LESS)
                    {
                        mSequence[0].keyval = 0x25;
                        mSequence[0].down   = true;
                        mSequence[1].keyval = 0x32;
                        mSequence[1].down   = true;
                        mSequence[2].keyval = 0x00;
                    }
                    break;

                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_LESS)
                    {
                        mSequence[0].keyval = 0x32;
                        mSequence[0].down   = false;
                        mSequence[1].keyval = 0x25;
                        mSequence[1].down   = false;
                        mSequence[2].keyval = 0x00;
                    }
                    break;

                default:
                    break;
            }
        }
        else return mTable[event.key.keysym.sym];
    }
    return 0; // TODO: Here was no return. Check this
}

bool KeyTrans::hasSequence()
{
    if (mSequence[mSeqIndex].keyval != 0x00) return true;
    else
    {
        mSequence[0].keyval=0x00;
        mSeqIndex=0;
        return false;
    }
}

const KeyTrans::SeqPair & KeyTrans::sequenceVal()
{
    uint si=mSeqIndex;
    if (mSequence[mSeqIndex].keyval == 0x00)
    {
        mSequence[0].keyval = 0x00;
        mSeqIndex=0;
        return mSequence[0];
    }
    mSeqIndex++;
    //mSequence[mSeqIndex].keyval = 0;
    return mSequence[si];
}

void KeyTrans::sequenceCatRun()
{
    mSequence[0].keyval = 0x76;
    mSequence[0].down   = true;
    mSequence[1].keyval = 0x76;
    mSequence[1].down   = false;

    mSequence[2].keyval = 0x85;
    mSequence[2].down   = true;
    mSequence[3].keyval = 0x85;
    mSequence[3].down   = false;

    mSequence[4].keyval = 0x63;
    mSequence[4].down   = true;
    mSequence[5].keyval = 0x63;
    mSequence[5].down   = false;

    mSequence[6].keyval = 0x22;
    mSequence[6].down   = true;
    mSequence[7].keyval = 0x22;
    mSequence[7].down   = false;

    mSequence[8].keyval = 0x62;
    mSequence[8].down   = true;
    mSequence[9].keyval = 0x62;
    mSequence[9].down   = false;

    mSequence[10].keyval = 0x52;
    mSequence[10].down   = true;
    mSequence[11].keyval = 0x52;
    mSequence[11].down   = false;

    mSequence[12].keyval = 0x56;
    mSequence[12].down   = true;
    mSequence[13].keyval = 0x56;
    mSequence[13].down   = false;

    mSequence[14].keyval = 0x25;
    mSequence[14].down   = true;
    mSequence[15].keyval = 0x81;
    mSequence[15].down   = true;
    mSequence[16].keyval = 0x81;
    mSequence[16].down   = false;
    mSequence[17].keyval = 0x25;
    mSequence[17].down   = false;

    mSequence[18].keyval = 0x00;
}


