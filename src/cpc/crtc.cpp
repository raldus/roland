/***************************************************************************
 *   Copyright (C) 2005-2013 by Fred Klaus <development@fkweb.de>          *
 *                                                                         *
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
#include "crtc.h"

namespace cpcx
{

    Crtc::Crtc()
    {
        mSelected = 0;
        mPpi      = 0;
        init();
    }

    Crtc::~Crtc()
    {
    }

    void Crtc::init(Ppi* ppi)
    {
        if (ppi) mPpi = ppi;
        if (!mPpi) return;

        tUBYTE ini[2][14] =
        {
            {0x3f, 0x28, 0x2e, 0x8e, 0x1f, 0x06, 0x19, 0x1b, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00},
            {0x3f, 0x28, 0x2e, 0x8e, 0x26, 0x00, 0x19, 0x1e, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00}
        };

        for(tUBYTE i=0; i<14; i++) mRegister[i]=ini[(mPpi->jumpers() & 0x10)>>4][i]; // PAL

        mSelected      = 0;
        mHswVdu        = 4; // formerly known as Vdu::mHsw
        mHswCount      = 0;
        mVswCount      = 0;
        mVtAdjustCount = 0;
        mRasterCount   = 0;
        mLineCount     = 0;
        mCharCount     = 0;

        mFlags            = (HDT | VDT);
        mHsw = mHswActive = mRegister[3] & 0x0f;
        mVsw              = mRegister[3] >> 4;
        mVtAdjust         = mRegister[5] & 0x1f;
        mMaxRaster        = mRegister[9] << 3;
        mSkew             = (mRegister[8] >> 4) & 3;
        if (mSkew == 3) mSkew = 0xff;    // no output?

        int val1, val2;
        val1  = mRegister[12] & 0x3f;    // ???
        val2  = val1 & 0x0f;             // isolate screen size
        val1  = (val1 << 1) & 0x60;      // isolate CPC RAM bank
        val2 |= val1;                    // combine
        mAddr = mRequestedAddr = (mRegister[13] + (val2 << 8)) << 1;

        mLastHDisp  = 0x28;
        }

    void Crtc::write(tUBYTE value)
    {
        mRegister[mSelected]=value;

        switch (mSelected)
        {
            case 3:  // sync width
                mHsw = value & 0x0f;  // isolate horizontal sync width

                mHswVdu=mHsw - 2;     // GA delays HSYNC by 2 chars
                if (mHswVdu < 0)      // negative valueue?
                {
                    mHswVdu=0;        // no VDU HSYNC
                }
                else if (mHswVdu > 4) // HSYNC longer than 4 chars?
                {
                    mHswVdu=4;        // maxium of 4
                }

                mVsw = value >> 4;    // isolate vertical sync width
                if (!mVsw)
                {
                    mVsw = 16;        // 0 = width of 16
                }
                break;

            case 5:  // vertical total adjust
                mVtAdjust = value & 0x1f;
                break;

            case 8:  // interlace and skew
                mSkew = (value >> 4) & 3; // isolate display timing skew
                if (mSkew == 3)           // no output?
                {
                    mSkew = 0xff;
                }
                break;

            case 9:  // maximum raster count
                mMaxRaster = value << 3; // modify value for easier handling
                break;

            case 12: // start address high byte
            case 13: // start address low byte
                {
                    tDWORD value1 = mRegister[12] & 0x3f;
                    tDWORD value2 = value1 & 0x0f;        // isolate screen size
                    value1 = (value1 << 1) & 0x60;       // isolate CPC RAM bank
                    value2 |= value1;                    // combine
                    mRequestedAddr = (mRegister[13] + (value2 << 8)) << 1;
                }
                break;
        }
    }

} // cpc
