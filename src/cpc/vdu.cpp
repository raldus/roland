/***************************************************************************
 *   Copyright (C) 2005-2013 by Fred Klaus <development@fkweb.de>          *
 *                                                                         *
 *                                                                         *
 *   This program is free software=0; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation=0; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY=0; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program=0; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "vdu.h"

#include "crtc.h"
#include "gatearray.h"
#include "z80.h"

Vdu::Vdu(Crtc* crtc, GateArray* gatearray, Z80* z80)
{
    mBpp       = Bpp32;
    mBorder    = true;
    mLineDoubling= false;
    mScale     = 2;
    init(true, crtc, gatearray, z80);
}

void Vdu::init(bool full, Crtc* crtc, GateArray* gatearray, Z80* z80)
{
    if (crtc)      mCrtc      = crtc;
    if (gatearray) mGateArray = gatearray;
    if (z80)       mZ80       = z80;


    //mHsw            = 4
    mHswActive      = 4;
    mHswActive      = 0;
    mHswCount       = 0;
    mVswCount       = 0;
    mHDelay         = 0;
    mVDelay         = 0;
    mScanline       = 0;
    mScanlineMin    = 272; // @todo 240 ???
    mFrameCompleted = false;
    mCharCount      = 0;
    mHCount         = 0;
    mHStart         = 0;
    mHWidth         = 0;
    mVCount         = 0;
    mVStart         = 0;
    mVHeight        = 0;

    //mScrWidth       = 800;
    //mScrHeight      = 600;
    //mScrBpp         = 16;

    if (mScale == 1)
    {
        mModeHandler[0][0]=&Vdu::draw16bpp_mode0;
        mModeHandler[0][1]=&Vdu::draw16bpp_mode1;
        mModeHandler[0][2]=&Vdu::draw16bpp_mode2;
        mModeHandler[0][3]=&Vdu::draw16bpp_mode0;

        mModeHandler[1][0]=&Vdu::draw24bpp_mode0;
        mModeHandler[1][1]=&Vdu::draw24bpp_mode1;
        mModeHandler[1][2]=&Vdu::draw24bpp_mode2;
        mModeHandler[1][3]=&Vdu::draw24bpp_mode0;

        mModeHandler[2][0]=&Vdu::draw32bpp_mode0;
        mModeHandler[2][1]=&Vdu::draw32bpp_mode1;
        mModeHandler[2][2]=&Vdu::draw32bpp_mode2;
        mModeHandler[2][3]=&Vdu::draw32bpp_mode0;

        if (mBorder)
        {
            mBorderHandler[0]=&Vdu::draw16bpp_border;
            mBorderHandler[1]=&Vdu::draw24bpp_border;
            mBorderHandler[2]=&Vdu::draw32bpp_border;
        }
        else
        {
            mBorderHandler[0]=&Vdu::draw16bpp_nullborder;
            mBorderHandler[1]=&Vdu::draw24bpp_nullborder;
            mBorderHandler[2]=&Vdu::draw32bpp_nullborder;
        }
    }

    if (mScale == 2)
    {
        mModeHandler[0][0]=&Vdu::draw16bppx2_mode0;
        mModeHandler[0][1]=&Vdu::draw16bppx2_mode1;
        mModeHandler[0][2]=&Vdu::draw16bppx2_mode2;
        mModeHandler[0][3]=&Vdu::draw16bppx2_mode0;

        mModeHandler[1][0]=&Vdu::draw24bppx2_mode0;
        mModeHandler[1][1]=&Vdu::draw24bppx2_mode1;
        mModeHandler[1][2]=&Vdu::draw24bppx2_mode2;
        mModeHandler[1][3]=&Vdu::draw24bppx2_mode0;

        mModeHandler[2][0]=&Vdu::draw32bppx2_mode0;
        mModeHandler[2][1]=&Vdu::draw32bppx2_mode1;
        mModeHandler[2][2]=&Vdu::draw32bppx2_mode2;
        mModeHandler[2][3]=&Vdu::draw32bppx2_mode0;

        if (mBorder)
        {
            mBorderHandler[0]=&Vdu::draw16bppx2_border;
            mBorderHandler[1]=&Vdu::draw24bppx2_border;
            mBorderHandler[2]=&Vdu::draw32bppx2_border;
        }
        else
        {
            mBorderHandler[0]=&Vdu::draw16bppx2_nullborder;
            mBorderHandler[1]=&Vdu::draw24bppx2_nullborder;
            mBorderHandler[2]=&Vdu::draw32bppx2_nullborder;
        }
    }

    mCurModeHandler  = mModeHandler  [mBpp][1];
    mCurBorderHandler= mBorderHandler[mBpp];

    if (full)
    {
        mHStart  = 7;
        mHWidth  = CPC_VISIBLE_SCR_WIDTH / 8;
        mVStart  = 27;
        mVHeight = CPC_VISIBLE_SCR_HEIGHT;

        mHswActive   = 4;
        mScanlineMin = 272; //240;// @todo 272 ???
    }


    tUDWORD idx, n, p1, p2, p3, p4;

    idx = 0;
    for (n = 0; n < 256; n++) // calculate mode0 tUBYTE-to-pixel translation table
    {
        p1 = ((n & 0x80) >> 7) + ((n & 0x08) >> 2) + ((n & 0x20) >> 3) + ((n & 0x02) << 2);
        p2 = ((n & 0x40) >> 6) + ((n & 0x04) >> 1) + ((n & 0x10) >> 2) + ((n & 0x01) << 3);
        mode0_table[idx++] = p1;
        mode0_table[idx++] = p2;
    }

    idx = 0;
    for (n = 0; n < 256; n++) // calculate mode1 tUBYTE-to-pixel translation table
    {
        p1 = ((n & 0x80) >> 7) + ((n & 0x08) >> 2);
        p2 = ((n & 0x40) >> 6) + ((n & 0x04) >> 1);
        p3 = ((n & 0x20) >> 5) +  (n & 0x02);
        p4 = ((n & 0x10) >> 4) + ((n & 0x01) << 1);
        mode1_table[idx++] = p1;
        mode1_table[idx++] = p2;
        mode1_table[idx++] = p3;
        mode1_table[idx++] = p4;
    }
}

void Vdu::setBpp(Bpp bpp)
{
    mBpp = bpp;
    init(true);
}

void Vdu::setScale(uchar scale)
{
    mScale=scale;
    init(true);
}

void Vdu::setLineDoubling(bool doubling)
{
    mLineDoubling = doubling;
}

void Vdu::setBorder(bool border)
{
    mBorder=border;

    if (mBorder)
    {
        mBorderHandler[0]=&Vdu::draw16bppx2_border;
        mBorderHandler[1]=&Vdu::draw24bppx2_border;
        mBorderHandler[2]=&Vdu::draw32bppx2_border;
    }
    else
    {
        mBorderHandler[0]=&Vdu::draw16bppx2_nullborder;
        mBorderHandler[1]=&Vdu::draw24bppx2_nullborder;
        mBorderHandler[2]=&Vdu::draw32bppx2_nullborder;
    }
}

void Vdu::access_video_memory(int repeat_count)
{
    register tUBYTE char_count, line_count, raster_count;

    do
    {
        char_count   = mCrtc->charCount();
        line_count   = mCrtc->lineCount();
        raster_count = mCrtc->rasterCount();

        char_count++; // update character count (cc)
        mCharCount++;

        if (mCrtc->flags() & Crtc::HT) // reached horizontal total on last cc?
        {
            mCrtc->removeFlags(Crtc::HT);
            mCrtc->setHswActive(mCrtc->hsw());
            mHswActive = mCrtc->hswVdu();
            char_count = 0;            // reset cc
            // next raster ----------------------------------------------------------------
            raster_count += 8;         // advance rc by one
            if (mCrtc->flags() & Crtc::VS)   // in VSYNC?
            {
                mCrtc->incVswCount();  // update width counter
                if (mCrtc->vswCount() >= mCrtc->vsw()) // reached end of VSYNC?
                {
                    //mCrtc->flags() = (mCrtc->flags() & ~VS_flag) | VSf_flag; // reset VSYNC, set 'just finished'
                    mCrtc->removeFlags(Crtc::VS);
                    mCrtc->addFlags(Crtc::VSf);
                }
            }
            if (mCrtc->flags() & Crtc::MR)    // reached maximum raster address on last rc?
            {
                mCrtc->removeFlags(Crtc::MR);
                raster_count = 0;             // reset rc
                if (!(mCrtc->flags() & Crtc::HDT))         // HDISPTMG still on (i.e. R01 > R00)?
                {
                    mCrtc->setAddr(mCrtc->addr() + mCrtc->lastHDisp() * 2); // advance CPC screen address to next line
                }
                line_count++;           // update line count
                line_count &= 127;      // limit to 7 bits
            }
            if (mCrtc->vtAdjustCount()) // vertical total adjust active?
            {
                mCrtc->setVtAdjustCount(mCrtc->vtAdjustCount()-1);
                if (mCrtc->vtAdjustCount() == 0) // done with vta?
                {
                    //mCrtc->flags() = (mCrtc->flags() & ~VSf_flag) | VDT_flag; // enable VDISPTMG
                    mCrtc->removeFlags(Crtc::VSf); // enable VDISPTMG
                    mCrtc->addFlags(Crtc::VDT);
                    raster_count = 0;   // reset rc
                    line_count   = 0;   // reset lc
                    mCrtc->setAddr(mCrtc->requestedAddr()); // update start of CPC screen address
                }
            }
            if (mCrtc->flags() & Crtc::VT)    // reached vertical total on last lc?
            {
                mCrtc->removeFlags(Crtc::VT);
                if (mCrtc->vtAdjust())  // do a vertical total adjust?
                {
                    mCrtc->setVtAdjustCount(mCrtc->vtAdjust()); // init vta counter
                }
                else
                {
                    //mCrtc->flags() = (mCrtc->flags() & ~VSf_flag) | VDT_flag; // enable VDISPTMG
                    mCrtc->removeFlags(Crtc::VSf); // enable VDISPTMG
                    mCrtc->addFlags(Crtc::VDT);
                    raster_count = 0;        // reset rc
                    line_count   = 0;        // reset lc
                    mCrtc->setAddr(mCrtc->requestedAddr()); // update start of CPC screen address
                }
            }
            if (raster_count == mCrtc->maxRaster()) // rc = maximum raster address?
            {
                mCrtc->addFlags(Crtc::MR);          // process max raster address on next rc
                if (!mCrtc->vtAdjustCount())        // no vertical total adjust?
                {
                    if (line_count == mCrtc->read(Crtc::VerticalTotal)) // lc = vertical total?
                    {
                        mCrtc->addFlags(Crtc::VT);  // takes effect on next lc
                    }
                }
            }
            if (line_count == mCrtc->read(Crtc::VerticalDisplayed))   // lc = vertical displayed?
            {
                mCrtc->removeFlags(Crtc::VDT);      // disable VDISPTMG
            }
            if (line_count == mCrtc->read(Crtc::VerticalSyncPosition)) // lc = vertical sync position?
            {
                if (!(mCrtc->flags() & (Crtc::VSf | Crtc::VS))) // not in VSYNC?
                {
                    mCrtc->addFlags(Crtc::VS);
                    mCrtc->setVswCount(0);      // clear vsw counter
                    mVDelay   = 2;              // GA delays vsync by 2 scanlines
                    mVswCount = 4;              // GA vsync is always 4 scanlines long
                    mGateArray->setIntDelay(2); // arm GA two scanlines interrupt delay
                }
            }
            // ----------------------------------------------------------------------------
            mCrtc->addFlags(Crtc::HDT);    // enable horizontal display
        }
        if (char_count == mCrtc->read(Crtc::HorizontalTotal))     // cc = horizontal total?
        {
            mCrtc->addFlags(Crtc::HT);     // takes effect on next cc
        }
        if (char_count == mCrtc->read(Crtc::HorizontalDisplayed)) // cc = horizontal displayed?
        {
            mCrtc->removeFlags(Crtc::HDT); // disable horizontal display
            mCrtc->setLastHDisp(mCrtc->read(Crtc::HorizontalDisplayed)); // save width for line advancement
        }
        if (mCrtc->flags() & Crtc::HS)     // in horizontal sync?
        {
            // check hsw ------------------------------------------------------------------
            if (mHDelay == 2) // ready to trigger VDU HSYNC?
            {
                if (--mHswCount == 0)
                {
                    if (mScrLine < CPC_SCR_HEIGHT) ///@todo what the hell is CPC_SCR_HEIGHT ????
                    {
                        mScrLine++;
                        if (mVCount != 0) // (!= 0) in the visible portion of the screen?
                        {
                            if (mScale == 2) doubling();
                            mScrBase += mScrLineOffset; // advance to next line
                        }
                    }
                    mScrOffset = 0;
                    mCharCount = 0;

                    mHDelay++; // prevent reentry
                }
            }
            else
            {
                mHDelay++; // update delay counter
            }
            mCrtc->setHswCount(mCrtc->hswCount() - 1);
            if (mCrtc->hswCount() == 0) // end of HSYNC?
            {
                // hsw end --------------------------------------------------------------------
                mCrtc->removeFlags(Crtc::HS); // reset HSYNC
                mGateArray->setMode(mGateArray->requestedMode()); // execute mode change
                mCurModeHandler=mModeHandler[mBpp][mGateArray->requestedMode()];
                mScanline++;
                if (mVDelay) // monitor delaying VSYNC?
                {
                    mVDelay--;
                }
                if (mVDelay == 0) // enter monitor VSYNC?
                {
                    if (mVswCount) // still in monitor VSYNC?
                    {
                        if (--mVswCount == 0) // done with monitor VSYNC?
                        {
                            if (mScanline > mScanlineMin) // above minimum scanline count?
                            {
                                mScrOffset = 0;
                                mScrLine = 0;
                                mFrameCompleted = true;      // force exit of emulation loop
                                mZ80->stop();
                            }
                        }
                    }
                }
                // GA interrupt trigger -------------------------------------------------------
                mGateArray->setSlCount(mGateArray->slCount() + 1); // update GA scanline counter
                if (mGateArray->intDelay()) // delay on VSYNC?
                {
                    mGateArray->setIntDelay(mGateArray->intDelay() - 1);
                    if (mGateArray->intDelay() == 0)     // delay expired?
                    {
                        if (mGateArray->slCount() >= 32) // counter above save margin?
                        {
                            /** @todo mZ80->int_pending = 1;// queue interrupt */
                            mZ80->setIntPending(1);
                            // mGateArray->setInterrupt(false);
                            mGateArray->setSlCount(0);   // clear counter
                        }
                        else
                        {
                            mGateArray->setSlCount(0);   // clear counter
                        }
                    }
                }
                if (mGateArray->slCount() == 52) // trigger interrupt?
                {
                    /** @todo mZ80->int_pending = 1; // queue interrupt */
                    mZ80->setIntPending(1);
                    // mGateArray->setInterrupt(false);
                    mGateArray->setSlCount(0);   // clear counter
                }
            }
        }
        // ----------------------------------------------------------------------------
        if (char_count == mCrtc->read(Crtc::HorizontalSyncPosition)) // cc = horizontal sync position?
        {
            if (mCrtc->hswActive())          // allow HSYNCs?
            {
                mCrtc->addFlags(Crtc::HS);   // set HSYNC
                mCrtc->setHswCount(mCrtc->hswActive()); // load hsw counter
                mHDelay = 0;                 // clear VDU 2 chars HSYNC delay
                mHswCount = mHswActive;      // load VDU hsw counter
            }
        }
        mCrtc->setCharCount(char_count);     // store cc
        mCrtc->setLineCount(line_count);     // store lc
        mCrtc->setRasterCount(raster_count); // store rc

        {
            addr.d = char_count;
            addr.d = (addr.d * 2) + mCrtc->addr(); // cc x2 + CPC screen memory address
            if (addr.b.h & 0x20)  // 32K screen?
            {
                addr.b.h += 0x40; // advance to next 16K segment
            }
            addr.b.h &= 0xc7;     // apply 11000111 mask
            addr.b.h |= (raster_count & 0x38); // insert rc, masked with 00111000

            if (mHCount != 0)
            {
                if ((mCrtc->flags() & (Crtc::HDT | Crtc::VDT)) == (Crtc::HDT | Crtc::VDT)) // DISPTMG enabled?
                {
                    if (mCrtc->skew() != 0)
                    {
                        mCrtc->setSkew(mCrtc->skew() - 1);
                        if (mScrBase < mScrEnd) (this->*(mCurBorderHandler))(); // @bug should not happen, but does. Only in 32 Bpp
                        //(this->*(mCurBorderHandler))();
                    }
                    else
                    {
                        if (mScrBase < mScrEnd) (this->*(mCurModeHandler))(); // @bug should not happen, but does. Only in 32 Bpp
                        //(this->*(mCurModeHandler))();
                    }
                }
                else
                {
                    if (mScrBase < mScrEnd) (this->*(mCurBorderHandler))(); // @bug should not happen, but does. Only in 32 Bpp
                    //(this->*(mCurBorderHandler))();
                }
                mHCount--;
            }
            else
            {
                if (mCharCount == mHStart)
                {
                    if (mVCount)
                    {
                        mHCount = mHWidth;
                        mVCount--;
                    }
                    else
                    {
                        if (mScrLine == mVStart)
                        {
                            mVCount = mVHeight;
                        }
                    }
                }
            }
        }
    }
    while (--repeat_count);
}

