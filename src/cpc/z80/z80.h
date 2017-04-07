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
#ifndef CPC_Z80_H
#define CPC_Z80_H

#include "types.h"
#include "register.h"

namespace cpcx
{

#define MF2_ACTIVE      1
#define MF2_RUNNING     2
#define MF2_INVISIBLE   4

    class Cpc;
    extern Cpc cpc;

#define z80_IN_handler  (cpc.*IN_handler)
#define z80_OUT_handler (cpc.*OUT_handler)
#define z80_wait_states (cpc.*WS_handler)(); iCycleCountSum += iCycleCount;


    /** @author Fred Klaus */
    class Z80 final
    {

        // using Z80_IN_Handler = tUBYTE (*) (tREGPAIR port);
        typedef tUBYTE (Cpc::*Z80_IN_Handler) (tREGPAIR port);
        typedef void   (Cpc::*Z80_OUT_Handler)(tREGPAIR port, tUBYTE value);
        typedef void   (Cpc::*Z80_WS_Handler) (void);

    public:
        Z80();
        ~Z80() ROLAND_DEFAULT

        //typedef void (*CB_WaitStates)(void);


        enum Flags : tUBYTE
        {
            Sflag  = 0x80, // sign flag
            Zflag  = 0x40, // zero flag
            Hflag  = 0x10, // halfcarry flag
            Pflag  = 0x04, // parity flag
            Vflag  = 0x04, // overflow flag
            Nflag  = 0x02, // negative flag
            Cflag  = 0x01, // carry flag
            Xflags = 0x28  // bit 5 & 3 flags
        };

        enum Flags2 : tUBYTE
        {
            CF = 0x01,
            NF = 0x02,
            PF = 0x04,
            VF = PF,
            XF = 0x08,
            HF = 0x10,
            YF = 0x20,
            ZF = 0x40,
            SF = 0x80
        };


        enum ExitCode : tUBYTE
        {
            EC_BREAKPOINT     = 10,
            EC_TRACE          = 20,
            EC_FRAME_COMPLETE = 30,
            EC_CYCLE_COUNT    = 40,
            EC_SOUND_BUFFER   = 50
        };


        enum opcodes : tUBYTE
        {
            nop, ld_bc_word, ld_mbc_a, inc_bc, inc_b, dec_b, ld_b_byte, rlca,
            ex_af_af, add_hl_bc, ld_a_mbc, dec_bc, inc_c, dec_c, ld_c_byte, rrca,
            djnz, ld_de_word, ld_mde_a, inc_de, inc_d, dec_d, ld_d_byte, rla,
            jr, add_hl_de, ld_a_mde, dec_de, inc_e, dec_e, ld_e_byte, rra,
            jr_nz, ld_hl_word, ld_mword_hl, inc_hl, inc_h, dec_h, ld_h_byte, daa,
            jr_z, add_hl_hl, ld_hl_mword, dec_hl, inc_l, dec_l, ld_l_byte, cpl,
            jr_nc, ld_sp_word, ld_mword_a, inc_sp, inc_mhl, dec_mhl, ld_mhl_byte, scf,
            jr_c, add_hl_sp, ld_a_mword, dec_sp, inc_a, dec_a, ld_a_byte, ccf,
            ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_mhl, ld_b_a,
            ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_mhl, ld_c_a,
            ld_d_b, ld_d_c, ld_d_d, ld_d_e, ld_d_h, ld_d_l, ld_d_mhl, ld_d_a,
            ld_e_b, ld_e_c, ld_e_d, ld_e_e, ld_e_h, ld_e_l, ld_e_mhl, ld_e_a,
            ld_h_b, ld_h_c, ld_h_d, ld_h_e, ld_h_h, ld_h_l, ld_h_mhl, ld_h_a,
            ld_l_b, ld_l_c, ld_l_d, ld_l_e, ld_l_h, ld_l_l, ld_l_mhl, ld_l_a,
            ld_mhl_b, ld_mhl_c, ld_mhl_d, ld_mhl_e, ld_mhl_h, ld_mhl_l, halt, ld_mhl_a,
            ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_mhl, ld_a_a,
            add_b, add_c, add_d, add_e, add_h, add_l, add_mhl, add_a,
            adc_b, adc_c, adc_d, adc_e, adc_h, adc_l, adc_mhl, adc_a,
            sub_b, sub_c, sub_d, sub_e, sub_h, sub_l, sub_mhl, sub_a,
            sbc_b, sbc_c, sbc_d, sbc_e, sbc_h, sbc_l, sbc_mhl, sbc_a,
            and_b, and_c, and_d, and_e, and_h, and_l, and_mhl, and_a,
            xor_b, xor_c, xor_d, xor_e, xor_h, xor_l, xor_mhl, xor_a,
            or_b, or_c, or_d, or_e, or_h, or_l, or_mhl, or_a,
            cp_b, cp_c, cp_d, cp_e, cp_h, cp_l, cp_mhl, cp_a,
            ret_nz, pop_bc, jp_nz, jp, call_nz, push_bc, add_byte, rst00,
            ret_z, ret, jp_z, pfx_cb, call_z, call, adc_byte, rst08,
            ret_nc, pop_de, jp_nc, outa, call_nc, push_de, sub_byte, rst10,
            ret_c, exx, jp_c, ina, call_c, pfx_dd, sbc_byte, rst18,
            ret_po, pop_hl, jp_po, ex_msp_hl, call_po, push_hl, and_byte, rst20,
            ret_pe, ld_pc_hl, jp_pe, ex_de_hl, call_pe, pfx_ed, xor_byte, rst28,
            ret_p, pop_af, jp_p, di, call_p, push_af, or_byte, rst30,
            ret_m, ld_sp_hl, jp_m, ei, call_m, pfx_fd, cp_byte, rst38
        };

