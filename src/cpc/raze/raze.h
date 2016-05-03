/*
 * RAZE-x86 Z80 emulator.
 *
 * Copyright (c) 1999 Richard Mitton
 *
 * This may only be distributed as part of the complete RAZE package.
 * See RAZE.TXT for license information.
 */

#ifndef __RAZE_H_INCLUDED__
#define __RAZE_H_INCLUDED__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif


   /* Memory map constants */
   #define Z80_MAP_DIRECT  0  /* Reads/writes are done directly */
   #define Z80_MAP_HANDLED 1  /* Reads/writes use a function handler */

   /* Z80 registers */
   typedef enum {
      Z80_REG_AF=0,
      Z80_REG_BC,
      Z80_REG_DE,
      Z80_REG_HL,
      Z80_REG_IX,
      Z80_REG_IY,
      Z80_REG_PC,
      Z80_REG_SP,
      Z80_REG_AF2,
      Z80_REG_BC2,
      Z80_REG_DE2,
      Z80_REG_HL2,
      Z80_REG_IFF1,        /* boolean - 1 or 0 */
      Z80_REG_IFF2,        /* boolean - 1 or 0 */
      Z80_REG_IR,
      Z80_REG_IM,          /* 0, 1, or 2 */
      Z80_REG_IRQVector,   /* 0x00 to 0xff */
      Z80_REG_IRQLine,     /* boolean - 1 or 0 */
      Z80_REG_Halted       /* boolean - 1 or 0 */
   } z80_register;

   /* Z80 main functions */
   void  z80_reset(void);
   int   z80_emulate(int cycles);
   void  z80_raise_IRQ(tUBYTE vector);
   void  z80_lower_IRQ(void);
   void  z80_cause_NMI(void);

   /* Z80 context functions */
   int   z80_get_context_size(void);
   void  z80_set_context(void *context);
   void  z80_get_context(void *context);
   tUWORD z80_get_reg(z80_register reg);
   void  z80_set_reg(z80_register reg, tUWORD value);

   /* Z80 cycle functions */
   int   z80_get_cycles_elapsed(void);
   void  z80_stop_emulating(void);
   void  z80_skip_idle(void);
   void  z80_do_wait_states(int n);

   /* Z80 I/O functions */
   void  z80_init_memmap(void);
   void  z80_map_fetch(tUWORD start, tUWORD end, tUBYTE *memory);
   void  z80_map_read(tUWORD start, tUWORD end, tUBYTE *memory);
   void  z80_map_write(tUWORD start, tUWORD end, tUBYTE *memory);
   void  z80_add_read(tUWORD start, tUWORD end, int method, void *data);
   void  z80_add_write(tUWORD start, tUWORD end, int method, void *data);
   void  z80_set_in(tUBYTE (*handler)(tUWORD port));
   void  z80_set_out(void (*handler)(tUWORD port, tUBYTE value));
   void  z80_set_reti(void (*handler)(void));
   void  z80_set_av(void (*handler)(void));
   void  z80_set_fetch_callback(void (*handler)(tUWORD pc));
   void  z80_end_memmap(void);

#ifdef __cplusplus
};
#endif

class Raze
{

public:
    Raze()  {mIntPending=0;}
    ~Raze() {}

    typedef tUBYTE (*Z80_IN_Handler) (tREGPAIR port);
    typedef void  (*Z80_OUT_Handler)(tREGPAIR port, tUBYTE value);

    void setInHandler (Z80_IN_Handler handler)  {}
    void setOutHandler(Z80_OUT_Handler handler) {}

    void init() {mLastCyclecount=0; mCycleCountInit=0;}

    void initMemMap() {z80_init_memmap();}
    void endMemMap()  {z80_end_memmap(); z80_reset();}

    void setMembank_read (tUBYTE bank, tUBYTE* ptr)
    {
        if (bank==0) {z80_map_read(0x0000, 0x3fff, ptr); z80_map_fetch(0x0000, 0x3fff, ptr); return;}
        if (bank==1) {z80_map_read(0x4000, 0x7fff, ptr); z80_map_fetch(0x4000, 0x7fff, ptr); return;}
        if (bank==2) {z80_map_read(0x8000, 0xbfff, ptr); z80_map_fetch(0x8000, 0xbfff, ptr); return;}
        if (bank==3) {z80_map_read(0xc000, 0xffff, ptr); z80_map_fetch(0xc000, 0xffff, ptr); return;}
    }
    void setMembank_write(tUBYTE bank, tUBYTE* ptr)
    {
        if (bank==0) {z80_map_write(0x0000, 0x3fff, ptr); return;}
        if (bank==1) {z80_map_write(0x4000, 0x7fff, ptr); return;}
        if (bank==2) {z80_map_write(0x8000, 0xbfff, ptr); return;}
        if (bank==3) {z80_map_write(0xc000, 0xffff, ptr); return;}
    }

    int  cyclecount() {int i=mLastCyclecount; mLastCyclecount=z80_get_cycles_elapsed(); return mLastCyclecount-i;}
    void setIntPending(tUBYTE ip) {/*mIntPending=ip;*/if (ip) {z80_raise_IRQ(0xff); z80_lower_IRQ();}}
    tUBYTE intPending() {return mIntPending;}

    void setInHandler (tUBYTE (*handler)(tUWORD port))  {z80_set_in(handler);}
    void setOutHandler(void (*handler)(tUWORD port, tUBYTE value)) {z80_set_out(handler);}
    void setWsHandler (void (*handler)(void))  {z80_set_av(handler);}

    int  execute(int i) {mLastCyclecount=0; mCycleCountInit=i; return z80_emulate(i);}
    void stop()         {z80_stop_emulating();}

private:
    //Z80_IN_Handler  IN_handler;
    //Z80_OUT_Handler OUT_handler;

    //tUBYTE z80_in_handler(tREGPAIR port)
    //void z80_out_handler(tREGPAIR port, tUBYTE value)
    int mLastCyclecount;
    int mCycleCountInit;
    tUBYTE mIntPending;

};


#endif /* __RAZE_H_INCLUDED__ */