inline void Vdu::doubling()
{
    if (mLineDoubling && (mScrBase < mScrEnd)) ///@todo Last line draws outside the screenmem
    {
        #ifdef USE_MMX
            mmx_memcpy((uchar*) ((uchar*)mScrBase + (mScrLineOffset*2)), (uchar*)mScrBase, mScrLineOffset*2);
        #else
            memcpy(    (uchar*) ((uchar*)mScrBase + (mScrLineOffset*2)), (uchar*)mScrBase, mScrLineOffset*2);
        #endif
    }
}

//******************************************
//*** 16 bpp *******************************
//******************************************
void Vdu::draw16bppx2_border()
{
    tUDWORD colour;
    register tUDWORD* mem_ptr;

    colour   = mGateArray->palette(16);
    colour |= (colour << 16);
    mem_ptr  = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer address
    *mem_ptr = colour; // write one pixel of border colour
    *(mem_ptr+1) = colour;
    *(mem_ptr+2) = colour;
    *(mem_ptr+3) = colour;
    *(mem_ptr+4) = colour;
    *(mem_ptr+5) = colour;
    *(mem_ptr+6) = colour;
    *(mem_ptr+7) = colour;

    mScrOffset+=8; // update PC screen buffer address
}

void Vdu::draw16bppx2_nullborder()
{
    mScrOffset+=8; // update PC screen buffer address
}