        enum CBcodes : tUBYTE
        {
            rlc_b, rlc_c, rlc_d, rlc_e, rlc_h, rlc_l, rlc_mhl, rlc_a,
            rrc_b, rrc_c, rrc_d, rrc_e, rrc_h, rrc_l, rrc_mhl, rrc_a,
            rl_b, rl_c, rl_d, rl_e, rl_h, rl_l, rl_mhl, rl_a,
            rr_b, rr_c, rr_d, rr_e, rr_h, rr_l, rr_mhl, rr_a,
            sla_b, sla_c, sla_d, sla_e, sla_h, sla_l, sla_mhl, sla_a,
            sra_b, sra_c, sra_d, sra_e, sra_h, sra_l, sra_mhl, sra_a,
            sll_b, sll_c, sll_d, sll_e, sll_h, sll_l, sll_mhl, sll_a,
            srl_b, srl_c, srl_d, srl_e, srl_h, srl_l, srl_mhl, srl_a,
            bit0_b, bit0_c, bit0_d, bit0_e, bit0_h, bit0_l, bit0_mhl, bit0_a,
            bit1_b, bit1_c, bit1_d, bit1_e, bit1_h, bit1_l, bit1_mhl, bit1_a,
            bit2_b, bit2_c, bit2_d, bit2_e, bit2_h, bit2_l, bit2_mhl, bit2_a,
            bit3_b, bit3_c, bit3_d, bit3_e, bit3_h, bit3_l, bit3_mhl, bit3_a,
            bit4_b, bit4_c, bit4_d, bit4_e, bit4_h, bit4_l, bit4_mhl, bit4_a,
            bit5_b, bit5_c, bit5_d, bit5_e, bit5_h, bit5_l, bit5_mhl, bit5_a,
            bit6_b, bit6_c, bit6_d, bit6_e, bit6_h, bit6_l, bit6_mhl, bit6_a,
            bit7_b, bit7_c, bit7_d, bit7_e, bit7_h, bit7_l, bit7_mhl, bit7_a,
            res0_b, res0_c, res0_d, res0_e, res0_h, res0_l, res0_mhl, res0_a,
            res1_b, res1_c, res1_d, res1_e, res1_h, res1_l, res1_mhl, res1_a,
            res2_b, res2_c, res2_d, res2_e, res2_h, res2_l, res2_mhl, res2_a,
            res3_b, res3_c, res3_d, res3_e, res3_h, res3_l, res3_mhl, res3_a,
            res4_b, res4_c, res4_d, res4_e, res4_h, res4_l, res4_mhl, res4_a,
            res5_b, res5_c, res5_d, res5_e, res5_h, res5_l, res5_mhl, res5_a,
            res6_b, res6_c, res6_d, res6_e, res6_h, res6_l, res6_mhl, res6_a,
            res7_b, res7_c, res7_d, res7_e, res7_h, res7_l, res7_mhl, res7_a,
            set0_b, set0_c, set0_d, set0_e, set0_h, set0_l, set0_mhl, set0_a,
            set1_b, set1_c, set1_d, set1_e, set1_h, set1_l, set1_mhl, set1_a,
            set2_b, set2_c, set2_d, set2_e, set2_h, set2_l, set2_mhl, set2_a,
            set3_b, set3_c, set3_d, set3_e, set3_h, set3_l, set3_mhl, set3_a,
            set4_b, set4_c, set4_d, set4_e, set4_h, set4_l, set4_mhl, set4_a,
            set5_b, set5_c, set5_d, set5_e, set5_h, set5_l, set5_mhl, set5_a,
            set6_b, set6_c, set6_d, set6_e, set6_h, set6_l, set6_mhl, set6_a,
            set7_b, set7_c, set7_d, set7_e, set7_h, set7_l, set7_mhl, set7_a
        };

