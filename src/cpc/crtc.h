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

    tUBYTE selected() {return mSelected;}

    void  select(tUBYTE reg)             {if (reg < 18) mSelected=reg;}
    tUBYTE read()                        {return mRegister[mSelected];}
    tUBYTE read(tUBYTE num)               {return (num < 18) ? mRegister[num] : 0;}
    void  write(tUBYTE reg, tUBYTE value) {mRegister[reg]=value;}
    void  write(tUBYTE value);

    uint flags()   {return mFlags;}
    tUBYTE hsw()            {return mHsw;}
    tUBYTE hswCount()       {return mHswCount;}
    tUBYTE hswActive()      {return mHswActive;}
    tUBYTE vsw()            {return mVsw;}
    tUBYTE vswCount()       {return mVswCount;}
    tUBYTE lastHDisp()      {return mLastHDisp;}
    tUBYTE skew()           {return mSkew;}
    tUBYTE vtAdjust()       {return mVtAdjust;}
    tUBYTE vtAdjustCount()  {return mVtAdjustCount;}
    tUBYTE maxRaster()      {return mMaxRaster;}
    tUBYTE rasterCount()    {return mRasterCount;}
    tUBYTE lineCount()      {return mLineCount;}
    tUBYTE charCount()      {return mCharCount;}
    uint requestedAddr()   {return mRequestedAddr;}
    uint addr()            {return mAddr;}

    void setFlags   (uint flags) {mFlags = flags;}
    void addFlags   (uint flags) {mFlags |= flags;}
    void removeFlags(uint flags) {mFlags &= ~flags;}

    //void setHsw(tUBYTE val) {write(read(SyncWidth) | (val & 0x0f));}
    //void setVsw(tUBYTE val) {write(read(SyncWidth) | (val >> 4));}

    void setHswActive     (tUBYTE value) {mHswActive     = value;}
    void setHswCount      (tUBYTE value) {mHswCount      = value;}
    void setAddr          (uint value)  {mAddr          = value;}
    void setVtAdjust      (tUBYTE value) {mVtAdjust      = value;}
    void setVtAdjustCount (tUBYTE value) {mVtAdjustCount = value;}
    void setVswCount      (tUBYTE value) {mVswCount      = value;}
    void setLastHDisp     (tUBYTE value) {mLastHDisp     = value;}
    void setCharCount     (tUBYTE value) {mCharCount     = value;}
    void setLineCount     (tUBYTE value) {mLineCount     = value;}
    void setRasterCount   (tUBYTE value) {mRasterCount   = value;}
    void setSkew          (tUBYTE value) {mSkew          = value;}

    void incVswCount()  {mVswCount++;}
    //void decVswCount()  {--mVswCount;}
    //void incLineCount() {mLineCount++;}

private:
    Vdu* mVdu;
    Ppi* mPpi;
    
    tUBYTE mRegister[18];
    tUBYTE mSelected;

    uint mFlags;
    tUBYTE mHsw;
    tUBYTE mHswCount;
    tUBYTE mHswActive;
    tUBYTE mVsw;
    tUBYTE mVswCount;
    tUBYTE mLastHDisp;
    tUBYTE mSkew;
    tUBYTE mVtAdjust;
    tUBYTE mVtAdjustCount;
    tUBYTE mMaxRaster;
    tUBYTE mRasterCount;
    tUBYTE mLineCount;
    tUBYTE mCharCount;
    uint mRequestedAddr;
    uint mAddr;

};

#endif