void Vdu::draw16bppx2_mode0()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    mem_ptr = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer address

    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *mem_ptr     = val;        // write one pixels
    *(mem_ptr+1) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+2) = val;
    *(mem_ptr+3) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1) & 0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(mem_ptr+4) = val;
    *(mem_ptr+5) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+6) = val;
    *(mem_ptr+7) = val;

    mScrOffset+=8; // update PC screen buffer addr.w.less
}

void Vdu::draw16bppx2_mode1()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    //std::cout << "CPC-addr.w.l: " << std::hex << (int) addr.w.l << "\n";

    mem_ptr = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer addr.w.less

    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *mem_ptr     = val;        // write one pixels
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(mem_ptr+1) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+2) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(mem_ptr+3) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(mem_ptr+4) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(mem_ptr+5) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+6) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(mem_ptr+7) = val;

    mScrOffset+=8; // update PC screen buffer addr.w.less
}

void Vdu::draw16bppx2_mode2()
{
    tUBYTE pat;
    register tUDWORD *mem_ptr;
    tUDWORD pen_on, pen_off;
    tREGPAIR val;

    mem_ptr = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer addr.w.less
    pen_on  = mGateArray->palette(1);
    pen_off = mGateArray->palette(0);

    pat = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val.w.l = (pat & 0x80) ? pen_on : pen_off;
    val.w.h = (pat & 0x40) ? pen_on : pen_off;
    *mem_ptr = val.d;      // write four pixels
    val.w.l = (pat & 0x20) ? pen_on : pen_off;
    val.w.h = (pat & 0x10) ? pen_on : pen_off;
    *(mem_ptr+1) = val.d;
    val.w.l = (pat & 0x08) ? pen_on : pen_off;
    val.w.h = (pat & 0x04) ? pen_on : pen_off;
    *(mem_ptr+2) = val.d;
    val.w.l = (pat & 0x02) ? pen_on : pen_off;
    val.w.h = (pat & 0x01) ? pen_on : pen_off;
    *(mem_ptr+3) = val.d;

    pat = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val.w.l = (pat & 0x80) ? pen_on : pen_off;
    val.w.h = (pat & 0x40) ? pen_on : pen_off;
    *(mem_ptr+4) = val.d;
    val.w.l = (pat & 0x20) ? pen_on : pen_off;
    val.w.h = (pat & 0x10) ? pen_on : pen_off;
    *(mem_ptr+5) = val.d;
    val.w.l = (pat & 0x08) ? pen_on : pen_off;
    val.w.h = (pat & 0x04) ? pen_on : pen_off;
    *(mem_ptr+6) = val.d;
    val.w.l = (pat & 0x02) ? pen_on : pen_off;
    val.w.h = (pat & 0x01) ? pen_on : pen_off;
    *(mem_ptr+7) = val.d;

    mScrOffset+=8; // update PC screen buffer addr.w.less
}