        enum EDcodes : tUBYTE
        {
            ed_00, ed_01, ed_02, ed_03, ed_04, ed_05, ed_06, ed_07,
            ed_08, ed_09, ed_0a, ed_0b, ed_0c, ed_0d, ed_0e, ed_0f,
            ed_10, ed_11, ed_12, ed_13, ed_14, ed_15, ed_16, ed_17,
            ed_18, ed_19, ed_1a, ed_1b, ed_1c, ed_1d, ed_1e, ed_1f,
            ed_20, ed_21, ed_22, ed_23, ed_24, ed_25, ed_26, ed_27,
            ed_28, ed_29, ed_2a, ed_2b, ed_2c, ed_2d, ed_2e, ed_2f,
            ed_30, ed_31, ed_32, ed_33, ed_34, ed_35, ed_36, ed_37,
            ed_38, ed_39, ed_3a, ed_3b, ed_3c, ed_3d, ed_3e, ed_3f,
            in_b_c, out_c_b, sbc_hl_bc, ld_EDmword_bc, neg, retn, im_0, ld_i_a,
            in_c_c, out_c_c, adc_hl_bc, ld_EDbc_mword, neg_1, reti, im_0_1, ld_r_a,
            in_d_c, out_c_d, sbc_hl_de, ld_EDmword_de, neg_2, retn_1, im_1, ld_a_i,
            in_e_c, out_c_e, adc_hl_de, ld_EDde_mword, neg_3, reti_1, im_2, ld_a_r,
            in_h_c, out_c_h, sbc_hl_hl, ld_EDmword_hl, neg_4, retn_2, im_0_2, rrd,
            in_l_c, out_c_l, adc_hl_hl, ld_EDhl_mword, neg_5, reti_2, im_0_3, rld,
            in_0_c, out_c_0, sbc_hl_sp, ld_EDmword_sp, neg_6, retn_3, im_1_1, ed_77,
            in_a_c, out_c_a, adc_hl_sp, ld_EDsp_mword, neg_7, reti_3, im_2_1, ed_7f,
            ed_80, ed_81, ed_82, ed_83, ed_84, ed_85, ed_86, ed_87,
            ed_88, ed_89, ed_8a, ed_8b, ed_8c, ed_8d, ed_8e, ed_8f,
            ed_90, ed_91, ed_92, ed_93, ed_94, ed_95, ed_96, ed_97,
            ed_98, ed_99, ed_9a, ed_9b, ed_9c, ed_9d, ed_9e, ed_9f,
            ldi, cpi, ini, outi, ed_a4, ed_a5, ed_a6, ed_a7,
            ldd, cpd, ind, outd, ed_ac, ed_ad, ed_ae, ed_af,
            ldir, cpir, inir, otir, ed_b4, ed_b5, ed_b6, ed_b7,
            lddr, cpdr, indr, otdr, ed_bc, ed_bd, ed_be, ed_bf,
            ed_c0, ed_c1, ed_c2, ed_c3, ed_c4, ed_c5, ed_c6, ed_c7,
            ed_c8, ed_c9, ed_ca, ed_cb, ed_cc, ed_cd, ed_ce, ed_cf,
            ed_d0, ed_d1, ed_d2, ed_d3, ed_d4, ed_d5, ed_d6, ed_d7,
            ed_d8, ed_d9, ed_da, ed_db, ed_dc, ed_dd, ed_de, ed_df,
            ed_e0, ed_e1, ed_e2, ed_e3, ed_e4, ed_e5, ed_e6, ed_e7,
            ed_e8, ed_e9, ed_ea, ed_eb, ed_ec, ed_ed, ed_ee, ed_ef,
            ed_f0, ed_f1, ed_f2, ed_f3, ed_f4, ed_f5, ed_f6, ed_f7,
            ed_f8, ed_f9, ed_fa, ed_fb, ed_fc, ed_fd, ed_fe, ed_ff
        };

