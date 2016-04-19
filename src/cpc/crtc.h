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
#ifndef CRTC_H
#define CRTC_H

#include "types.h"

#include "vdu.h"
#include "ppi.h"

/// The CRTC6845
/** CRTC6845\n\n @htmlinclude crtc.html
    @author Fred Klaus */
class Crtc
{

public:
    enum Register
    {                                 ///<  PAL   SECAM   NTSC
        HorizontalTotal         =  0, ///<   63    63      63
        HorizontalDisplayed     =  1, ///<   40    40      40
        HorizontalSyncPosition  =  2, ///<   46    46      46
        SyncWidth               =  3, ///< 0x8E  0x8E    0x8E
        VerticalTotal           =  4, ///<   38    38      31
        VerticalTotalAdjust     =  5, ///<    0     0       6
        VerticalDisplayed       =  6, ///<   25    25      25
        VerticalSyncPosition    =  7, ///<   30    30      27
        InterlaceAndSkew        =  8, ///<    0     0       0
        MaximumRasterAddress    =  9, ///<    7     7       7
        CursorStartRaster       = 10, ///<    X     X       X
        CursorEndRaster         = 11, ///<    X     X       X
        StartAddressHigh        = 12, ///<    X     X       X
        StartAddressLow         = 13, ///<    X     X       X
        CursorHigh              = 14, ///<    X     X       X
        CursorLow               = 15, ///<    X     X       X
        LightPenHigh            = 16, ///<    X     X       X
        LightPenLow             = 17  ///<    X     X       X
    };

    enum Flags
    {
        VS     = 1,
        HS     = 2,
        HDT    = 4,
        VDT    = 8,
        HT     = 16,
        VT     = 32,
        MR     = 64,
        VTadj  = 128,
        VSf    = 256
    };

    Crtc() : mSelected(0) {mVdu=0; mPpi=0; init();}
    ~Crtc() {}

    void init(Vdu* vdu=0, Ppi* ppi=0);
    void reset() {init();}

    UBYTE selected() {return mSelected;}

    void  select(UBYTE reg)             {if (reg < 18) mSelected=reg;}
    UBYTE read()                        {return mRegister[mSelected];}
    UBYTE read(UBYTE num)               {return (num < 18) ? mRegister[num] : 0;}
    void  write(UBYTE reg, UBYTE value) {mRegister[reg]=value;}
    void  write(UBYTE value);

    uint flags()   {return mFlags;}
    UBYTE hsw()            {return mHsw;}
    UBYTE hswCount()       {return mHswCount;}
    UBYTE hswActive()      {return mHswActive;}
    UBYTE vsw()            {return mVsw;}
    UBYTE vswCount()       {return mVswCount;}
    UBYTE lastHDisp()      {return mLastHDisp;}
    UBYTE skew()           {return mSkew;}
    UBYTE vtAdjust()       {return mVtAdjust;}
    UBYTE vtAdjustCount()  {return mVtAdjustCount;}
    UBYTE maxRaster()      {return mMaxRaster;}
    UBYTE rasterCount()    {return mRasterCount;}
    UBYTE lineCount()      {return mLineCount;}
    UBYTE charCount()      {return mCharCount;}
    uint requestedAddr()   {return mRequestedAddr;}
    uint addr()            {return mAddr;}

    void setFlags   (uint flags) {mFlags = flags;}
    void addFlags   (uint flags) {mFlags |= flags;}
    void removeFlags(uint flags) {mFlags &= ~flags;}

    //void setHsw(UBYTE val) {write(read(SyncWidth) | (val & 0x0f));}
    //void setVsw(UBYTE val) {write(read(SyncWidth) | (val >> 4));}

    void setHswActive     (UBYTE value) {mHswActive     = value;}
    void setHswCount      (UBYTE value) {mHswCount      = value;}
    void setAddr          (uint value)  {mAddr          = value;}
    void setVtAdjust      (UBYTE value) {mVtAdjust      = value;}
    void setVtAdjustCount (UBYTE value) {mVtAdjustCount = value;}
    void setVswCount      (UBYTE value) {mVswCount      = value;}
    void setLastHDisp     (UBYTE value) {mLastHDisp     = value;}
    void setCharCount     (UBYTE value) {mCharCount     = value;}
    void setLineCount     (UBYTE value) {mLineCount     = value;}
    void setRasterCount   (UBYTE value) {mRasterCount   = value;}
    void setSkew          (UBYTE value) {mSkew          = value;}

    void incVswCount()  {mVswCount++;}
    //void decVswCount()  {--mVswCount;}
    //void incLineCount() {mLineCount++;}

private:
    Vdu* mVdu;
    Ppi* mPpi;
    
    UBYTE mRegister[18];
    UBYTE mSelected;

    uint mFlags;
    UBYTE mHsw;
    UBYTE mHswCount;
    UBYTE mHswActive;
    UBYTE mVsw;
    UBYTE mVswCount;
    UBYTE mLastHDisp;
    UBYTE mSkew;
    UBYTE mVtAdjust;
    UBYTE mVtAdjustCount;
    UBYTE mMaxRaster;
    UBYTE mRasterCount;
    UBYTE mLineCount;
    UBYTE mCharCount;
    uint mRequestedAddr;
    uint mAddr;

};

#endif