//******************************************
//*** 24 bpp *******************************
//******************************************
void Vdu::draw24bppx2_border()
{
    tUDWORD colour;
    register tUBYTE *mem_ptr;

    colour = mGateArray->palette(16);
    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer address
    *(tUDWORD *) mem_ptr = colour; // write one pixel of border colour
    *(tUDWORD *)(mem_ptr+3)  = colour;
    *(tUDWORD *)(mem_ptr+6)  = colour;
    *(tUDWORD *)(mem_ptr+9)  = colour;
    *(tUDWORD *)(mem_ptr+12) = colour;
    *(tUDWORD *)(mem_ptr+15) = colour;
    *(tUDWORD *)(mem_ptr+18) = colour;
    *(tUDWORD *)(mem_ptr+21) = colour;
    *(tUDWORD *)(mem_ptr+24) = colour;
    *(tUDWORD *)(mem_ptr+27) = colour;
    *(tUDWORD *)(mem_ptr+30) = colour;
    *(tUDWORD *)(mem_ptr+33) = colour;
    *(tUDWORD *)(mem_ptr+36) = colour;
    *(tUDWORD *)(mem_ptr+39) = colour;
    *(tUDWORD *)(mem_ptr+42) = colour;
    *(tUDWORD *)(mem_ptr+45) = colour;

    mScrOffset+=12; // update PC screen buffer address
}

