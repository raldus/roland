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
#ifndef Z80_MACROS_H
#define Z80_MACROS_H

//#define z80_wait_states iCycleCount=0;
//#define z80_int_handler iCycleCount=iCycleCount;


/*
#define z80_wait_states \
{ \
   vdu.access_video_memory(iCycleCount >> 2); \
   if (sound.sndEnabled()) { \
      psg.setCycleCountHigh(psg.cycleCountHigh() + iCycleCount); \
      if (psg.cycleCountHigh() >= sound.sndCycleCountInitHigh()) { \
         psg.setCycleCountBoth(psg.cycleCountBoth() - sound.sndCycleCountInitBoth()); \
         (sound.*(psg.synthesizer()))(); \
      } \
   } \
   if (fdc.phase() == EXEC_PHASE) { \
      fdc.setTimeout(fdc.timeout() - iCycleCount); \
      if (fdc.timeout() <= 0) { \
         fdc.addFlags(OVERRUN_flag); \
         if (fdc.cmdDirection() == FDC_TO_CPU) { \
            fdc.read_data(); \
         } \
         else { \
            fdc.write_data(0xff); \
         } \
      } \
   } \*/
   /*if ((cpc.tape_motor) && (cpc.tape_play_button)) { \
      iTapeCycleCount -= iCycleCount; \
      if (iTapeCycleCount <= 0) { \
         Tape_UpdateLevel(); \
      } \
   }*/ /*\
   cpc.setCycleCount(cpc.cycleCount() - iCycleCount); \
}


#define z80_int_handler \
{ \
   if (_IFF1) { // process interrupts? \
      _R++; \
      _IFF1 = _IFF2 = 0; // clear interrupt flip-flops \
      z80.int_pending = 0; \
      cpc.gatearray().setSlCount(cpc.gatearray().slCount() & 0xdf); // clear bit 5 of GA scanline counter  \
      if (_HALT) { // HALT instruction active?  \
         _HALT = 0; // exit HALT 'loop'  \
         _PC++; // correct PC  \
      } \
      if (_IM < 2) { // interrupt mode 0 or 1? (IM0 = IM1 on the CPC)  \
         iCycleCount = 20; \
         if (iWSAdjust) { \
            iCycleCount -= 4; \
         } \
         RST(0x0038); \
         z80_wait_states \
      } \
      else { // interrupt mode 2  \
         REGPAIR addr; \
         iCycleCount = 28; // was 76  \
         if (iWSAdjust) { \
            iCycleCount -= 4; \
         } \
         write_mem(--_SP, z80.PC.b.h); // store high byte of current PC  \
         write_mem(--_SP, z80.PC.b.l); // store low byte of current PC  \
         addr.b.l = 0xff; // assemble pointer  \
         addr.b.h = _I; \
         z80.PC.b.l = read_mem(addr.w.l); // retrieve low byte of vector  \
         z80.PC.b.h = read_mem(addr.w.l+1); // retrieve high byte of vector  \
         z80_wait_states \
      } \
   } \
}

*/

#define z80_int_handler \
{ \
	if (_IFF1) \
	{  \
		_R++; \
		_IFF1 = _IFF2 = 0;  \
		z80.int_pending = 0; \
		cpc.gatearray().setSlCount(cpc.gatearray().slCount() & 0xdf);   \
		if (_HALT) \
		{   \
			_HALT = 0;   \
			_PC++;   \
		} \
		if (_IM < 2)   \
		{   \
			iCycleCount = 20; \
			if (iWSAdjust)  \
			{ \
      			iCycleCount -= 4; \
			} \
			RST(0x0038); \
			z80_wait_states \
		} \
		else    \
		{   \
			REGPAIR addr; \
			iCycleCount = 28;  \
			if (iWSAdjust)    \
			{ \
				iCycleCount -= 4; \
			} \
			write_mem(--_SP, z80.PC.b.h);   \
			write_mem(--_SP, z80.PC.b.l);   \
			addr.b.l = 0xff;   \
			addr.b.h = _I; \
			z80.PC.b.l = read_mem(addr.w.l);   \
			z80.PC.b.h = read_mem(addr.w.l+1);   \
			z80_wait_states \
		} \
	} \
}


#define ADC(value) \
{ \
   unsigned val = value; \
   unsigned res = _A + val + (_F & Cflag); \
   _F = SZ[res & 0xff] | ((res >> 8) & Cflag) | ((_A ^ res ^ val) & Hflag) | \
      (((val ^ _A ^ 0x80) & (val ^ res) & 0x80) >> 5); \
   _A = res; \
}

