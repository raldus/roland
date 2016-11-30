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
#ifndef CPC_REGISTER_H
#define CPC_REGISTER_H

#include "types.h"
#include "z80.h"

#include <cstring>

namespace cpcx
{

    /** @author Fred Klaus */
    class Register final
    {
        friend class Z80;

    public:
        Register() {init();}
        ~Register() = default;

        void init() {std::memset(this, 0, sizeof(*this)); IX.w.l=IY.w.l=0xffff;AF.b.l=0x40;break_point = 0xffffffff;}

    private:
        tREGPAIR AF, BC, DE, HL, PC, SP, AFx, BCx, DEx, HLx, IX, IY;
        tUBYTE   I, R, Rb7, IFF1, IFF2, IM, HALT, EI_issued, int_pending;
        tDWORD   break_point, trace;

    };

} // cpc

#endif // CPC_REGISTER_H