void Vdu::draw24bppx2_nullborder()
{
    mScrOffset+=12; // update PC screen buffer address
}

void Vdu::draw24bppx2_mode0()
{
    tUBYTE idx;
    register tUBYTE *mem_ptr;
    tUDWORD val;

    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(tUDWORD *)mem_ptr = val; // write one pixels
    *(tUDWORD *)(mem_ptr+3)  = val;
    *(tUDWORD *)(mem_ptr+6)  = val;
    *(tUDWORD *)(mem_ptr+9)  = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(tUDWORD *)(mem_ptr+12) = val;
    *(tUDWORD *)(mem_ptr+15) = val;
    *(tUDWORD *)(mem_ptr+18) = val;
    *(tUDWORD *)(mem_ptr+21) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(tUDWORD *)(mem_ptr+24) = val;
    *(tUDWORD *)(mem_ptr+27) = val;
    *(tUDWORD *)(mem_ptr+30) = val;
    *(tUDWORD *)(mem_ptr+33) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(tUDWORD *)(mem_ptr+36) = val;
    *(tUDWORD *)(mem_ptr+39) = val;
    *(tUDWORD *)(mem_ptr+42) = val;
    *(tUDWORD *)(mem_ptr+45) = val;

    mScrOffset+=12; // update PC screen buffer address
}

void Vdu::draw24bppx2_mode1()
{
    tUBYTE idx;
    register tUBYTE *mem_ptr;
    tUDWORD val;

    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(tUDWORD *)mem_ptr = val; // write one pixels
    *(tUDWORD *)(mem_ptr+3)  = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(tUDWORD *)(mem_ptr+6)  = val;
    *(tUDWORD *)(mem_ptr+9)  = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(tUDWORD *)(mem_ptr+12) = val;
    *(tUDWORD *)(mem_ptr+15) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(tUDWORD *)(mem_ptr+18) = val;
    *(tUDWORD *)(mem_ptr+21) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(tUDWORD *)(mem_ptr+24) = val;
    *(tUDWORD *)(mem_ptr+27) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(tUDWORD *)(mem_ptr+30) = val;
    *(tUDWORD *)(mem_ptr+33) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(tUDWORD *)(mem_ptr+36) = val;
    *(tUDWORD *)(mem_ptr+39) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(tUDWORD *)(mem_ptr+42) = val;
    *(tUDWORD *)(mem_ptr+45) = val;

    mScrOffset+=12; // update PC screen buffer address
}

