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
   if (z80.IFF1) { // process interrupts? \
      z80.R++; \
      z80.IFF1 = z80.IFF2 = 0; // clear interrupt flip-flops \
      z80.int_pending = 0; \
      cpc.gatearray().setSlCount(cpc.gatearray().slCount() & 0xdf); // clear bit 5 of GA scanline counter  \
      if (z80.HALT) { // HALT instruction active?  \
         z80.HALT = 0; // exit HALT 'loop'  \
         z80.PC.w.l++; // correct PC  \
      } \
      if (z80.IM < 2) { // interrupt mode 0 or 1? (IM0 = IM1 on the CPC)  \
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
         write_mem(--z80.SP.w.l, z80.PC.b.h); // store high byte of current PC  \
         write_mem(--z80.SP.w.l, z80.PC.b.l); // store low byte of current PC  \
         addr.b.l = 0xff; // assemble pointer  \
         addr.b.h = MF2_RUNNING; \
         z80.PC.b.l = read_mem(addr.w.l); // retrieve low byte of vector  \
         z80.PC.b.h = read_mem(addr.w.l+1); // retrieve high byte of vector  \
         z80_wait_states \
      } \
   } \
}

*/

#define z80_int_handler \
{ \
    if (z80.IFF1) \
    {  \
        z80.R++; \
        z80.IFF1 = z80.IFF2 = 0;  \
        z80.int_pending = 0; \
        cpc.gatearray().setSlCount(cpc.gatearray().slCount() & 0xdf);   \
        if (z80.HALT) \
        {   \
            z80.HALT = 0;   \
            z80.PC.w.l++;   \
        } \
        if (z80.IM < 2)   \
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
            write_mem(--z80.SP.w.l, z80.PC.b.h);   \
            write_mem(--z80.SP.w.l, z80.PC.b.l);   \
            addr.b.l = 0xff;   \
            addr.b.h = MF2_RUNNING; \
            z80.PC.b.l = read_mem(addr.w.l);   \
            z80.PC.b.h = read_mem(addr.w.l+1);   \
            z80_wait_states \
        } \
    } \
}


#define ADC(value) \
{ \
   unsigned val = value; \
   unsigned res = z80.AF.b.h + val + (z80.AF.b.l & Cflag); \
   z80.AF.b.l = SZ[res & 0xff] | ((res >> 8) & Cflag) | ((z80.AF.b.h ^ res ^ val) & Hflag) | \
      (((val ^ z80.AF.b.h ^ 0x80) & (val ^ res) & 0x80) >> 5); \
   z80.AF.b.h = res; \
}

#define ADD(value) \
{ \
   unsigned val = value; \
   unsigned res = z80.AF.b.h + val; \
   z80.AF.b.l = SZ[(UBYTE)res] | ((res >> 8) & Cflag) | ((z80.AF.b.h ^ res ^ val) & Hflag) | \
      (((val ^ z80.AF.b.h ^ 0x80) & (val ^ res) & 0x80) >> 5); \
   z80.AF.b.h = (UBYTE)res; \
}

#define ADD16(dest, src) \
{ \
   DWORD res = z80.dest.d + z80.src.d; \
   z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Vflag)) | (((z80.dest.d ^ res ^ z80.src.d) >> 8) & Hflag) | \
      ((res >> 16) & Cflag) | ((res >> 8) & Xflags); \
   z80.dest.w.l = (UWORD)res; \
}

#define AND(val) \
{ \
   z80.AF.b.h &= val; \
   z80.AF.b.l = SZP[z80.AF.b.h] | Hflag; \
}

#define CALL \
{ \
   REGPAIR dest; \
   dest.b.l = read_mem(z80.PC.w.l++); /* subroutine address low byte */ \
   dest.b.h = read_mem(z80.PC.w.l++); /* subroutine address high byte */ \
   write_mem(--z80.SP.w.l, z80.PC.b.h); /* store high byte of current PC */ \
   write_mem(--z80.SP.w.l, z80.PC.b.l); /* store low byte of current PC */ \
   z80.PC.w.l = dest.w.l; /* continue execution at subroutine */ \
}

#define CP(value) \
{ \
   unsigned val = value; \
   unsigned res = z80.AF.b.h - val; \
   z80.AF.b.l = (SZ[res & 0xff] & (Sflag | Zflag)) | (val & Xflags) | ((res >> 8) & Cflag) | Nflag | ((z80.AF.b.h ^ res ^ val) & Hflag) | \
      ((((val ^ z80.AF.b.h) & (z80.AF.b.h ^ res)) >> 5) & Vflag); \
}