#define ADD(value) \
{ \
   unsigned val = value; \
   unsigned res = _A + val; \
   _F = SZ[(UBYTE)res] | ((res >> 8) & Cflag) | ((_A ^ res ^ val) & Hflag) | \
      (((val ^ _A ^ 0x80) & (val ^ res) & 0x80) >> 5); \
   _A = (UBYTE)res; \
}

#define ADD16(dest, src) \
{ \
   DWORD res = z80.dest.d + z80.src.d; \
   _F = (_F & (Sflag | Zflag | Vflag)) | (((z80.dest.d ^ res ^ z80.src.d) >> 8) & Hflag) | \
      ((res >> 16) & Cflag) | ((res >> 8) & Xflags); \
   z80.dest.w.l = (UWORD)res; \
}

#define AND(val) \
{ \
   _A &= val; \
   _F = SZP[_A] | Hflag; \
}

#define CALL \
{ \
   REGPAIR dest; \
   dest.b.l = read_mem(_PC++); /* subroutine address low byte */ \
   dest.b.h = read_mem(_PC++); /* subroutine address high byte */ \
   write_mem(--_SP, z80.PC.b.h); /* store high byte of current PC */ \
   write_mem(--_SP, z80.PC.b.l); /* store low byte of current PC */ \
   _PC = dest.w.l; /* continue execution at subroutine */ \
}

#define CP(value) \
{ \
   unsigned val = value; \
   unsigned res = _A - val; \
   _F = (SZ[res & 0xff] & (Sflag | Zflag)) | (val & Xflags) | ((res >> 8) & Cflag) | Nflag | ((_A ^ res ^ val) & Hflag) | \
      ((((val ^ _A) & (_A ^ res)) >> 5) & Vflag); \
}

#define DAA \
{ \
   int idx = _A; \
   if(_F & Cflag) \
      idx |= 0x100; \
   if(_F & Hflag) \
      idx |= 0x200; \
   if(_F & Nflag) \
      idx |= 0x400; \
   _AF = DAATable[idx]; \
}

#define DEC(reg) \
{ \
   reg--; \
   _F = (_F & Cflag) | SZHV_dec[reg]; \
}

#define JR \
{ \
   signed char offset; \
   offset = (signed char)(read_mem(_PC)); /* grab signed jump offset */ \
   _PC += offset + 1; /* add offset & correct PC */ \
}

#define EXX \
{ \
   REGPAIR temp; \
   temp = z80.BCx; \
   z80.BCx = z80.BC; \
   z80.BC = temp; \
   temp = z80.DEx; \
   z80.DEx = z80.DE; \
   z80.DE = temp; \
   temp = z80.HLx; \
   z80.HLx = z80.HL; \
   z80.HL = temp; \
}

#define EX(op1, op2) \
{ \
   REGPAIR temp; \
   temp = op1; \
   op1 = op2; \
   op2 = temp; \
}

#define EX_SP(reg) \
{ \
   REGPAIR temp; \
   temp.b.l = read_mem(_SP++); \
   temp.b.h = read_mem(_SP); \
   write_mem(_SP--, z80.reg.b.h); \
   write_mem(_SP, z80.reg.b.l); \
   z80.reg.w.l = temp.w.l; \
}

#define INC(reg) \
{ \
   reg++; \
   _F = (_F & Cflag) | SZHV_inc[reg]; \
}

#define JP \
{ \
   REGPAIR addr; \
   addr.b.l = read_mem(_PC++); \
   addr.b.h = read_mem(_PC); \
   _PC = addr.w.l; \
}

#define LD16_MEM(reg) \
{ \
   REGPAIR addr; \
   addr.b.l = read_mem(_PC++); \
   addr.b.h = read_mem(_PC++); \
   z80.reg.b.l = read_mem(addr.w.l); \
   z80.reg.b.h = read_mem(addr.w.l+1); \
}

#define LDMEM_16(reg) \
{ \
   REGPAIR addr; \
   addr.b.l = read_mem(_PC++); \
   addr.b.h = read_mem(_PC++); \
   write_mem(addr.w.l, z80.reg.b.l); \
   write_mem(addr.w.l+1, z80.reg.b.h); \
}

#define OR(val) \
{ \
   _A |= val; \
   _F = SZP[_A]; \
}

#define POP(reg) \
{ \
   z80.reg.b.l = read_mem(_SP++); \
   z80.reg.b.h = read_mem(_SP++); \
}

