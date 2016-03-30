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
#ifndef VDU_H
#define VDU_H


#ifdef USE_MMX
    #include "mmx_memcpy.h"
#else
    #include <memory>
#endif

#include "types.h"

class GateArray;
class Crtc;
class Z80;

/** @author Fred Klaus */
class Vdu
{

public:
    enum Bpp {Bpp16=0, Bpp24=1, Bpp32=2};

    Vdu(Crtc* crtc=0, GateArray* gatearray=0, Z80* z80=0);
    ~Vdu() {}

    /** Pointer on function to handle drawing one line for different CPC-Modes (0-2) and bits per Pixel (16,24,32)*/
    typedef void (Vdu::*ModeHandler)();
    /** Pointer on function to handle drawing the border for different bits per Pixel (16,24,32)*/
    typedef void (Vdu::*BorderHandler)();

    void init(bool full=false, Crtc* crtc=0, GateArray* gatearray=0, Z80* z80=0);

    void access_video_memory(int repeat_count);

    void draw16bpp_border();     ///< Border=true,  16 bpp
    void draw16bpp_nullborder(); ///< Border=false, 16 bpp @todo nullborder should clipped from the Screen
    void draw16bpp_mode0();      ///< Mode 0, 16 bpp
    void draw16bpp_mode1();      ///< Mode 1, 16 bpp
    void draw16bpp_mode2();      ///< Mode 2, 16 bpp

    void draw24bpp_border();     ///< Border=true,  24 bpp
    void draw24bpp_nullborder(); ///< Border=false, 24 bpp @todo nullborder should clipped from the Screen
    void draw24bpp_mode0();      ///< Mode 0, 24 bpp
    void draw24bpp_mode1();      ///< Mode 1, 24 bpp
    void draw24bpp_mode2();      ///< Mode 2, 24 bpp

    void draw32bpp_border();     ///< Border=true,  32 bpp
    void draw32bpp_nullborder(); ///< Border=false, 32 bpp @todo nullborder should clipped from the Screen
    void draw32bpp_mode0();      ///< Mode 0, 32 bpp
    void draw32bpp_mode1();      ///< Mode 1, 32 bpp
    void draw32bpp_mode2();      ///< Mode 2, 32 bpp
    

    uint hsw()            {return mHsw;}
    uint hswActive()      {return mHswActive;}
    uint hswCount()       {return mHswCount;}
    uint vswCount()       {return mVswCount;}
    uint hDelay()         {return mHDelay;}
    uint vDelay()         {return mVDelay;}
    uint scanline()       {return mScanline;}
    uint scanlineMin()    {return mScanlineMin;}
    bool frameCompleted() {return mFrameCompleted;}
    uint charCount()      {return mCharCount;}
    uint hCount()         {return mHCount;}
    uint hStart()         {return mHStart;}
    uint hWidth()         {return mHWidth;}
    uint vCount()         {return mVCount;}
    uint vStart()         {return mVStart;}
    uint vHeight()        {return mVHeight;}
    
    void setBpp        (Bpp bpp);
    void setBorder     (bool border     = true);
    void setDoublescan (bool doublescan = true);

    void setHsw            (uint value) {mHsw            = value;}
    void setHswActive      (uint value) {mHswActive      = value;}
    void setHswCount       (uint value) {mHswCount       = value;}
    void setVswCount       (uint value) {mVswCount       = value;}
    void setHDelay         (uint value) {mHDelay         = value;}
    void setVDelay         (uint value) {mVDelay         = value;}
    void setScanline       (uint value) {mScanline       = value;}
    void setScanlineMin    (uint value) {mScanlineMin    = value;}
    void setFrameCompleted (bool value) {mFrameCompleted = value;}
    void setCharCount      (uint value) {mCharCount      = value;}
    void setHCount         (uint value) {mHCount         = value;}
    void setHStart         (uint value) {mHStart         = value;}
    void setHWidth         (uint value) {mHWidth         = value;}
    void setVCount         (uint value) {mVCount         = value;}
    void setVStart         (uint value) {mVStart         = value;}
    void setVHeight        (uint value) {mVHeight        = value;}

    void setScrBase      (uint*  value) {mScrBase       = value;}
    void setScrEnd       (uint*  value) {mScrEnd        = value;}
    void setScrLineOffset(uint   value) {mScrLineOffset = value;}
    void setScrOffset    (uint   value) {mScrOffset     = value;}
    void setScrLine      (uint   value) {mScrLine       = value;}
    void setCpcRamBase   (UBYTE* value) {mCpcRamBase    = value;}
    
    inline void doublescan();

private:
    Crtc*      mCrtc;
    GateArray* mGateArray;
    Z80*       mZ80;

    Bpp mBpp;

    REGPAIR addr;

    bool mFrameCompleted;
    bool mBorder;
    bool mDoublescan;

    uint mHsw;
    uint mHswActive;
    uint mHswCount;
    uint mVswCount;
    uint mHDelay;
    uint mVDelay;
    uint mScanline;
    uint mScanlineMin;
    uint mCharCount;
    uint mHCount;
    uint mHStart;
    uint mHWidth;
    uint mVCount;
    uint mVStart;
    uint mVHeight;

    /*
    uint  mScrFsWidth;
    uint  mScrFsHeight;
    uint  mScrFs_bpp;
    uint  mScrStyle;
    uint  mScrVSync;
    uint  mScrLed;
    uint  mScrFps;
    uint  mScrTube;
    uint  mScrWindow;
    uint  mScrBps;
    */

    uint* mScrBase;
    uint* mScrEnd;
    uint  mScrWidth;
    uint  mScrHeight;
    uint  mScrBpp;
    uint  mScrLineOffset;
    uint  mScrOffset;
    uint  mScrLine;

    UBYTE* mCpcRamBase;

    UBYTE mode0_table[512];
    UBYTE mode1_table[1024];

    ModeHandler   mModeHandler[3][4];     // [bpp][mode]
    BorderHandler mBorderHandler[4];      // [mode]
    BorderHandler mNullBorderHandler[4];  // [mode]

    ModeHandler   mCurModeHandler;
    BorderHandler mCurBorderHandler;

};

#endif