#define DAA \
{ \
   int idx = z80.AF.b.h; \
   if(z80.AF.b.l & Cflag) \
      idx |= 0x100; \
   if(z80.AF.b.l & Hflag) \
      idx |= 0x200; \
   if(z80.AF.b.l & Nflag) \
      idx |= 0x400; \
   z80.AF.w.l = DAATable[idx]; \
}

#define DEC(reg) \
{ \
   reg--; \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | SZHV_dec[reg]; \
}

#define JR \
{ \
   signed char offset; \
   offset = (signed char)(read_mem(z80.PC.w.l)); /* grab signed jump offset */ \
   z80.PC.w.l += offset + 1; /* add offset & correct PC */ \
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
   temp.b.l = read_mem(z80.SP.w.l++); \
   temp.b.h = read_mem(z80.SP.w.l); \
   write_mem(z80.SP.w.l--, z80.reg.b.h); \
   write_mem(z80.SP.w.l, z80.reg.b.l); \
   z80.reg.w.l = temp.w.l; \
}

#define INC(reg) \
{ \
   reg++; \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | SZHV_inc[reg]; \
}

#define JP \
{ \
   REGPAIR addr; \
   addr.b.l = read_mem(z80.PC.w.l++); \
   addr.b.h = read_mem(z80.PC.w.l); \
   z80.PC.w.l = addr.w.l; \
}

#define LD16_MEM(reg) \
{ \
   REGPAIR addr; \
   addr.b.l = read_mem(z80.PC.w.l++); \
   addr.b.h = read_mem(z80.PC.w.l++); \
   z80.reg.b.l = read_mem(addr.w.l); \
   z80.reg.b.h = read_mem(addr.w.l+1); \
}

#define LDMEM_16(reg) \
{ \
   REGPAIR addr; \
   addr.b.l = read_mem(z80.PC.w.l++); \
   addr.b.h = read_mem(z80.PC.w.l++); \
   write_mem(addr.w.l, z80.reg.b.l); \
   write_mem(addr.w.l+1, z80.reg.b.h); \
}

#define OR(val) \
{ \
   z80.AF.b.h |= val; \
   z80.AF.b.l = SZP[z80.AF.b.h]; \
}

#define POP(reg) \
{ \
   z80.reg.b.l = read_mem(z80.SP.w.l++); \
   z80.reg.b.h = read_mem(z80.SP.w.l++); \
}

#define PUSH(reg) \
{ \
   write_mem(--z80.SP.w.l, z80.reg.b.h); \
   write_mem(--z80.SP.w.l, z80.reg.b.l); \
}

#define RET \
{ \
   z80.PC.b.l = read_mem(z80.SP.w.l++); \
   z80.PC.b.h = read_mem(z80.SP.w.l++); \
}

#define RLA \
{ \
   UBYTE res = (z80.AF.b.h << 1) | (z80.AF.b.l & Cflag); \
   UBYTE carry = (z80.AF.b.h & 0x80) ? Cflag : 0; \
   z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | carry | (res & Xflags); \
   z80.AF.b.h = res; \
}

#define RLCA \
{ \
   z80.AF.b.h = (z80.AF.b.h << 1) | (z80.AF.b.h >> 7); \
   z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | (z80.AF.b.h & (Xflags | Cflag)); \
}

#define RRA \
{ \
   UBYTE res = (z80.AF.b.h >> 1) | (z80.AF.b.l << 7); \
   UBYTE carry = (z80.AF.b.h & 0x01) ? Cflag : 0; \
   z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | carry | (res & Xflags); \
   z80.AF.b.h = res; \
}

#define RRCA \
{ \
   z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | (z80.AF.b.h & Cflag); \
   z80.AF.b.h = (z80.AF.b.h >> 1) | (z80.AF.b.h << 7); \
   z80.AF.b.l |= (z80.AF.b.h & Xflags); \
}

#define RST(addr) \
{ \
   write_mem(--z80.SP.w.l, z80.PC.b.h); /* store high byte of current PC */ \
   write_mem(--z80.SP.w.l, z80.PC.b.l); /* store low byte of current PC */ \
   z80.PC.w.l = addr; /* continue execution at restart address */ \
}

#define SBC(value) \
{ \
   unsigned val = value; \
   unsigned res = z80.AF.b.h - val - (z80.AF.b.l & Cflag); \
   z80.AF.b.l = SZ[res & 0xff] | ((res >> 8) & Cflag) | Nflag | ((z80.AF.b.h ^ res ^ val) & Hflag) | \
      (((val ^ z80.AF.b.h) & (z80.AF.b.h ^ res) & 0x80) >> 5); \
   z80.AF.b.h = res; \
}