#define PUSH(reg) \
{ \
   write_mem(--_SP, z80.reg.b.h); \
   write_mem(--_SP, z80.reg.b.l); \
}

#define RET \
{ \
   z80.PC.b.l = read_mem(_SP++); \
   z80.PC.b.h = read_mem(_SP++); \
}

#define RLA \
{ \
   UBYTE res = (_A << 1) | (_F & Cflag); \
   UBYTE carry = (_A & 0x80) ? Cflag : 0; \
   _F = (_F & (Sflag | Zflag | Pflag)) | carry | (res & Xflags); \
   _A = res; \
}

#define RLCA \
{ \
   _A = (_A << 1) | (_A >> 7); \
   _F = (_F & (Sflag | Zflag | Pflag)) | (_A & (Xflags | Cflag)); \
}

#define RRA \
{ \
   UBYTE res = (_A >> 1) | (_F << 7); \
   UBYTE carry = (_A & 0x01) ? Cflag : 0; \
   _F = (_F & (Sflag | Zflag | Pflag)) | carry | (res & Xflags); \
   _A = res; \
}

#define RRCA \
{ \
   _F = (_F & (Sflag | Zflag | Pflag)) | (_A & Cflag); \
   _A = (_A >> 1) | (_A << 7); \
   _F |= (_A & Xflags); \
}

#define RST(addr) \
{ \
   write_mem(--_SP, z80.PC.b.h); /* store high byte of current PC */ \
   write_mem(--_SP, z80.PC.b.l); /* store low byte of current PC */ \
   _PC = addr; /* continue execution at restart address */ \
}

#define SBC(value) \
{ \
   unsigned val = value; \
   unsigned res = _A - val - (_F & Cflag); \
   _F = SZ[res & 0xff] | ((res >> 8) & Cflag) | Nflag | ((_A ^ res ^ val) & Hflag) | \
      (((val ^ _A) & (_A ^ res) & 0x80) >> 5); \
   _A = res; \
}

#define SUB(value) \
{ \
   unsigned val = value; \
   unsigned res = _A - val; \
   _F = SZ[res & 0xff] | ((res >> 8) & Cflag) | Nflag | ((_A ^ res ^ val) & Hflag) | \
      (((val ^ _A) & (_A ^ res) & 0x80) >> 5); \
   _A = res; \
}

#define XOR(val) \
{ \
   _A ^= val; \
   _F = SZP[_A]; \
}

#define BIT(bit, reg) \
   _F = (_F & Cflag) | Hflag | SZ_BIT[reg & (1 << bit)]

#define BIT_XY BIT

// *************************************************************************************

#define ADC16(reg) \
{ \
   DWORD res = _HLdword + z80.reg.d + (_F & Cflag); \
   _F = (((_HLdword ^ res ^ z80.reg.d) >> 8) & Hflag) | \
      ((res >> 16) & Cflag) | \
      ((res >> 8) & (Sflag | Xflags)) | \
      ((res & 0xffff) ? 0 : Zflag) | \
      (((z80.reg.d ^ _HLdword ^ 0x8000) & (z80.reg.d ^ res) & 0x8000) >> 13); \
   _HL = (UWORD)res; \
}

#define CPD \
{ \
   UBYTE val = read_mem(_HL); \
   UBYTE res = _A - val; \
   _HL--; \
   _BC--; \
   _F = (_F & Cflag) | (SZ[res] & ~Xflags) | ((_A ^ val ^ res) & Hflag) | Nflag; \
   if(_F & Hflag) res -= 1; \
   if(res & 0x02) _F |= 0x20; \
   if(res & 0x08) _F |= 0x08; \
   if(_BC) _F |= Vflag; \
}

#define CPDR \
   CPD; \
   if(_BC && !(_F & Zflag)) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
      iWSAdjust++; \
   }

#define CPI \
{ \
   UBYTE val = read_mem(_HL); \
   UBYTE res = _A - val; \
   _HL++; \
   _BC--; \
   _F = (_F & Cflag) | (SZ[res] & ~Xflags) | ((_A ^ val ^ res) & Hflag) | Nflag; \
   if(_F & Hflag) res -= 1; \
   if(res & 0x02) _F |= 0x20; \
   if(res & 0x08) _F |= 0x08; \
   if(_BC) _F |= Vflag; \
}

#define CPIR \
   CPI; \
   if(_BC && !(_F & Zflag)) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
      iWSAdjust++; \
   }

