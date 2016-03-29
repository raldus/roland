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
#ifndef REGISTER_H
#define REGISTER_H

#include "types.h"

#include <memory>


#define _A        z80.AF.b.h
#define _F        z80.AF.b.l
#define _AF       z80.AF.w.l
#define _AFdword  z80.AF.d
#define _B        z80.BC.b.h
#define _C        z80.BC.b.l
#define _BC       z80.BC.w.l
#define _BCdword  z80.BC.d
#define _D        z80.DE.b.h
#define _E        z80.DE.b.l
#define _DE       z80.DE.w.l
#define _DEdword  z80.DE.d
#define _H        z80.HL.b.h
#define _L        z80.HL.b.l
#define _HL       z80.HL.w.l
#define _HLdword  z80.HL.d
#define _PC       z80.PC.w.l
#define _PCdword  z80.PC.d
#define _SP       z80.SP.w.l

#define _IXh      z80.IX.b.h
#define _IXl      z80.IX.b.l
#define _IX       z80.IX.w.l
#define _IXdword  z80.IX.d
#define _IYh      z80.IY.b.h
#define _IYl      z80.IY.b.l
#define _IY       z80.IY.w.l
#define _IYdword  z80.IY.d

#define _I        z80.I
#define _R        z80.R
#define _Rb7      z80.Rb7
#define _IFF1     z80.IFF1
#define _IFF2     z80.IFF2
#define _IM       z80.IM
#define _HALT     z80.HALT


class Z80;

/** @author Fred Klaus */
class Register
{
	friend class Z80;

public:
	Register() {init();}
	~Register(){}

	void init() {memset(this, 0, sizeof(*this)); IX.w.l=IY.w.l=0xffff;AF.b.l=0x40;break_point = 0xffffffff;}

private:
	REGPAIR AF, BC, DE, HL, PC, SP, AFx, BCx, DEx, HLx, IX, IY;
	UBYTE   I, R, Rb7, IFF1, IFF2, IM, HALT, EI_issued, int_pending;
	DWORD   break_point, trace;

};

#endif