#define SUB(value) \
{ \
   unsigned val = value; \
   unsigned res = z80.AF.b.h - val; \
   z80.AF.b.l = SZ[res & 0xff] | ((res >> 8) & Cflag) | Nflag | ((z80.AF.b.h ^ res ^ val) & Hflag) | \
      (((val ^ z80.AF.b.h) & (z80.AF.b.h ^ res) & 0x80) >> 5); \
   z80.AF.b.h = res; \
}

#define XOR(val) \
{ \
   z80.AF.b.h ^= val; \
   z80.AF.b.l = SZP[z80.AF.b.h]; \
}

#define BIT(bit, reg) \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | Hflag | SZ_BIT[reg & (1 << bit)]

#define BIT_XY BIT

// *************************************************************************************

#define ADC16(reg) \
{ \
   DWORD res = z80.HL.d + z80.reg.d + (z80.AF.b.l & Cflag); \
   z80.AF.b.l = (((z80.HL.d ^ res ^ z80.reg.d) >> 8) & Hflag) | \
      ((res >> 16) & Cflag) | \
      ((res >> 8) & (Sflag | Xflags)) | \
      ((res & 0xffff) ? 0 : Zflag) | \
      (((z80.reg.d ^ z80.HL.d ^ 0x8000) & (z80.reg.d ^ res) & 0x8000) >> 13); \
   z80.HL.w.l = (UWORD)res; \
}

#define CPD \
{ \
   UBYTE val = read_mem(z80.HL.w.l); \
   UBYTE res = z80.AF.b.h - val; \
   z80.HL.w.l--; \
   z80.BC.w.l--; \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | (SZ[res] & ~Xflags) | ((z80.AF.b.h ^ val ^ res) & Hflag) | Nflag; \
   if(z80.AF.b.l & Hflag) res -= 1; \
   if(res & 0x02) z80.AF.b.l |= 0x20; \
   if(res & 0x08) z80.AF.b.l |= 0x08; \
   if(z80.BC.w.l) z80.AF.b.l |= Vflag; \
}

#define CPDR \
   CPD; \
   if(z80.BC.w.l && !(z80.AF.b.l & Zflag)) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
      iWSAdjust++; \
   }

#define CPI \
{ \
   UBYTE val = read_mem(z80.HL.w.l); \
   UBYTE res = z80.AF.b.h - val; \
   z80.HL.w.l++; \
   z80.BC.w.l--; \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | (SZ[res] & ~Xflags) | ((z80.AF.b.h ^ val ^ res) & Hflag) | Nflag; \
   if(z80.AF.b.l & Hflag) res -= 1; \
   if(res & 0x02) z80.AF.b.l |= 0x20; \
   if(res & 0x08) z80.AF.b.l |= 0x08; \
   if(z80.BC.w.l) z80.AF.b.l |= Vflag; \
}

#define CPIR \
   CPI; \
   if(z80.BC.w.l && !(z80.AF.b.l & Zflag)) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
      iWSAdjust++; \
   }

#define IND \
{ \
   UBYTE io = z80_IN_handler(z80.BC); \
   z80.BC.b.h--; \
   write_mem(z80.HL.w.l, io); \
   z80.HL.w.l--; \
   z80.AF.b.l = SZ[z80.BC.b.h]; \
   if(io & Sflag) z80.AF.b.l |= Nflag; \
   if((((z80.BC.b.l - 1) & 0xff) + io) & 0x100) z80.AF.b.l |= Hflag | Cflag; \
   if((drep_tmp1[z80.BC.b.l & 3][io & 3] ^ breg_tmp2[z80.BC.b.h] ^ (z80.BC.b.l >> 2) ^ (io >> 2)) & 1) \
      z80.AF.b.l |= Pflag; \
}

#define INDR \
   IND; \
   if(z80.BC.b.h) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
   }

#define INI \
{ \
   UBYTE io = z80_IN_handler(z80.BC); \
   z80.BC.b.h--; \
   write_mem(z80.HL.w.l, io); \
   z80.HL.w.l++; \
   z80.AF.b.l = SZ[z80.BC.b.h]; \
   if(io & Sflag) z80.AF.b.l |= Nflag; \
   if((((z80.BC.b.l + 1) & 0xff) + io) & 0x100) z80.AF.b.l |= Hflag | Cflag; \
   if((irep_tmp1[z80.BC.b.l & 3][io & 3] ^ breg_tmp2[z80.BC.b.h] ^ (z80.BC.b.l >> 2) ^ (io >> 2)) & 1) \
      z80.AF.b.l |= Pflag; \
}

#define INIR \
   INI; \
   if(z80.BC.b.h) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
   }

