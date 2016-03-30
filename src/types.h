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
#ifndef CPCMU_TYPES_H
#define CPCMU_TYPES_H

#include <cstdint>
//enum Bits {Bit0=1, Bit1=2, Bit2=4, Bit3=8, Bit4=16, Bit5=32, Bit6=64, Bit7=128};


#define CPC_SCR_WIDTH 1024         // max width
#define CPC_SCR_HEIGHT 312         // max height
#define CPC_VISIBLE_SCR_WIDTH  384 // visible width : 4+40+4 * 8
#define CPC_VISIBLE_SCR_HEIGHT 272 // visible height: 5+25+4 * 8  @todo original is 272


// **************
// *** signed ***
// **************
#ifndef BYTE
    typedef int8_t BYTE;
#else
    #warning "*** BYTE is already defined and may not match requested size. ***"
#endif

#ifndef WORD
    typedef int16_t WORD;
#else
    #warning "*** WORD is already defined and may not match requested size. ***"
#endif

#ifndef DWORD
    typedef int32_t DWORD;
#else
    #warning "*** DWORD is already defined and may not match requested size. ***"
#endif

#ifndef QWORD
    typedef int64_t QWORD;
#else
    #warning "*** QWORD is already defined and may not match requested size. ***"
#endif

#ifndef INT64
    typedef int64_t INT64;
#else
    #warning "*** INT64 is already defined and may not match requested size. ***"
#endif


// ****************
// *** unsigned ***
// ****************
#ifndef BYTE
    typedef uint8_t UBYTE;
#else
    #warning "*** UBYTE is already defined and may not match requested size. ***"
#endif

#ifndef WORD
    typedef uint16_t UWORD;
#else
    #warning "*** UWORD is already defined and may not match requested size. ***"
#endif

#ifndef DWORD
    typedef uint32_t UDWORD;
#else
    #warning "*** UDWORD is already defined and may not match requested size. ***"
#endif

#ifndef QWORD
    typedef uint64_t UQWORD;
#else
    #warning "*** UQWORD is already defined and may not match requested size. ***"
#endif

#ifndef INT64
    typedef uint64_t UINT64;
#else
    #warning "*** UINT64 is already defined and may not match requested size. ***"
#endif


#ifndef REGPAIR
    typedef union
    {
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
       struct { UBYTE l, h, h2, h3; } b;
       struct { UWORD l, h; } w;
    #else
       struct { UBYTE h3, h2, h, l; } b;
       struct { UWORD h, l; } w;
    #endif
       DWORD d;
    }  REGPAIR;
#else
    #warning "*** REGPAIR is already defined and may not match requested size. ***"
#endif

#ifndef uint
    typedef unsigned int uint;
#else
    #warning "*** WORD is already defined and may not match requested size. ***"
#endif


//@todo Should this be here ??
static const UBYTE bit_values[8] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};



/*
class WORD
{
public:
    WORD() {}
    ~WORD() {}

    operator short int() {return mValue.Both;}
    WORD & operator=(const short int & w)
        {mValue.Both=w; return *this;}
    WORD & operator--()
        {mValue.Both--; return *this;}

    BYTE low()  {return mValue.Low;}
    BYTE high() {return mValue.High;}

    void setLow (BYTE w) {mValue.Low  = w;}
    void setHigh(BYTE w) {mValue.High = w;}

private:
    typedef union
    {
        struct
        {
            BYTE Low;
            BYTE High;
        };
        short int Both;
    } WORD_t;

    WORD_t mValue;
};

class UWORD
{
public:
    UWORD() {}
    UWORD(unsigned short int v) {mValue.Both=v;}
    ~UWORD() {}

    operator unsigned short int() {return mValue.Both;}
    operator uint()       {return (uint) mValue.Both;}
    operator int()  {return (int) mValue.Both;}
    operator bool() {return (bool) mValue.Both;}

    //UWORD operator+ (const UWORD & w)
    //  {return UWORD(mValue.Both + w.both());}

    UWORD operator-(const int & w)
        {return UWORD(mValue.Both - (unsigned short int) w);}

    UWORD operator+(const int & w)
        {return UWORD(mValue.Both + (unsigned short int) w);}

    UWORD operator*(const int & w)
        {return UWORD(mValue.Both * (unsigned short int) w);}

    UWORD* operator&(UWORD & w)
        {return w.ptr();}

    UWORD & operator=(const unsigned short int & w)
        {mValue.Both=w; return *this;}

    UWORD & operator-=(const unsigned short int & w)
        {mValue.Both-=w; return *this;}

    UWORD & operator+=(const unsigned short int & w)
        {mValue.Both+=w; return *this;}

    bool operator<(const UWORD & v) const
        {return (bool) (mValue.Both < v.both());}
    bool operator<=(const UWORD & v) const
        {return (bool) (mValue.Both <= v.both());}
    bool operator>(const UWORD & v) const
        {return (bool) (mValue.Both > v.both());}
    bool operator>=(const UWORD & v) const
        {return (bool) (mValue.Both >= v.both());}
    bool operator==(const UWORD & v) const
        {return (bool) (mValue.Both == v.both());}
    bool operator!=(const UWORD & v) const
        {return (bool) (mValue.Both != v.both());}

    */

    /*
    bool operator<(const unsigned short int & v) const
        {return (bool) (mValue.Both < v);}
    bool operator<=(const unsigned short int & v) const
        {return (bool) (mValue.Both <= v);}
    bool operator>(const unsigned short int & v) const
        {return (bool) (mValue.Both > v);}
    bool operator>=(const unsigned short int & v) const
        {return (bool) (mValue.Both >= v);}
    bool operator==(const unsigned short int & v) const
        {return (bool) (mValue.Both == v);}
    bool operator!=(const unsigned short int & v) const
        {return (bool) (mValue.Both != v);}
    */

/*
    UWORD & operator--()
        {--mValue.Both; return *this;}

    UWORD operator--(int)
        {UWORD tmp(*this);--mValue.Both; return tmp;}

    UWORD & operator++()
        {++mValue.Both; return *this;}

    UWORD operator++(int)
        {UWORD tmp(*this);++mValue.Both; return tmp;}


    UBYTE low()  {return mValue.Low;}
    UBYTE high() {return mValue.High;}

    const unsigned short int both() const {return mValue.Both;}

    void setLow (UBYTE w) {mValue.Low  = w;}
    void setHigh(UBYTE w) {mValue.High = w;}

    UWORD* ptr() {return this;}

private:
    typedef union
    {
        struct
        {
            UBYTE Low;
            UBYTE High;
        };
        unsigned short int Both;
    } UWORD_t;

    UWORD_t mValue;
};
*/

#endif // CPCMU_TYPES_H

