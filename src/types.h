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
#ifndef ROLAND_TYPES_H
#define ROLAND_TYPES_H

#include <cstdint>
#include <string>

//! Represents c++ std::string
namespace sdltk
{
    using String = std::string;
};
using tString = std::string;


//enum Bits {Bit0=1, Bit1=2, Bit2=4, Bit3=8, Bit4=16, Bit5=32, Bit6=64, Bit7=128};

// **************
// *** signed ***
// **************
#ifndef tBYTE
	typedef int8_t tBYTE;
#else
	#error "*** tBYTE is already defined and may not match requested size. ***"
#endif

#ifndef tWORD
	typedef int16_t tWORD;
#else
	#error "*** tWORD is already defined and may not match requested size. ***"
#endif

#ifndef tDWORD
	typedef int32_t tDWORD;
#else
	#error "*** tDWORD is already defined and may not match requested size. ***"
#endif

#ifndef tQWORD
	typedef int64_t tQWORD;
#else
	#error "*** tQWORD is already defined and may not match requested size. ***"
#endif

#ifndef tINT64
	typedef int64_t tINT64;
#else
	#error "*** tINT64 is already defined and may not match requested size. ***"
#endif


// ****************
// *** unsigned ***
// ****************
#ifndef tBYTE
	typedef uint8_t tUBYTE;
#else
	#error "*** tUBYTE is already defined and may not match requested size. ***"
#endif

#ifndef tWORD
	typedef uint16_t tUWORD;
#else
	#error "*** tUWORD is already defined and may not match requested size. ***"
#endif

#ifndef tDWORD
	typedef uint32_t tUDWORD;
#else
	#error "*** tUDWORD is already defined and may not match requested size. ***"
#endif

#ifndef tQWORD
	typedef uint64_t tUQWORD;
#else
	#error "*** tUQWORD is already defined and may not match requested size. ***"
#endif

#ifndef tINT64
	typedef uint64_t tUINT64;
#else
	#error "*** tUINT64 is already defined and may not match requested size. ***"
#endif


#ifndef tREGPAIR
	typedef union
	{
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	   struct { tUBYTE l, h, h2, h3; } b;
	   struct { tUWORD l, h; } w;
	#else
	   struct { tUBYTE h3, h2, h, l; } b;
	   struct { tUWORD h, l; } w;
	#endif
	   tDWORD d;
	}  tREGPAIR;
#else
	#error "*** tREGPAIR is already defined and may not match requested size. ***"
#endif

#ifndef uint
	typedef unsigned int uint;
#else
	#warning "*** uint is already defined and may not match requested size. ***"
#endif

#ifndef uchar
	typedef unsigned char uchar;
#else
	#warning "*** uchar is already defined and may not match requested size. ***"
#endif


///@todo Should this be here ??
static constexpr tUBYTE bit_values[8] =
{
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};



/*
class tWORD
{
public:
	tWORD() {}
	~tWORD() {}

	operator short int() {return mValue.Both;}
	tWORD & operator=(const short int & w)
		{mValue.Both=w; return *this;}
	tWORD & operator--()
		{mValue.Both--; return *this;}

	tBYTE low()  {return mValue.Low;}
	tBYTE high() {return mValue.High;}

	void setLow (tBYTE w) {mValue.Low  = w;}
	void setHigh(tBYTE w) {mValue.High = w;}

private:
	typedef union
	{
		struct
		{
			tBYTE Low;
			tBYTE High;
		};
		short int Both;
	} WORD_t;

	WORD_t mValue;
};

class tUWORD
{
public:
	tUWORD() {}
	tUWORD(unsigned short int v) {mValue.Both=v;}
	~tUWORD() {}

	operator unsigned short int() {return mValue.Both;}
	operator uint()       {return (uint) mValue.Both;}
	operator int()  {return (int) mValue.Both;}
	operator bool() {return (bool) mValue.Both;}

	//tUWORD operator+ (const tUWORD & w)
	//	{return tUWORD(mValue.Both + w.both());}

	tUWORD operator-(const int & w)
		{return tUWORD(mValue.Both - (unsigned short int) w);}

	tUWORD operator+(const int & w)
		{return tUWORD(mValue.Both + (unsigned short int) w);}

	tUWORD operator*(const int & w)
		{return tUWORD(mValue.Both * (unsigned short int) w);}

	tUWORD* operator&(tUWORD & w)
		{return w.ptr();}

	tUWORD & operator=(const unsigned short int & w)
		{mValue.Both=w; return *this;}

	tUWORD & operator-=(const unsigned short int & w)
		{mValue.Both-=w; return *this;}

	tUWORD & operator+=(const unsigned short int & w)
		{mValue.Both+=w; return *this;}

	bool operator<(const tUWORD & v) const
		{return (bool) (mValue.Both < v.both());}
	bool operator<=(const tUWORD & v) const
		{return (bool) (mValue.Both <= v.both());}
	bool operator>(const tUWORD & v) const
		{return (bool) (mValue.Both > v.both());}
	bool operator>=(const tUWORD & v) const
		{return (bool) (mValue.Both >= v.both());}
	bool operator==(const tUWORD & v) const
		{return (bool) (mValue.Both == v.both());}
	bool operator!=(const tUWORD & v) const
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
	tUWORD & operator--()
		{--mValue.Both; return *this;}

	tUWORD operator--(int)
		{tUWORD tmp(*this);--mValue.Both; return tmp;}

	tUWORD & operator++()
		{++mValue.Both; return *this;}

	tUWORD operator++(int)
		{tUWORD tmp(*this);++mValue.Both; return tmp;}


	tUBYTE low()  {return mValue.Low;}
	tUBYTE high() {return mValue.High;}

	const unsigned short int both() const {return mValue.Both;}

	void setLow (tUBYTE w) {mValue.Low  = w;}
	void setHigh(tUBYTE w) {mValue.High = w;}

	tUWORD* ptr() {return this;}

private:
	typedef union
	{
		struct
		{
			tUBYTE Low;
			tUBYTE High;
		};
		unsigned short int Both;
	} UWORD_t;

	UWORD_t mValue;
};
*/

#endif // ROLAND_TYPES_H