#define IND \
{ \
   UBYTE io = z80_IN_handler(z80.BC); \
   _B--; \
   write_mem(_HL, io); \
   _HL--; \
   _F = SZ[_B]; \
   if(io & Sflag) _F |= Nflag; \
   if((((_C - 1) & 0xff) + io) & 0x100) _F |= Hflag | Cflag; \
   if((drep_tmp1[_C & 3][io & 3] ^ breg_tmp2[_B] ^ (_C >> 2) ^ (io >> 2)) & 1) \
      _F |= Pflag; \
}

#define INDR \
   IND; \
   if(_B) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
   }

#define INI \
{ \
   UBYTE io = z80_IN_handler(z80.BC); \
   _B--; \
   write_mem(_HL, io); \
   _HL++; \
   _F = SZ[_B]; \
   if(io & Sflag) _F |= Nflag; \
   if((((_C + 1) & 0xff) + io) & 0x100) _F |= Hflag | Cflag; \
   if((irep_tmp1[_C & 3][io & 3] ^ breg_tmp2[_B] ^ (_C >> 2) ^ (io >> 2)) & 1) \
      _F |= Pflag; \
}

#define INIR \
   INI; \
   if(_B) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
   }

#define LDD \
{ \
   UBYTE io = read_mem(_HL); \
   write_mem(_DE, io); \
   _F &= Sflag | Zflag | Cflag; \
   if((_A + io) & 0x02) _F |= 0x20; \
   if((_A + io) & 0x08) _F |= 0x08; \
   _HL--; \
   _DE--; \
   _BC--; \
   if(_BC) _F |= Vflag; \
}

#define LDDR \
   LDD; \
   if(_BC) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
   }

#define LDI \
{ \
   UBYTE io = read_mem(_HL); \
   write_mem(_DE, io); \
   _F &= Sflag | Zflag | Cflag; \
   if((_A + io) & 0x02) _F |= 0x20; \
   if((_A + io) & 0x08) _F |= 0x08; \
   _HL++; \
   _DE++; \
   _BC--; \
   if(_BC) _F |= Vflag; \
}

#define LDIR \
   LDI; \
   if(_BC) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
   }

#define NEG \
{ \
   UBYTE value = _A; \
   _A = 0; \
   SUB(value); \
}

#define OUTD \
{ \
   UBYTE io = read_mem(_HL); \
   _B--; \
   z80_OUT_handler(z80.BC, io); \
   _HL--; \
   _F = SZ[_B]; \
   if(io & Sflag) _F |= Nflag; \
   if((((_C - 1) & 0xff) + io) & 0x100) _F |= Hflag | Cflag; \
   if((drep_tmp1[_C & 3][io & 3] ^ breg_tmp2[_B] ^ (_C >> 2) ^ (io >> 2)) & 1) \
      _F |= Pflag; \
}

#define OTDR \
   OUTD; \
   if(_B) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
   }

#define OUTI \
{ \
   UBYTE io = read_mem(_HL); \
   _B--; \
   z80_OUT_handler(z80.BC, io); \
   _HL++; \
   _F = SZ[_B]; \
   if(io & Sflag) _F |= Nflag; \
   if((((_C + 1) & 0xff) + io) & 0x100) _F |= Hflag | Cflag; \
   if((irep_tmp1[_C & 3][io & 3] ^ breg_tmp2[_B] ^ (_C >> 2) ^ (io >> 2)) & 1) \
      _F |= Pflag; \
}

#define OTIR \
   OUTI; \
   if(_B) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      _PC -= 2; \
   }

#define RLD \
{ \
   UBYTE n = read_mem(_HL); \
   write_mem(_HL, (n << 4) | (_A & 0x0f)); \
   _A = (_A & 0xf0) | (n >> 4); \
   _F = (_F & Cflag) | SZP[_A]; \
}

#define RRD \
{ \
   UBYTE n = read_mem(_HL); \
   write_mem(_HL, (n >> 4) | (_A << 4)); \
   _A = (_A & 0xf0) | (n & 0x0f); \
   _F = (_F & Cflag) | SZP[_A]; \
}

#define SBC16(reg) \
{ \
   DWORD res = _HLdword - z80.reg.d - (_F & Cflag); \
   _F = (((_HLdword ^ res ^ z80.reg.d) >> 8) & Hflag) | Nflag | \
      ((res >> 16) & Cflag) | \
      ((res >> 8) & (Sflag | Xflags)) | \
      ((res & 0xffff) ? 0 : Zflag) | \
      (((z80.reg.d ^ _HLdword) & (_HLdword ^ res) &0x8000) >> 13); \
   _HL = (UWORD)res; \
}


#endif // Z80_MACROS_H