#define LDD \
{ \
   UBYTE io = read_mem(z80.HL.w.l); \
   write_mem(z80.DE.w.l, io); \
   z80.AF.b.l &= Sflag | Zflag | Cflag; \
   if((z80.AF.b.h + io) & 0x02) z80.AF.b.l |= 0x20; \
   if((z80.AF.b.h + io) & 0x08) z80.AF.b.l |= 0x08; \
   z80.HL.w.l--; \
   z80.DE.w.l--; \
   z80.BC.w.l--; \
   if(z80.BC.w.l) z80.AF.b.l |= Vflag; \
}

#define LDDR \
   LDD; \
   if(z80.BC.w.l) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
   }

#define LDI \
{ \
   UBYTE io = read_mem(z80.HL.w.l); \
   write_mem(z80.DE.w.l, io); \
   z80.AF.b.l &= Sflag | Zflag | Cflag; \
   if((z80.AF.b.h + io) & 0x02) z80.AF.b.l |= 0x20; \
   if((z80.AF.b.h + io) & 0x08) z80.AF.b.l |= 0x08; \
   z80.HL.w.l++; \
   z80.DE.w.l++; \
   z80.BC.w.l--; \
   if(z80.BC.w.l) z80.AF.b.l |= Vflag; \
}

#define LDIR \
   LDI; \
   if(z80.BC.w.l) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
   }

#define NEG \
{ \
   UBYTE value = z80.AF.b.h; \
   z80.AF.b.h = 0; \
   SUB(value); \
}

#define OUTD \
{ \
   UBYTE io = read_mem(z80.HL.w.l); \
   z80.BC.b.h--; \
   z80_OUT_handler(z80.BC, io); \
   z80.HL.w.l--; \
   z80.AF.b.l = SZ[z80.BC.b.h]; \
   if(io & Sflag) z80.AF.b.l |= Nflag; \
   if((((z80.BC.b.l - 1) & 0xff) + io) & 0x100) z80.AF.b.l |= Hflag | Cflag; \
   if((drep_tmp1[z80.BC.b.l & 3][io & 3] ^ breg_tmp2[z80.BC.b.h] ^ (z80.BC.b.l >> 2) ^ (io >> 2)) & 1) \
      z80.AF.b.l |= Pflag; \
}

#define OTDR \
   OUTD; \
   if(z80.BC.b.h) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
   }

#define OUTI \
{ \
   UBYTE io = read_mem(z80.HL.w.l); \
   z80.BC.b.h--; \
   z80_OUT_handler(z80.BC, io); \
   z80.HL.w.l++; \
   z80.AF.b.l = SZ[z80.BC.b.h]; \
   if(io & Sflag) z80.AF.b.l |= Nflag; \
   if((((z80.BC.b.l + 1) & 0xff) + io) & 0x100) z80.AF.b.l |= Hflag | Cflag; \
   if((irep_tmp1[z80.BC.b.l & 3][io & 3] ^ breg_tmp2[z80.BC.b.h] ^ (z80.BC.b.l >> 2) ^ (io >> 2)) & 1) \
      z80.AF.b.l |= Pflag; \
}

#define OTIR \
   OUTI; \
   if(z80.BC.b.h) \
   { \
      iCycleCount += cc_ex[bOpCode]; \
      z80.PC.w.l -= 2; \
   }

#define RLD \
{ \
   UBYTE n = read_mem(z80.HL.w.l); \
   write_mem(z80.HL.w.l, (n << 4) | (z80.AF.b.h & 0x0f)); \
   z80.AF.b.h = (z80.AF.b.h & 0xf0) | (n >> 4); \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.AF.b.h]; \
}

#define RRD \
{ \
   UBYTE n = read_mem(z80.HL.w.l); \
   write_mem(z80.HL.w.l, (n >> 4) | (z80.AF.b.h << 4)); \
   z80.AF.b.h = (z80.AF.b.h & 0xf0) | (n & 0x0f); \
   z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.AF.b.h]; \
}

#define SBC16(reg) \
{ \
   DWORD res = z80.HL.d - z80.reg.d - (z80.AF.b.l & Cflag); \
   z80.AF.b.l = (((z80.HL.d ^ res ^ z80.reg.d) >> 8) & Hflag) | Nflag | \
      ((res >> 16) & Cflag) | \
      ((res >> 8) & (Sflag | Xflags)) | \
      ((res & 0xffff) ? 0 : Zflag) | \
      (((z80.reg.d ^ z80.HL.d) & (z80.HL.d ^ res) &0x8000) >> 13); \
   z80.HL.w.l = (UWORD)res; \
}


#endif // Z80_MACROS_H