        //void  access_video_memory(int repeat_count);     // not provided by Z80.c

        tUBYTE read_mem  (tUWORD addr)                       // returns a tUBYTE from a 16KB memory bank
            {return (*(membank_read[addr >> 14] + (addr & 0x3fff)));}

        void  write_mem (tUWORD addr, tUBYTE val)          // writes a tUBYTE to a 16KB memory bank
            {*(membank_write[addr >> 14] + (addr & 0x3fff)) = val;}

        void setMembank_read (tUBYTE bank, tUBYTE* ptr) {membank_read [bank]=ptr;}
        void setMembank_write(tUBYTE bank, tUBYTE* ptr) {membank_write[bank]=ptr;}

        void setInHandler (Z80_IN_Handler handler)  {IN_handler=handler;}
        void setOutHandler(Z80_OUT_Handler handler) {OUT_handler=handler;}
        void setWsHandler (Z80_WS_Handler handler)  {WS_handler=handler;}


        Register & reg() {return z80;}

        void init();
        int  execute(int cc);
        void stop() {mStop=true;}
        void init_tables();

        int  cycleCount() {return iCycleCount;}

        void mf2stop();
        void setMF2ExitAddr(tUDWORD addr) {dwMF2ExitAddr=addr;}

        //void doNothing() {return;}

        void  setIntPending(tUBYTE ip) {z80.int_pending=ip;}
        tUBYTE intPending   ()         {return z80.int_pending;}

        void initMemMap() {}
        void endMemMap()  {}

        void z80_pfx_cb();
        void z80_pfx_dd();
        void z80_pfx_ddcb();
        void z80_pfx_ed();
        void z80_pfx_fd();
        void z80_pfx_fdcb();

        inline tUBYTE RES(tUBYTE bit, tUBYTE val);
        inline tUBYTE RLC(tUBYTE val);
        inline tUBYTE RL (tUBYTE val);
        inline tUBYTE RRC(tUBYTE val);
        inline tUBYTE RR (tUBYTE val);
        inline tUBYTE SET(tUBYTE bit, tUBYTE val);
        inline tUBYTE SLA(tUBYTE val);
        inline tUBYTE SLL(tUBYTE val);
        inline tUBYTE SRA(tUBYTE val);
        inline tUBYTE SRL(tUBYTE val);

    private:
        Register z80;

        tUBYTE* membank_read [4];
        tUBYTE* membank_write[4];

        Z80_IN_Handler  IN_handler;
        Z80_OUT_Handler OUT_handler;
        Z80_WS_Handler  WS_handler;

        tDWORD dwMF2ExitAddr;
        tDWORD dwMF2Flags;

        int iCycleCountSum;
        int iCycleCount;
        int iCycleCountInit;
        int iWSAdjust;

        bool mStop;

        static tUBYTE irep_tmp1[4][4];
        static tUBYTE drep_tmp1[4][4];
        static tUBYTE breg_tmp2[256];
        static tUBYTE cc_op[256];
        static tUBYTE cc_cb[256];
        static tUBYTE cc_ed[256];
        static tUBYTE cc_xy[256];
        static tUBYTE cc_ex[256];
        static tUBYTE cc_xycb[256];
        static tUWORD DAATable[2048];

        tUBYTE SZ[256];       // zero and sign flags
        tUBYTE SZ_BIT[256];   // zero, sign and parity/overflow (=zero) flags for BIT opcode
        tUBYTE SZP[256];      // zero, sign and parity flags
        tUBYTE SZHV_inc[256]; // zero, sign, half carry and overflow flags INC r8
        tUBYTE SZHV_dec[256]; // zero, sign, half carry and overflow flags DEC r8

    };

} // cpc

#endif // CPC_Z80_H