void Vdu::draw24bppx2_mode2()
{
    tUBYTE pat;
    register tUBYTE *mem_ptr;
    tUDWORD pen_on, pen_off;

    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer addr.w.less
    pen_on  = mGateArray->palette(1);
    pen_off = mGateArray->palette(0);

    pat = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    *(tUDWORD *) mem_ptr     = (pat & 0x80) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+3)  = (pat & 0x40) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+6)  = (pat & 0x20) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+9)  = (pat & 0x10) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+12) = (pat & 0x08) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+15) = (pat & 0x04) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+18) = (pat & 0x02) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+21) = (pat & 0x01) ? pen_on : pen_off;

    pat = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    *(tUDWORD *)(mem_ptr+24) = (pat & 0x80) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+27) = (pat & 0x40) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+30) = (pat & 0x20) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+33) = (pat & 0x10) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+36) = (pat & 0x08) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+39) = (pat & 0x04) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+42) = (pat & 0x02) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+45) = (pat & 0x01) ? pen_on : pen_off;

    mScrOffset+=12; // update PC screen buffer address
}




//******************************************
//*** 32 bpp x 2 ***************************
//******************************************
void Vdu::draw32bppx2_border()
{
    tUDWORD colour;
    register tUDWORD *mem_ptr;

    colour = mGateArray->palette(16);
    mem_ptr = (mScrBase + mScrOffset); // PC screen buffer addr.w.less
    *mem_ptr = colour; // write one pixel of border colour
    *(mem_ptr+1)  = colour;
    *(mem_ptr+2)  = colour;
    *(mem_ptr+3)  = colour;
    *(mem_ptr+4)  = colour;
    *(mem_ptr+5)  = colour;
    *(mem_ptr+6)  = colour;
    *(mem_ptr+7)  = colour;
    *(mem_ptr+8)  = colour;
    *(mem_ptr+9)  = colour;
    *(mem_ptr+10) = colour;
    *(mem_ptr+11) = colour;
    *(mem_ptr+12) = colour;
    *(mem_ptr+13) = colour;
    *(mem_ptr+14) = colour;
    *(mem_ptr+15) = colour;

    mScrOffset+=16; // update PC screen buffer address
}

void Vdu::draw32bppx2_nullborder()
{
    mScrOffset+=16; // update PC screen buffer address
}

void Vdu::draw32bppx2_mode0()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    mem_ptr = mScrBase + mScrOffset; // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *mem_ptr = val; // write one pixels
    *(mem_ptr+1) = val;
    *(mem_ptr+2) = val;
    *(mem_ptr+3) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+4) = val;
    *(mem_ptr+5) = val;
    *(mem_ptr+6) = val;
    *(mem_ptr+7) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(mem_ptr+8)  = val;
    *(mem_ptr+9)  = val;
    *(mem_ptr+10) = val;
    *(mem_ptr+11) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+12) = val;
    *(mem_ptr+13) = val;
    *(mem_ptr+14) = val;
    *(mem_ptr+15) = val;

    mScrOffset+=16; // update PC screen buffer address
}

void Vdu::draw32bppx2_mode1()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    mem_ptr = mScrBase + mScrOffset; // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *mem_ptr = val; // write one pixels
    *(mem_ptr+1) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(mem_ptr+2) = val;
    *(mem_ptr+3) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+4) = val;
    *(mem_ptr+5) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(mem_ptr+6) = val;
    *(mem_ptr+7) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(mem_ptr+8) = val;
    *(mem_ptr+9) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(mem_ptr+10) = val;
    *(mem_ptr+11) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+12) = val;
    *(mem_ptr+13) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(mem_ptr+14) = val;
    *(mem_ptr+15) = val;

    mScrOffset+=16; // update PC screen buffer address
}

void Vdu::draw32bppx2_mode2()
{
    tUBYTE pat;
    register tUDWORD *mem_ptr;
    tUDWORD pen_on, pen_off;

    mem_ptr = mScrBase + mScrOffset; // PC screen buffer addr.w.less
    pen_on  = mGateArray->palette(1);
    pen_off = mGateArray->palette(0);

    pat = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    *mem_ptr = (pat & 0x80) ? pen_on : pen_off;
    *(mem_ptr+1) = (pat & 0x40) ? pen_on : pen_off;
    *(mem_ptr+2) = (pat & 0x20) ? pen_on : pen_off;
    *(mem_ptr+3) = (pat & 0x10) ? pen_on : pen_off;
    *(mem_ptr+4) = (pat & 0x08) ? pen_on : pen_off;
    *(mem_ptr+5) = (pat & 0x04) ? pen_on : pen_off;
    *(mem_ptr+6) = (pat & 0x02) ? pen_on : pen_off;
    *(mem_ptr+7) = (pat & 0x01) ? pen_on : pen_off;

    pat = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    *(mem_ptr+8)  = (pat & 0x80) ? pen_on : pen_off;
    *(mem_ptr+9)  = (pat & 0x40) ? pen_on : pen_off;
    *(mem_ptr+10) = (pat & 0x20) ? pen_on : pen_off;
    *(mem_ptr+11) = (pat & 0x10) ? pen_on : pen_off;
    *(mem_ptr+12) = (pat & 0x08) ? pen_on : pen_off;
    *(mem_ptr+13) = (pat & 0x04) ? pen_on : pen_off;
    *(mem_ptr+14) = (pat & 0x02) ? pen_on : pen_off;
    *(mem_ptr+15) = (pat & 0x01) ? pen_on : pen_off;

    mScrOffset+=16; // update PC screen buffer address
}

//******************************************
//*** 32 bpp x 1 ***************************
//******************************************
void Vdu::draw32bpp_border()
{
    tUDWORD colour;
    register tUDWORD *mem_ptr;

    colour = mGateArray->palette(16);
    mem_ptr = (mScrBase + mScrOffset); // PC screen buffer addr.w.less
    *mem_ptr      = colour; // write one pixel of border colour
    *(mem_ptr+1)  = colour;
    *(mem_ptr+2)  = colour;
    *(mem_ptr+3)  = colour;
    *(mem_ptr+4)  = colour;
    *(mem_ptr+5)  = colour;
    *(mem_ptr+6)  = colour;
    *(mem_ptr+7)  = colour;

    mScrOffset+=8; // update PC screen buffer address
}

void Vdu::draw32bpp_nullborder()
{
    mScrOffset+=8; // update PC screen buffer address
}

void Vdu::draw32bpp_mode0()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    mem_ptr = mScrBase + mScrOffset; // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *mem_ptr     = val; // write one pixels
    *(mem_ptr+1) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+2) = val;
    *(mem_ptr+3) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(mem_ptr+4) = val;
    *(mem_ptr+5) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+6) = val;
    *(mem_ptr+7) = val;

    mScrOffset+=8; // update PC screen buffer address
}

void Vdu::draw32bpp_mode1()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    mem_ptr = mScrBase + mScrOffset; // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *mem_ptr = val; // write one pixels
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(mem_ptr+1) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+2) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(mem_ptr+3) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(mem_ptr+4) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(mem_ptr+5) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+6) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(mem_ptr+7) = val;

    mScrOffset+=8; // update PC screen buffer address
}

void Vdu::draw32bpp_mode2()
{
    tUBYTE pat;
    register tUDWORD *mem_ptr;
    tUDWORD pen_on, pen_off;

    mem_ptr = mScrBase + mScrOffset; // PC screen buffer addr.w.less
    pen_on  = mGateArray->palette(1);
    pen_off = mGateArray->palette(0);

    pat = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    *mem_ptr     = (pat & 0x80) ? pen_on : pen_off;
    *(mem_ptr+1) = (pat & 0x20) ? pen_on : pen_off;
    *(mem_ptr+2) = (pat & 0x08) ? pen_on : pen_off;
    *(mem_ptr+3) = (pat & 0x02) ? pen_on : pen_off;

    pat = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    *(mem_ptr+4) = (pat & 0x80) ? pen_on : pen_off;
    *(mem_ptr+5) = (pat & 0x20) ? pen_on : pen_off;
    *(mem_ptr+6) = (pat & 0x08) ? pen_on : pen_off;
    *(mem_ptr+7) = (pat & 0x02) ? pen_on : pen_off;

    mScrOffset+=8; // update PC screen buffer address
}



//******************************************
//*** 24 bpp *******************************
//******************************************
void Vdu::draw24bpp_border()
{
    tUDWORD colour;
    register tUBYTE *mem_ptr;

    colour = mGateArray->palette(16);
    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer address
    *(tUDWORD *) mem_ptr     = colour; // write one pixel of border colour
    *(tUDWORD *)(mem_ptr+3)  = colour;
    *(tUDWORD *)(mem_ptr+6)  = colour;
    *(tUDWORD *)(mem_ptr+9)  = colour;
    *(tUDWORD *)(mem_ptr+12) = colour;
    *(tUDWORD *)(mem_ptr+15) = colour;
    *(tUDWORD *)(mem_ptr+18) = colour;
    *(tUDWORD *)(mem_ptr+21) = colour;

    mScrOffset+=6; // update PC screen buffer address
}

void Vdu::draw24bpp_nullborder()
{
    mScrOffset+=6; // update PC screen buffer address
}

void Vdu::draw24bpp_mode0()
{
    tUBYTE idx;
    register tUBYTE *mem_ptr;
    tUDWORD val;

    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(tUDWORD *) mem_ptr     = val; // write one pixels
    *(tUDWORD *)(mem_ptr+3)  = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(tUDWORD *)(mem_ptr+6) = val;
    *(tUDWORD *)(mem_ptr+9) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(tUDWORD *)(mem_ptr+12) = val;
    *(tUDWORD *)(mem_ptr+15) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(tUDWORD *)(mem_ptr+18) = val;
    *(tUDWORD *)(mem_ptr+21) = val;

    mScrOffset+=6; // update PC screen buffer address
}

void Vdu::draw24bpp_mode1()
{
    tUBYTE idx;
    register tUBYTE *mem_ptr;
    tUDWORD val;

    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer addr.w.less
    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(tUDWORD *) mem_ptr     = val; // write one pixels
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(tUDWORD *)(mem_ptr+3)  = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(tUDWORD *)(mem_ptr+6) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(tUDWORD *)(mem_ptr+9) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(tUDWORD *)(mem_ptr+12) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+1]);
    *(tUDWORD *)(mem_ptr+15) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(tUDWORD *)(mem_ptr+18) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+3]);
    *(tUDWORD *)(mem_ptr+21) = val;

    mScrOffset+=6; // update PC screen buffer address
}

void Vdu::draw24bpp_mode2()
{
    tUBYTE pat;
    register tUBYTE *mem_ptr;
    tUDWORD pen_on, pen_off;

    mem_ptr = (tUBYTE *)(mScrBase + mScrOffset); // PC screen buffer addr.w.less
    pen_on  = mGateArray->palette(1);
    pen_off = mGateArray->palette(0);

    pat = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    *(tUDWORD *) mem_ptr    = (pat & 0x80) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+3) = (pat & 0x20) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+6) = (pat & 0x08) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+9) = (pat & 0x02) ? pen_on : pen_off;

    pat = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    *(tUDWORD *)(mem_ptr+12) = (pat & 0x80) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+15) = (pat & 0x20) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+18) = (pat & 0x08) ? pen_on : pen_off;
    *(tUDWORD *)(mem_ptr+21) = (pat & 0x02) ? pen_on : pen_off;

    mScrOffset+=6; // update PC screen buffer address
}



//******************************************
//*** 16 bpp *******************************
//******************************************
void Vdu::draw16bpp_border()
{
    tUDWORD colour;
    register tUDWORD* mem_ptr;

    colour   = mGateArray->palette(16);
    colour |= (colour << 16);
    mem_ptr  = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer address
    *mem_ptr = colour; // write one pixel of border colour
    *(mem_ptr+1) = colour;
    *(mem_ptr+2) = colour;
    *(mem_ptr+3) = colour;

    mScrOffset+=4; // update PC screen buffer address
}

void Vdu::draw16bpp_nullborder()
{
    mScrOffset+=4; // update PC screen buffer address
}

void Vdu::draw16bpp_mode0()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    mem_ptr = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer address

    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *mem_ptr     = val;        // write one pixels
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+1) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1) & 0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode0_table[(idx*2)]);
    *(mem_ptr+2) = val;
    val = mGateArray->palette(mode0_table[(idx*2)+1]);
    *(mem_ptr+3) = val;

    mScrOffset+=4; // update PC screen buffer addr.w.less
}

void Vdu::draw16bpp_mode1()
{
    tUBYTE idx;
    register tUDWORD *mem_ptr;
    tUDWORD val;

    //std::cout << "CPC-addr.w.l: " << std::hex << (int) addr.w.l << "\n";

    mem_ptr = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer addr.w.less

    idx = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *mem_ptr     = val;        // write one pixels
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+1) = val;

    idx = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val = mGateArray->palette(mode1_table[(idx*4)]);
    *(mem_ptr+2) = val;
    val = mGateArray->palette(mode1_table[(idx*4)+2]);
    *(mem_ptr+3) = val;


    mScrOffset+=4; // update PC screen buffer addr.w.less
}

void Vdu::draw16bpp_mode2()
{
    tUBYTE pat;
    register tUDWORD *mem_ptr;
    tUDWORD pen_on, pen_off;
    tREGPAIR val;

    mem_ptr = (tUDWORD*) mScrBase + mScrOffset; // PC screen buffer addr.w.less
    pen_on  = mGateArray->palette(1);
    pen_off = mGateArray->palette(0);

    pat = *(mCpcRamBase + addr.w.l); // grab first CPC screen memory tUBYTE
    val.w.l = (pat & 0x80) ? pen_on : pen_off;
    val.w.h = (pat & 0x40) ? pen_on : pen_off;
    *mem_ptr = val.d;      // write four pixels
    val.w.l = (pat & 0x20) ? pen_on : pen_off;
    val.w.h = (pat & 0x10) ? pen_on : pen_off;
    *(mem_ptr+1) = val.d;
    val.w.l = (pat & 0x08) ? pen_on : pen_off;
    val.w.h = (pat & 0x04) ? pen_on : pen_off;
    *(mem_ptr+2) = val.d;
    val.w.l = (pat & 0x02) ? pen_on : pen_off;
    val.w.h = (pat & 0x01) ? pen_on : pen_off;
    *(mem_ptr+3) = val.d;

    pat = *(mCpcRamBase + ((addr.w.l+1)&0xffff)); // grab second CPC screen memory tUBYTE
    val.w.l = (pat & 0x80) ? pen_on : pen_off;
    val.w.h = (pat & 0x40) ? pen_on : pen_off;
    *(mem_ptr+4) = val.d;
    val.w.l = (pat & 0x20) ? pen_on : pen_off;
    val.w.h = (pat & 0x10) ? pen_on : pen_off;
    *(mem_ptr+5) = val.d;
    val.w.l = (pat & 0x08) ? pen_on : pen_off;
    val.w.h = (pat & 0x04) ? pen_on : pen_off;
    *(mem_ptr+6) = val.d;
    val.w.l = (pat & 0x02) ? pen_on : pen_off;
    val.w.h = (pat & 0x01) ? pen_on : pen_off;
    *(mem_ptr+7) = val.d;

    mScrOffset+=8; // update PC screen buffer addr.w.less
}



