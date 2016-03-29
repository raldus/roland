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
#include "z80.h"

#include "macros.h"

#include "cpc.h"


UBYTE Z80::irep_tmp1[4][4] =
{
	{0, 0, 1, 0}, {0, 1, 0, 1}, {1, 0, 1, 1}, {0, 1, 1, 0}
};

/* tmp1 value for ind/indr/outd/otdr for [C.1-0][io.1-0] */
UBYTE Z80::drep_tmp1[4][4] =
{
	{0, 1, 0, 0}, {1, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 1}
};

/* tmp2 value for all in/out repeated opcodes for B.7-0 */
UBYTE Z80::breg_tmp2[256] =
{
	0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
	0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1
};

UBYTE Z80::cc_op[256] =
{
	4, 12,  8,  8,  4,  4,  8,  4,  4, 12,  8,  8,  4,  4,  8,  4,
	12, 12,  8,  8,  4,  4,  8,  4, 12, 12,  8,  8,  4,  4,  8,  4,
	8, 12, 20,  8,  4,  4,  8,  4,  8, 12, 20,  8,  4,  4,  8,  4,
	8, 12, 16,  8, 12, 12, 12,  4,  8, 12, 16,  8,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	8, 12, 12, 12, 12, 16,  8, 16,  8, 12, 12,  4, 12, 20,  8, 16,
	8, 12, 12, 12, 12, 16,  8, 16,  8,  4, 12, 12, 12,  4,  8, 16,
	8, 12, 12, 24, 12, 16,  8, 16,  8,  4, 12,  4, 12,  4,  8, 16,
	8, 12, 12,  4, 12, 16,  8, 16,  8,  8, 12,  4, 12,  4,  8, 16
};

UBYTE Z80::cc_cb[256] =
{
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
	4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4
};

UBYTE Z80::cc_ed[256] =
{
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	12, 12, 12, 20,  4, 12,  4,  8, 12, 12, 12, 20,  4, 12,  4,  8,
	12, 12, 12, 20,  4, 12,  4,  8, 12, 12, 12, 20,  4, 12,  4,  8,
	12, 12, 12, 20,  4, 12,  4, 16, 12, 12, 12, 20,  4, 12,  4, 16,
	12, 12, 12, 20,  4, 12,  4,  4, 12, 12, 12, 20,  4, 12,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	16, 12, 16, 16,  4,  4,  4,  4, 16, 12, 16, 16,  4,  4,  4,  4,
	16, 12, 16, 16,  4,  4,  4,  4, 16, 12, 16, 16,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4
};

UBYTE Z80::cc_xy[256] =
{
	4, 12,  8,  8,  4,  4,  8,  4,  4, 12,  8,  8,  4,  4,  8,  4,
	12, 12,  8,  8,  4,  4,  8,  4, 12, 12,  8,  8,  4,  4,  8,  4,
	8, 12, 20,  8,  4,  4,  8,  4,  8, 12, 20,  8,  4,  4,  8,  4,
	8, 12, 16,  8, 20, 20, 20,  4,  8, 12, 16,  8,  4,  4,  8,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	16, 16, 16, 16, 16, 16,  4, 16,  4,  4,  4,  4,  4,  4, 16,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
	8, 12, 12, 12, 12, 16,  8, 16,  8, 12, 12,  4, 12, 20,  8, 16,
	8, 12, 12, 12, 12, 16,  8, 16,  8,  4, 12, 12, 12,  4,  8, 16,
	8, 12, 12, 24, 12, 16,  8, 16,  8,  4, 12,  4, 12,  4,  8, 16,
	8, 12, 12,  4, 12, 16,  8, 16,  8,  8, 12,  4, 12,  4,  8, 16
};

UBYTE Z80::cc_xycb[256] =
{
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20
};

UBYTE Z80::cc_ex[256] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	4,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
	4,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	4,  8,  4,  4,  0,  0,  0,  0,  4,  8,  4,  4,  0,  0,  0,  0,
	8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0
};

UWORD Z80::DAATable[2048] =
{
	0x0044,0x0100,0x0200,0x0304,0x0400,0x0504,0x0604,0x0700,
	0x0808,0x090C,0x1010,0x1114,0x1214,0x1310,0x1414,0x1510,
	0x1000,0x1104,0x1204,0x1300,0x1404,0x1500,0x1600,0x1704,
	0x180C,0x1908,0x2030,0x2134,0x2234,0x2330,0x2434,0x2530,
	0x2020,0x2124,0x2224,0x2320,0x2424,0x2520,0x2620,0x2724,
	0x282C,0x2928,0x3034,0x3130,0x3230,0x3334,0x3430,0x3534,
	0x3024,0x3120,0x3220,0x3324,0x3420,0x3524,0x3624,0x3720,
	0x3828,0x392C,0x4010,0x4114,0x4214,0x4310,0x4414,0x4510,
	0x4000,0x4104,0x4204,0x4300,0x4404,0x4500,0x4600,0x4704,
	0x480C,0x4908,0x5014,0x5110,0x5210,0x5314,0x5410,0x5514,
	0x5004,0x5100,0x5200,0x5304,0x5400,0x5504,0x5604,0x5700,
	0x5808,0x590C,0x6034,0x6130,0x6230,0x6334,0x6430,0x6534,
	0x6024,0x6120,0x6220,0x6324,0x6420,0x6524,0x6624,0x6720,
	0x6828,0x692C,0x7030,0x7134,0x7234,0x7330,0x7434,0x7530,
	0x7020,0x7124,0x7224,0x7320,0x7424,0x7520,0x7620,0x7724,
	0x782C,0x7928,0x8090,0x8194,0x8294,0x8390,0x8494,0x8590,
	0x8080,0x8184,0x8284,0x8380,0x8484,0x8580,0x8680,0x8784,
	0x888C,0x8988,0x9094,0x9190,0x9290,0x9394,0x9490,0x9594,
	0x9084,0x9180,0x9280,0x9384,0x9480,0x9584,0x9684,0x9780,
	0x9888,0x998C,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
	0x0045,0x0101,0x0201,0x0305,0x0401,0x0505,0x0605,0x0701,
	0x0809,0x090D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
	0x1001,0x1105,0x1205,0x1301,0x1405,0x1501,0x1601,0x1705,
	0x180D,0x1909,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
	0x2021,0x2125,0x2225,0x2321,0x2425,0x2521,0x2621,0x2725,
	0x282D,0x2929,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
	0x3025,0x3121,0x3221,0x3325,0x3421,0x3525,0x3625,0x3721,
	0x3829,0x392D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
	0x4001,0x4105,0x4205,0x4301,0x4405,0x4501,0x4601,0x4705,
	0x480D,0x4909,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
	0x5005,0x5101,0x5201,0x5305,0x5401,0x5505,0x5605,0x5701,
	0x5809,0x590D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
	0x6025,0x6121,0x6221,0x6325,0x6421,0x6525,0x6625,0x6721,
	0x6829,0x692D,0x7031,0x7135,0x7235,0x7331,0x7435,0x7531,
	0x7021,0x7125,0x7225,0x7321,0x7425,0x7521,0x7621,0x7725,
	0x782D,0x7929,0x8091,0x8195,0x8295,0x8391,0x8495,0x8591,
	0x8081,0x8185,0x8285,0x8381,0x8485,0x8581,0x8681,0x8785,
	0x888D,0x8989,0x9095,0x9191,0x9291,0x9395,0x9491,0x9595,
	0x9085,0x9181,0x9281,0x9385,0x9481,0x9585,0x9685,0x9781,
	0x9889,0x998D,0xA0B5,0xA1B1,0xA2B1,0xA3B5,0xA4B1,0xA5B5,
	0xA0A5,0xA1A1,0xA2A1,0xA3A5,0xA4A1,0xA5A5,0xA6A5,0xA7A1,
	0xA8A9,0xA9AD,0xB0B1,0xB1B5,0xB2B5,0xB3B1,0xB4B5,0xB5B1,
	0xB0A1,0xB1A5,0xB2A5,0xB3A1,0xB4A5,0xB5A1,0xB6A1,0xB7A5,
	0xB8AD,0xB9A9,0xC095,0xC191,0xC291,0xC395,0xC491,0xC595,
	0xC085,0xC181,0xC281,0xC385,0xC481,0xC585,0xC685,0xC781,
	0xC889,0xC98D,0xD091,0xD195,0xD295,0xD391,0xD495,0xD591,
	0xD081,0xD185,0xD285,0xD381,0xD485,0xD581,0xD681,0xD785,
	0xD88D,0xD989,0xE0B1,0xE1B5,0xE2B5,0xE3B1,0xE4B5,0xE5B1,
	0xE0A1,0xE1A5,0xE2A5,0xE3A1,0xE4A5,0xE5A1,0xE6A1,0xE7A5,
	0xE8AD,0xE9A9,0xF0B5,0xF1B1,0xF2B1,0xF3B5,0xF4B1,0xF5B5,
	0xF0A5,0xF1A1,0xF2A1,0xF3A5,0xF4A1,0xF5A5,0xF6A5,0xF7A1,
	0xF8A9,0xF9AD,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
	0x0045,0x0101,0x0201,0x0305,0x0401,0x0505,0x0605,0x0701,
	0x0809,0x090D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
	0x1001,0x1105,0x1205,0x1301,0x1405,0x1501,0x1601,0x1705,
	0x180D,0x1909,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
	0x2021,0x2125,0x2225,0x2321,0x2425,0x2521,0x2621,0x2725,
	0x282D,0x2929,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
	0x3025,0x3121,0x3221,0x3325,0x3421,0x3525,0x3625,0x3721,
	0x3829,0x392D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
	0x4001,0x4105,0x4205,0x4301,0x4405,0x4501,0x4601,0x4705,
	0x480D,0x4909,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
	0x5005,0x5101,0x5201,0x5305,0x5401,0x5505,0x5605,0x5701,
	0x5809,0x590D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
	0x0604,0x0700,0x0808,0x090C,0x0A0C,0x0B08,0x0C0C,0x0D08,
	0x0E08,0x0F0C,0x1010,0x1114,0x1214,0x1310,0x1414,0x1510,
	0x1600,0x1704,0x180C,0x1908,0x1A08,0x1B0C,0x1C08,0x1D0C,
	0x1E0C,0x1F08,0x2030,0x2134,0x2234,0x2330,0x2434,0x2530,
	0x2620,0x2724,0x282C,0x2928,0x2A28,0x2B2C,0x2C28,0x2D2C,
	0x2E2C,0x2F28,0x3034,0x3130,0x3230,0x3334,0x3430,0x3534,
	0x3624,0x3720,0x3828,0x392C,0x3A2C,0x3B28,0x3C2C,0x3D28,
	0x3E28,0x3F2C,0x4010,0x4114,0x4214,0x4310,0x4414,0x4510,
	0x4600,0x4704,0x480C,0x4908,0x4A08,0x4B0C,0x4C08,0x4D0C,
	0x4E0C,0x4F08,0x5014,0x5110,0x5210,0x5314,0x5410,0x5514,
	0x5604,0x5700,0x5808,0x590C,0x5A0C,0x5B08,0x5C0C,0x5D08,
	0x5E08,0x5F0C,0x6034,0x6130,0x6230,0x6334,0x6430,0x6534,
	0x6624,0x6720,0x6828,0x692C,0x6A2C,0x6B28,0x6C2C,0x6D28,
	0x6E28,0x6F2C,0x7030,0x7134,0x7234,0x7330,0x7434,0x7530,
	0x7620,0x7724,0x782C,0x7928,0x7A28,0x7B2C,0x7C28,0x7D2C,
	0x7E2C,0x7F28,0x8090,0x8194,0x8294,0x8390,0x8494,0x8590,
	0x8680,0x8784,0x888C,0x8988,0x8A88,0x8B8C,0x8C88,0x8D8C,
	0x8E8C,0x8F88,0x9094,0x9190,0x9290,0x9394,0x9490,0x9594,
	0x9684,0x9780,0x9888,0x998C,0x9A8C,0x9B88,0x9C8C,0x9D88,
	0x9E88,0x9F8C,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
	0x0605,0x0701,0x0809,0x090D,0x0A0D,0x0B09,0x0C0D,0x0D09,
	0x0E09,0x0F0D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
	0x1601,0x1705,0x180D,0x1909,0x1A09,0x1B0D,0x1C09,0x1D0D,
	0x1E0D,0x1F09,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
	0x2621,0x2725,0x282D,0x2929,0x2A29,0x2B2D,0x2C29,0x2D2D,
	0x2E2D,0x2F29,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
	0x3625,0x3721,0x3829,0x392D,0x3A2D,0x3B29,0x3C2D,0x3D29,
	0x3E29,0x3F2D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
	0x4601,0x4705,0x480D,0x4909,0x4A09,0x4B0D,0x4C09,0x4D0D,
	0x4E0D,0x4F09,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
	0x5605,0x5701,0x5809,0x590D,0x5A0D,0x5B09,0x5C0D,0x5D09,
	0x5E09,0x5F0D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
	0x6625,0x6721,0x6829,0x692D,0x6A2D,0x6B29,0x6C2D,0x6D29,
	0x6E29,0x6F2D,0x7031,0x7135,0x7235,0x7331,0x7435,0x7531,
	0x7621,0x7725,0x782D,0x7929,0x7A29,0x7B2D,0x7C29,0x7D2D,
	0x7E2D,0x7F29,0x8091,0x8195,0x8295,0x8391,0x8495,0x8591,
	0x8681,0x8785,0x888D,0x8989,0x8A89,0x8B8D,0x8C89,0x8D8D,
	0x8E8D,0x8F89,0x9095,0x9191,0x9291,0x9395,0x9491,0x9595,
	0x9685,0x9781,0x9889,0x998D,0x9A8D,0x9B89,0x9C8D,0x9D89,
	0x9E89,0x9F8D,0xA0B5,0xA1B1,0xA2B1,0xA3B5,0xA4B1,0xA5B5,
	0xA6A5,0xA7A1,0xA8A9,0xA9AD,0xAAAD,0xABA9,0xACAD,0xADA9,
	0xAEA9,0xAFAD,0xB0B1,0xB1B5,0xB2B5,0xB3B1,0xB4B5,0xB5B1,
	0xB6A1,0xB7A5,0xB8AD,0xB9A9,0xBAA9,0xBBAD,0xBCA9,0xBDAD,
	0xBEAD,0xBFA9,0xC095,0xC191,0xC291,0xC395,0xC491,0xC595,
	0xC685,0xC781,0xC889,0xC98D,0xCA8D,0xCB89,0xCC8D,0xCD89,
	0xCE89,0xCF8D,0xD091,0xD195,0xD295,0xD391,0xD495,0xD591,
	0xD681,0xD785,0xD88D,0xD989,0xDA89,0xDB8D,0xDC89,0xDD8D,
	0xDE8D,0xDF89,0xE0B1,0xE1B5,0xE2B5,0xE3B1,0xE4B5,0xE5B1,
	0xE6A1,0xE7A5,0xE8AD,0xE9A9,0xEAA9,0xEBAD,0xECA9,0xEDAD,
	0xEEAD,0xEFA9,0xF0B5,0xF1B1,0xF2B1,0xF3B5,0xF4B1,0xF5B5,
	0xF6A5,0xF7A1,0xF8A9,0xF9AD,0xFAAD,0xFBA9,0xFCAD,0xFDA9,
	0xFEA9,0xFFAD,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
	0x0605,0x0701,0x0809,0x090D,0x0A0D,0x0B09,0x0C0D,0x0D09,
	0x0E09,0x0F0D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
	0x1601,0x1705,0x180D,0x1909,0x1A09,0x1B0D,0x1C09,0x1D0D,
	0x1E0D,0x1F09,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
	0x2621,0x2725,0x282D,0x2929,0x2A29,0x2B2D,0x2C29,0x2D2D,
	0x2E2D,0x2F29,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
	0x3625,0x3721,0x3829,0x392D,0x3A2D,0x3B29,0x3C2D,0x3D29,
	0x3E29,0x3F2D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
	0x4601,0x4705,0x480D,0x4909,0x4A09,0x4B0D,0x4C09,0x4D0D,
	0x4E0D,0x4F09,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
	0x5605,0x5701,0x5809,0x590D,0x5A0D,0x5B09,0x5C0D,0x5D09,
	0x5E09,0x5F0D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
	0x0046,0x0102,0x0202,0x0306,0x0402,0x0506,0x0606,0x0702,
	0x080A,0x090E,0x0402,0x0506,0x0606,0x0702,0x080A,0x090E,
	0x1002,0x1106,0x1206,0x1302,0x1406,0x1502,0x1602,0x1706,
	0x180E,0x190A,0x1406,0x1502,0x1602,0x1706,0x180E,0x190A,
	0x2022,0x2126,0x2226,0x2322,0x2426,0x2522,0x2622,0x2726,
	0x282E,0x292A,0x2426,0x2522,0x2622,0x2726,0x282E,0x292A,
	0x3026,0x3122,0x3222,0x3326,0x3422,0x3526,0x3626,0x3722,
	0x382A,0x392E,0x3422,0x3526,0x3626,0x3722,0x382A,0x392E,
	0x4002,0x4106,0x4206,0x4302,0x4406,0x4502,0x4602,0x4706,
	0x480E,0x490A,0x4406,0x4502,0x4602,0x4706,0x480E,0x490A,
	0x5006,0x5102,0x5202,0x5306,0x5402,0x5506,0x5606,0x5702,
	0x580A,0x590E,0x5402,0x5506,0x5606,0x5702,0x580A,0x590E,
	0x6026,0x6122,0x6222,0x6326,0x6422,0x6526,0x6626,0x6722,
	0x682A,0x692E,0x6422,0x6526,0x6626,0x6722,0x682A,0x692E,
	0x7022,0x7126,0x7226,0x7322,0x7426,0x7522,0x7622,0x7726,
	0x782E,0x792A,0x7426,0x7522,0x7622,0x7726,0x782E,0x792A,
	0x8082,0x8186,0x8286,0x8382,0x8486,0x8582,0x8682,0x8786,
	0x888E,0x898A,0x8486,0x8582,0x8682,0x8786,0x888E,0x898A,
	0x9086,0x9182,0x9282,0x9386,0x9482,0x9586,0x9686,0x9782,
	0x988A,0x998E,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
	0x4003,0x4107,0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,
	0x480F,0x490B,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
	0x5007,0x5103,0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,
	0x580B,0x590F,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
	0x6027,0x6123,0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,
	0x682B,0x692F,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
	0x7023,0x7127,0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,
	0x782F,0x792B,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
	0x8083,0x8187,0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,
	0x888F,0x898B,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
	0x9087,0x9183,0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,
	0x988B,0x998F,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F,
	0xA0A7,0xA1A3,0xA2A3,0xA3A7,0xA4A3,0xA5A7,0xA6A7,0xA7A3,
	0xA8AB,0xA9AF,0xA4A3,0xA5A7,0xA6A7,0xA7A3,0xA8AB,0xA9AF,
	0xB0A3,0xB1A7,0xB2A7,0xB3A3,0xB4A7,0xB5A3,0xB6A3,0xB7A7,
	0xB8AF,0xB9AB,0xB4A7,0xB5A3,0xB6A3,0xB7A7,0xB8AF,0xB9AB,
	0xC087,0xC183,0xC283,0xC387,0xC483,0xC587,0xC687,0xC783,
	0xC88B,0xC98F,0xC483,0xC587,0xC687,0xC783,0xC88B,0xC98F,
	0xD083,0xD187,0xD287,0xD383,0xD487,0xD583,0xD683,0xD787,
	0xD88F,0xD98B,0xD487,0xD583,0xD683,0xD787,0xD88F,0xD98B,
	0xE0A3,0xE1A7,0xE2A7,0xE3A3,0xE4A7,0xE5A3,0xE6A3,0xE7A7,
	0xE8AF,0xE9AB,0xE4A7,0xE5A3,0xE6A3,0xE7A7,0xE8AF,0xE9AB,
	0xF0A7,0xF1A3,0xF2A3,0xF3A7,0xF4A3,0xF5A7,0xF6A7,0xF7A3,
	0xF8AB,0xF9AF,0xF4A3,0xF5A7,0xF6A7,0xF7A3,0xF8AB,0xF9AF,
	0x0047,0x0103,0x0203,0x0307,0x0403,0x0507,0x0607,0x0703,
	0x080B,0x090F,0x0403,0x0507,0x0607,0x0703,0x080B,0x090F,
	0x1003,0x1107,0x1207,0x1303,0x1407,0x1503,0x1603,0x1707,
	0x180F,0x190B,0x1407,0x1503,0x1603,0x1707,0x180F,0x190B,
	0x2023,0x2127,0x2227,0x2323,0x2427,0x2523,0x2623,0x2727,
	0x282F,0x292B,0x2427,0x2523,0x2623,0x2727,0x282F,0x292B,
	0x3027,0x3123,0x3223,0x3327,0x3423,0x3527,0x3627,0x3723,
	0x382B,0x392F,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
	0x4003,0x4107,0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,
	0x480F,0x490B,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
	0x5007,0x5103,0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,
	0x580B,0x590F,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
	0x6027,0x6123,0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,
	0x682B,0x692F,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
	0x7023,0x7127,0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,
	0x782F,0x792B,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
	0x8083,0x8187,0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,
	0x888F,0x898B,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
	0x9087,0x9183,0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,
	0x988B,0x998F,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F,
	0xFABE,0xFBBA,0xFCBE,0xFDBA,0xFEBA,0xFFBE,0x0046,0x0102,
	0x0202,0x0306,0x0402,0x0506,0x0606,0x0702,0x080A,0x090E,
	0x0A1E,0x0B1A,0x0C1E,0x0D1A,0x0E1A,0x0F1E,0x1002,0x1106,
	0x1206,0x1302,0x1406,0x1502,0x1602,0x1706,0x180E,0x190A,
	0x1A1A,0x1B1E,0x1C1A,0x1D1E,0x1E1E,0x1F1A,0x2022,0x2126,
	0x2226,0x2322,0x2426,0x2522,0x2622,0x2726,0x282E,0x292A,
	0x2A3A,0x2B3E,0x2C3A,0x2D3E,0x2E3E,0x2F3A,0x3026,0x3122,
	0x3222,0x3326,0x3422,0x3526,0x3626,0x3722,0x382A,0x392E,
	0x3A3E,0x3B3A,0x3C3E,0x3D3A,0x3E3A,0x3F3E,0x4002,0x4106,
	0x4206,0x4302,0x4406,0x4502,0x4602,0x4706,0x480E,0x490A,
	0x4A1A,0x4B1E,0x4C1A,0x4D1E,0x4E1E,0x4F1A,0x5006,0x5102,
	0x5202,0x5306,0x5402,0x5506,0x5606,0x5702,0x580A,0x590E,
	0x5A1E,0x5B1A,0x5C1E,0x5D1A,0x5E1A,0x5F1E,0x6026,0x6122,
	0x6222,0x6326,0x6422,0x6526,0x6626,0x6722,0x682A,0x692E,
	0x6A3E,0x6B3A,0x6C3E,0x6D3A,0x6E3A,0x6F3E,0x7022,0x7126,
	0x7226,0x7322,0x7426,0x7522,0x7622,0x7726,0x782E,0x792A,
	0x7A3A,0x7B3E,0x7C3A,0x7D3E,0x7E3E,0x7F3A,0x8082,0x8186,
	0x8286,0x8382,0x8486,0x8582,0x8682,0x8786,0x888E,0x898A,
	0x8A9A,0x8B9E,0x8C9A,0x8D9E,0x8E9E,0x8F9A,0x9086,0x9182,
	0x9282,0x9386,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
	0x3A3F,0x3B3B,0x3C3F,0x3D3B,0x3E3B,0x3F3F,0x4003,0x4107,
	0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
	0x4A1B,0x4B1F,0x4C1B,0x4D1F,0x4E1F,0x4F1B,0x5007,0x5103,
	0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
	0x5A1F,0x5B1B,0x5C1F,0x5D1B,0x5E1B,0x5F1F,0x6027,0x6123,
	0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
	0x6A3F,0x6B3B,0x6C3F,0x6D3B,0x6E3B,0x6F3F,0x7023,0x7127,
	0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
	0x7A3B,0x7B3F,0x7C3B,0x7D3F,0x7E3F,0x7F3B,0x8083,0x8187,
	0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
	0x8A9B,0x8B9F,0x8C9B,0x8D9F,0x8E9F,0x8F9B,0x9087,0x9183,
	0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F,
	0x9A9F,0x9B9B,0x9C9F,0x9D9B,0x9E9B,0x9F9F,0xA0A7,0xA1A3,
	0xA2A3,0xA3A7,0xA4A3,0xA5A7,0xA6A7,0xA7A3,0xA8AB,0xA9AF,
	0xAABF,0xABBB,0xACBF,0xADBB,0xAEBB,0xAFBF,0xB0A3,0xB1A7,
	0xB2A7,0xB3A3,0xB4A7,0xB5A3,0xB6A3,0xB7A7,0xB8AF,0xB9AB,
	0xBABB,0xBBBF,0xBCBB,0xBDBF,0xBEBF,0xBFBB,0xC087,0xC183,
	0xC283,0xC387,0xC483,0xC587,0xC687,0xC783,0xC88B,0xC98F,
	0xCA9F,0xCB9B,0xCC9F,0xCD9B,0xCE9B,0xCF9F,0xD083,0xD187,
	0xD287,0xD383,0xD487,0xD583,0xD683,0xD787,0xD88F,0xD98B,
	0xDA9B,0xDB9F,0xDC9B,0xDD9F,0xDE9F,0xDF9B,0xE0A3,0xE1A7,
	0xE2A7,0xE3A3,0xE4A7,0xE5A3,0xE6A3,0xE7A7,0xE8AF,0xE9AB,
	0xEABB,0xEBBF,0xECBB,0xEDBF,0xEEBF,0xEFBB,0xF0A7,0xF1A3,
	0xF2A3,0xF3A7,0xF4A3,0xF5A7,0xF6A7,0xF7A3,0xF8AB,0xF9AF,
	0xFABF,0xFBBB,0xFCBF,0xFDBB,0xFEBB,0xFFBF,0x0047,0x0103,
	0x0203,0x0307,0x0403,0x0507,0x0607,0x0703,0x080B,0x090F,
	0x0A1F,0x0B1B,0x0C1F,0x0D1B,0x0E1B,0x0F1F,0x1003,0x1107,
	0x1207,0x1303,0x1407,0x1503,0x1603,0x1707,0x180F,0x190B,
	0x1A1B,0x1B1F,0x1C1B,0x1D1F,0x1E1F,0x1F1B,0x2023,0x2127,
	0x2227,0x2323,0x2427,0x2523,0x2623,0x2727,0x282F,0x292B,
	0x2A3B,0x2B3F,0x2C3B,0x2D3F,0x2E3F,0x2F3B,0x3027,0x3123,
	0x3223,0x3327,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
	0x3A3F,0x3B3B,0x3C3F,0x3D3B,0x3E3B,0x3F3F,0x4003,0x4107,
	0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
	0x4A1B,0x4B1F,0x4C1B,0x4D1F,0x4E1F,0x4F1B,0x5007,0x5103,
	0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
	0x5A1F,0x5B1B,0x5C1F,0x5D1B,0x5E1B,0x5F1F,0x6027,0x6123,
	0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
	0x6A3F,0x6B3B,0x6C3F,0x6D3B,0x6E3B,0x6F3F,0x7023,0x7127,
	0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
	0x7A3B,0x7B3F,0x7C3B,0x7D3F,0x7E3F,0x7F3B,0x8083,0x8187,
	0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
	0x8A9B,0x8B9F,0x8C9B,0x8D9F,0x8E9F,0x8F9B,0x9087,0x9183,
	0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F
};



inline UBYTE Z80::RES(UBYTE bit, UBYTE val)
{
	return val & ~(1 << bit);
}

inline UBYTE Z80::RLC(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x80) ? Cflag : 0;
	res = ((res << 1) | (res >> 7)) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::RL(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x80) ? Cflag : 0;
	res = ((res << 1) | (_F & Cflag)) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::RRC(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x01) ? Cflag : 0;
	res = ((res >> 1) | (res << 7)) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::RR(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x01) ? Cflag : 0;
	res = ((res >> 1) | (_F << 7)) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::SET(UBYTE bit, UBYTE val)
{
	return val | (1 << bit);
}

inline UBYTE Z80::SLA(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x80) ? Cflag : 0;
	res = (res << 1) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::SLL(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x80) ? Cflag : 0;
	res = ((res << 1) | 0x01) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::SRA(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x01) ? Cflag : 0;
	res = ((res >> 1) | (res & 0x80)) & 0xff;
	_F = SZP[res] | carry;
	return res;
}

inline UBYTE Z80::SRL(UBYTE val)
{
	unsigned res = val;
	unsigned carry = (res & 0x01) ? Cflag : 0;
	res = (res >> 1) & 0xff;
	_F = SZP[res] | carry;
	return res;
}



Z80::Z80()
{
	init();
}

void Z80::init()
{
	iCycleCount    = 0;
	iCycleCountSum = 0;
	iCycleCountInit= 0;
	iWSAdjust      = 0;
	dwMF2ExitAddr  = 0xffffffff;
	dwMF2Flags     = 0;
	mStop          = false;

	z80.init();

	init_tables();
}

void Z80::init_tables()
{
	int i, p;

	for (i = 0; i < 256; i++)
	{
		p = 0;
		if(i & 0x01) ++p;
		if(i & 0x02) ++p;
		if(i & 0x04) ++p;
		if(i & 0x08) ++p;
		if(i & 0x10) ++p;
		if(i & 0x20) ++p;
		if(i & 0x40) ++p;
		if(i & 0x80) ++p;

		SZ[i] = i ? i & Sflag : Zflag;
		SZ[i] |= (i & Xflags);
		SZ_BIT[i] = i ? i & Sflag : Zflag | Pflag;
		SZ_BIT[i] |= (i & Xflags);
		SZP[i] = SZ[i] | ((p & 1) ? 0 : Pflag);
		SZHV_inc[i] = SZ[i];
		if(i == 0x80) SZHV_inc[i] |= Vflag;
		if((i & 0x0f) == 0x00) SZHV_inc[i] |= Hflag;
		SZHV_dec[i] = SZ[i] | Nflag;
		if(i == 0x7f) SZHV_dec[i] |= Vflag;
		if((i & 0x0f) == 0x0f) SZHV_dec[i] |= Hflag;
	}
}

void Z80::mf2stop()
{
	_R++;
	_IFF1 = 0;
	z80.EI_issued = 0;
	iCycleCount   = 20;
	if (iWSAdjust) iCycleCount -= 4;
	dwMF2ExitAddr = _PCdword;
	RST(0x0066); // MF2 stop button causes a Z80 NMI
	z80_wait_states
	dwMF2Flags = MF2_ACTIVE | MF2_RUNNING;
}

int Z80::execute(int cc)
{
	register UBYTE bOpCode;

	iCycleCountInit = cc;
	iCycleCountSum  = 0;

	while (_PCdword != z80.break_point) // loop until break point
	{

		if (dwMF2Flags & MF2_RUNNING)
		{
			if (_PCdword == dwMF2ExitAddr) // have we returned from the MF2?
			{
				dwMF2Flags = MF2_INVISIBLE; // clear running flag and make the MF2 'invisible'
			}
		}

		bOpCode         = read_mem(_PC++);
		iCycleCount     = cc_op[bOpCode];
		_R++;
		switch(bOpCode)
		{
		 case adc_a:       ADC(_A); break;
		 case adc_b:       ADC(_B); break;
		 case adc_byte:    ADC(read_mem(_PC++)); break;
		 case adc_c:       ADC(_C); break;
		 case adc_d:       ADC(_D); break;
		 case adc_e:       ADC(_E); break;
		 case adc_h:       ADC(_H); break;
		 case adc_l:       ADC(_L); break;
		 case adc_mhl:     ADC(read_mem(_HL)); break;
		 case add_a:       ADD(_A); break;
		 case add_b:       ADD(_B); break;
		 case add_byte:    ADD(read_mem(_PC++)); break;
		 case add_c:       ADD(_C); break;
		 case add_d:       ADD(_D); break;
		 case add_e:       ADD(_E); break;
		 case add_h:       ADD(_H); break;
		 case add_hl_bc:   ADD16(HL, BC); break;
		 case add_hl_de:   ADD16(HL, DE); break;
		 case add_hl_hl:   ADD16(HL, HL); break;
		 case add_hl_sp:   ADD16(HL, SP); break;
		 case add_l:       ADD(_L); break;
		 case add_mhl:     ADD(read_mem(_HL)); break;
		 case and_a:       AND(_A); break;
		 case and_b:       AND(_B); break;
		 case and_byte:    AND(read_mem(_PC++)); break;
		 case and_c:       AND(_C); break;
		 case and_d:       AND(_D); break;
		 case and_e:       AND(_E); break;
		 case and_h:       AND(_H); break;
		 case and_l:       AND(_L); break;
		 case and_mhl:     AND(read_mem(_HL)); break;
		 case call:        CALL; break;
		 case call_c:      if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_m:      if (_F & Sflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_nc:     if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_nz:     if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_p:      if (!(_F & Sflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_pe:     if (_F & Pflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_po:     if (!(_F & Pflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case call_z:      if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
		 case ccf:         _F = ((_F & (Sflag | Zflag | Pflag | Cflag)) | ((_F & CF) << 4) | (_A & Xflags)) ^ CF; break;
		 case cpl:         _A ^= 0xff; _F = (_F & (Sflag | Zflag | Pflag | Cflag)) | Hflag | Nflag | (_A & Xflags); break;
		 case cp_a:        CP(_A); break;
		 case cp_b:        CP(_B); break;
		 case cp_byte:     CP(read_mem(_PC++)); break;
		 case cp_c:        CP(_C); break;
		 case cp_d:        CP(_D); break;
		 case cp_e:        CP(_E); break;
		 case cp_h:        CP(_H); break;
		 case cp_l:        CP(_L); break;
		 case cp_mhl:      CP(read_mem(_HL)); break;
		 case daa:         DAA; break;
		 case dec_a:       DEC(_A); break;
		 case dec_b:       DEC(_B); break;
		 case dec_bc:      _BC--; iWSAdjust++; break;
		 case dec_c:       DEC(_C); break;
		 case dec_d:       DEC(_D); break;
		 case dec_de:      _DE--; iWSAdjust++; break;
		 case dec_e:       DEC(_E); break;
		 case dec_h:       DEC(_H); break;
		 case dec_hl:      _HL--; iWSAdjust++; break;
		 case dec_l:       DEC(_L); break;
		 case dec_mhl:     { UBYTE b = read_mem(_HL); DEC(b); write_mem(_HL, b); } break;
		 case dec_sp:      _SP--; iWSAdjust++; break;
		 case di:          _IFF1 = _IFF2 = 0; z80.EI_issued = 0; break;
		 case djnz:        if (--_B) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; } break;
		 case ei:          z80.EI_issued = 2; break;
		 case exx:         EXX; break;
		 case ex_af_af:    EX(z80.AF, z80.AFx); break;
		 case ex_de_hl:    EX(z80.DE, z80.HL); break;
		 case ex_msp_hl:   EX_SP(HL); iWSAdjust++; break;
		 case halt:        _HALT = 1; _PC--; break;
		 case ina:         { REGPAIR p; p.b.l = read_mem(_PC++); p.b.h = _A; _A = z80_IN_handler(p); } break;
		 case inc_a:       INC(_A); break;
		 case inc_b:       INC(_B); break;
		 case inc_bc:      _BC++; iWSAdjust++; break;
		 case inc_c:       INC(_C); break;
		 case inc_d:       INC(_D); break;
		 case inc_de:      _DE++; iWSAdjust++; break;
		 case inc_e:       INC(_E); break;
		 case inc_h:       INC(_H); break;
		 case inc_hl:      _HL++; iWSAdjust++; break;
		 case inc_l:       INC(_L); break;
		 case inc_mhl:     { UBYTE b = read_mem(_HL); INC(b); write_mem(_HL, b); } break;
		 case inc_sp:      _SP++; iWSAdjust++; break;
		 case jp:          JP; break;
		 case jp_c:        if (_F & Cflag) { JP } else { _PC += 2; }; break;
		 case jp_m:        if (_F & Sflag) { JP } else { _PC += 2; }; break;
		 case jp_nc:       if (!(_F & Cflag)) { JP } else { _PC += 2; }; break;
		 case jp_nz:       if (!(_F & Zflag)) { JP } else { _PC += 2; }; break;
		 case jp_p:        if (!(_F & Sflag)) { JP } else { _PC += 2; }; break;
		 case jp_pe:       if (_F & Pflag) { JP } else { _PC += 2; }; break;
		 case jp_po:       if (!(_F & Pflag)) { JP } else { _PC += 2; }; break;
		 case jp_z:        if (_F & Zflag) { JP } else { _PC += 2; }; break;
		 case jr:          JR; break;
		 case jr_c:        if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
		 case jr_nc:       if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
		 case jr_nz:       if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
		 case jr_z:        if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
		 case ld_a_a:      break;
		 case ld_a_b:      _A = _B; break;
		 case ld_a_byte:   _A = read_mem(_PC++); break;
		 case ld_a_c:      _A = _C; break;
		 case ld_a_d:      _A = _D; break;
		 case ld_a_e:      _A = _E; break;
		 case ld_a_h:      _A = _H; break;
		 case ld_a_l:      _A = _L; break;
		 case ld_a_mbc:    _A = read_mem(_BC); break;
		 case ld_a_mde:    _A = read_mem(_DE); break;
		 case ld_a_mhl:    _A = read_mem(_HL); break;
		 case ld_a_mword:  { REGPAIR addr; addr.b.l = read_mem(_PC++); addr.b.h = read_mem(_PC++); _A = read_mem(addr.w.l); } break;
		 case ld_bc_word:  z80.BC.b.l = read_mem(_PC++); z80.BC.b.h = read_mem(_PC++); break;
		 case ld_b_a:      _B = _A; break;
		 case ld_b_b:      break;
		 case ld_b_byte:   _B = read_mem(_PC++); break;
		 case ld_b_c:      _B = _C; break;
		 case ld_b_d:      _B = _D; break;
		 case ld_b_e:      _B = _E; break;
		 case ld_b_h:      _B = _H; break;
		 case ld_b_l:      _B = _L; break;
		 case ld_b_mhl:    _B = read_mem(_HL); break;
		 case ld_c_a:      _C = _A; break;
		 case ld_c_b:      _C = _B; break;
		 case ld_c_byte:   _C = read_mem(_PC++); break;
		 case ld_c_c:      break;
		 case ld_c_d:      _C = _D; break;
		 case ld_c_e:      _C = _E; break;
		 case ld_c_h:      _C = _H; break;
		 case ld_c_l:      _C = _L; break;
		 case ld_c_mhl:    _C = read_mem(_HL); break;
		 case ld_de_word:  z80.DE.b.l = read_mem(_PC++); z80.DE.b.h = read_mem(_PC++); break;
		 case ld_d_a:      _D = _A; break;
		 case ld_d_b:      _D = _B; break;
		 case ld_d_byte:   _D = read_mem(_PC++); break;
		 case ld_d_c:      _D = _C; break;
		 case ld_d_d:      break;
		 case ld_d_e:      _D = _E; break;
		 case ld_d_h:      _D = _H; break;
		 case ld_d_l:      _D = _L; break;
		 case ld_d_mhl:    _D = read_mem(_HL); break;
		 case ld_e_a:      _E = _A; break;
		 case ld_e_b:      _E = _B; break;
		 case ld_e_byte:   _E = read_mem(_PC++); break;
		 case ld_e_c:      _E = _C; break;
		 case ld_e_d:      _E = _D; break;
		 case ld_e_e:      break;
		 case ld_e_h:      _E = _H; break;
		 case ld_e_l:      _E = _L; break;
		 case ld_e_mhl:    _E = read_mem(_HL); break;
		 case ld_hl_mword: LD16_MEM(HL); break;
		 case ld_hl_word:  z80.HL.b.l = read_mem(_PC++); z80.HL.b.h = read_mem(_PC++); break;
		 case ld_h_a:      _H = _A; break;
		 case ld_h_b:      _H = _B; break;
		 case ld_h_byte:   _H = read_mem(_PC++); break;
		 case ld_h_c:      _H = _C; break;
		 case ld_h_d:      _H = _D; break;
		 case ld_h_e:      _H = _E; break;
		 case ld_h_h:      break;
		 case ld_h_l:      _H = _L; break;
		 case ld_h_mhl:    _H = read_mem(_HL); break;
		 case ld_l_a:      _L = _A; break;
		 case ld_l_b:      _L = _B; break;
		 case ld_l_byte:   _L = read_mem(_PC++); break;
		 case ld_l_c:      _L = _C; break;
		 case ld_l_d:      _L = _D; break;
		 case ld_l_e:      _L = _E; break;
		 case ld_l_h:      _L = _H; break;
		 case ld_l_l:      break;
		 case ld_l_mhl:    _L = read_mem(_HL); break;
		 case ld_mbc_a:    write_mem(_BC, _A); break;
		 case ld_mde_a:    write_mem(_DE, _A); break;
		 case ld_mhl_a:    write_mem(_HL, _A); break;
		 case ld_mhl_b:    write_mem(_HL, _B); break;
		 case ld_mhl_byte: { UBYTE b = read_mem(_PC++); write_mem(_HL, b); } break;
		 case ld_mhl_c:    write_mem(_HL, _C); break;
		 case ld_mhl_d:    write_mem(_HL, _D); break;
		 case ld_mhl_e:    write_mem(_HL, _E); break;
		 case ld_mhl_h:    write_mem(_HL, _H); break;
		 case ld_mhl_l:    write_mem(_HL, _L); break;
		 case ld_mword_a:  { REGPAIR addr; addr.b.l = read_mem(_PC++); addr.b.h = read_mem(_PC++); write_mem(addr.w.l, _A); } break;
		 case ld_mword_hl: LDMEM_16(HL); break;
		 case ld_pc_hl:    _PC = _HL; break;
		 case ld_sp_hl:    _SP = _HL; iWSAdjust++; break;
		 case ld_sp_word:  z80.SP.b.l = read_mem(_PC++); z80.SP.b.h = read_mem(_PC++); break;
		 case nop:         break;
		 case or_a:        OR(_A); break;
		 case or_b:        OR(_B); break;
		 case or_byte:     OR(read_mem(_PC++)); break;
		 case or_c:        OR(_C); break;
		 case or_d:        OR(_D); break;
		 case or_e:        OR(_E); break;
		 case or_h:        OR(_H); break;
		 case or_l:        OR(_L); break;
		 case or_mhl:      OR(read_mem(_HL)); break;
		 case outa:        { REGPAIR p; p.b.l = read_mem(_PC++); p.b.h = _A; z80_OUT_handler(p, _A); } break;
		 case pfx_cb:      z80_pfx_cb(); break;
		 case pfx_dd:      z80_pfx_dd(); break;
		 case pfx_ed:      z80_pfx_ed(); break;
		 case pfx_fd:      z80_pfx_fd(); break;
		 case pop_af:      POP(AF); break;
		 case pop_bc:      POP(BC); break;
		 case pop_de:      POP(DE); break;
		 case pop_hl:      POP(HL); break;
		 case push_af:     PUSH(AF); break;
		 case push_bc:     PUSH(BC); break;
		 case push_de:     PUSH(DE); break;
		 case push_hl:     PUSH(HL); break;
		 case ret:         RET; break;
		 case ret_c:       if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_m:       if (_F & Sflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_nc:      if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_nz:      if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_p:       if (!(_F & Sflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_pe:      if (_F & Pflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_po:      if (!(_F & Pflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case ret_z:       if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		 case rla:         RLA; break;
		 case rlca:        RLCA; break;
		 case rra:         RRA; break;
		 case rrca:        RRCA; break;
		 case rst00:       RST(0x0000); break;
		 case rst08:       RST(0x0008); break;
		 case rst10:       RST(0x0010); break;
		 case rst18:       RST(0x0018); break;
		 case rst20:       RST(0x0020); break;
		 case rst28:       RST(0x0028); break;
		 case rst30:       RST(0x0030); break;
		 case rst38:       RST(0x0038); break;
		 case sbc_a:       SBC(_A); break;
		 case sbc_b:       SBC(_B); break;
		 case sbc_byte:    SBC(read_mem(_PC++)); break;
		 case sbc_c:       SBC(_C); break;
		 case sbc_d:       SBC(_D); break;
		 case sbc_e:       SBC(_E); break;
		 case sbc_h:       SBC(_H); break;
		 case sbc_l:       SBC(_L); break;
		 case sbc_mhl:     SBC(read_mem(_HL)); break;
		 case scf:         _F = (_F & (Sflag | Zflag | Pflag)) | Cflag | (_A & Xflags); break;
		 case sub_a:       SUB(_A); break;
		 case sub_b:       SUB(_B); break;
		 case sub_byte:    SUB(read_mem(_PC++)); break;
		 case sub_c:       SUB(_C); break;
		 case sub_d:       SUB(_D); break;
		 case sub_e:       SUB(_E); break;
		 case sub_h:       SUB(_H); break;
		 case sub_l:       SUB(_L); break;
		 case sub_mhl:     SUB(read_mem(_HL)); break;
		 case xor_a:       XOR(_A); break;
		 case xor_b:       XOR(_B); break;
		 case xor_byte:    XOR(read_mem(_PC++)); break;
		 case xor_c:       XOR(_C); break;
		 case xor_d:       XOR(_D); break;
		 case xor_e:       XOR(_E); break;
		 case xor_h:       XOR(_H); break;
		 case xor_l:       XOR(_L); break;
		 case xor_mhl:     XOR(read_mem(_HL)); break;
		}

		z80_wait_states


		if (z80.EI_issued) // EI 'delay' in effect?
		{
			if (--z80.EI_issued == 0)
			{
				_IFF1 = _IFF2 = Pflag; // set interrupt flip-flops
				if (z80.int_pending) z80_int_handler
			}
		}
		else if (z80.int_pending) z80_int_handler // any interrupts pending?

		iWSAdjust = 0;

		if (cpc.sound().bufferFull()) {return iCycleCountSum;} // @todo put this in STOP

		if (mStop)  // video emulation finished building frame?
		{
			mStop=false;
			return iCycleCountSum;   // exit emulation loop
		}
		else if (iCycleCountSum >= iCycleCountInit) return iCycleCountSum;



		/*
		if (z80.trace)                  // tracing instructions?
		{
			z80.trace = 0;              // reset trace condition
			return EC_TRACE;            // exit emulation loop
		}
		else if (vdu.frameCompleted())  // video emulation finished building frame?
		{
			return EC_FRAME_COMPLETE;   // exit emulation loop
		}
		else if (cpc.cycleCount() <= 0) // emulation loop ran for one frame?
		{
			return EC_CYCLE_COUNT;      // exit emulation loop
		}
		*/
	}
	return EC_BREAKPOINT;
}



void Z80::z80_pfx_cb()
{
	register UBYTE bOpCode;

	bOpCode = read_mem(_PC++);
	iCycleCount += cc_cb[bOpCode];
	_R++;
	switch(bOpCode)
	{
	  case bit0_a:      BIT(0, _A); break;
	  case bit0_b:      BIT(0, _B); break;
	  case bit0_c:      BIT(0, _C); break;
	  case bit0_d:      BIT(0, _D); break;
	  case bit0_e:      BIT(0, _E); break;
	  case bit0_h:      BIT(0, _H); break;
	  case bit0_l:      BIT(0, _L); break;
	  case bit0_mhl:    BIT(0, read_mem(_HL)); break;
	  case bit1_a:      BIT(1, _A); break;
	  case bit1_b:      BIT(1, _B); break;
	  case bit1_c:      BIT(1, _C); break;
	  case bit1_d:      BIT(1, _D); break;
	  case bit1_e:      BIT(1, _E); break;
	  case bit1_h:      BIT(1, _H); break;
	  case bit1_l:      BIT(1, _L); break;
	  case bit1_mhl:    BIT(1, read_mem(_HL)); break;
	  case bit2_a:      BIT(2, _A); break;
	  case bit2_b:      BIT(2, _B); break;
	  case bit2_c:      BIT(2, _C); break;
	  case bit2_d:      BIT(2, _D); break;
	  case bit2_e:      BIT(2, _E); break;
	  case bit2_h:      BIT(2, _H); break;
	  case bit2_l:      BIT(2, _L); break;
	  case bit2_mhl:    BIT(2, read_mem(_HL)); break;
	  case bit3_a:      BIT(3, _A); break;
	  case bit3_b:      BIT(3, _B); break;
	  case bit3_c:      BIT(3, _C); break;
	  case bit3_d:      BIT(3, _D); break;
	  case bit3_e:      BIT(3, _E); break;
	  case bit3_h:      BIT(3, _H); break;
	  case bit3_l:      BIT(3, _L); break;
	  case bit3_mhl:    BIT(3, read_mem(_HL)); break;
	  case bit4_a:      BIT(4, _A); break;
	  case bit4_b:      BIT(4, _B); break;
	  case bit4_c:      BIT(4, _C); break;
	  case bit4_d:      BIT(4, _D); break;
	  case bit4_e:      BIT(4, _E); break;
	  case bit4_h:      BIT(4, _H); break;
	  case bit4_l:      BIT(4, _L); break;
	  case bit4_mhl:    BIT(4, read_mem(_HL)); break;
	  case bit5_a:      BIT(5, _A); break;
	  case bit5_b:      BIT(5, _B); break;
	  case bit5_c:      BIT(5, _C); break;
	  case bit5_d:      BIT(5, _D); break;
	  case bit5_e:      BIT(5, _E); break;
	  case bit5_h:      BIT(5, _H); break;
	  case bit5_l:      BIT(5, _L); break;
	  case bit5_mhl:    BIT(5, read_mem(_HL)); break;
	  case bit6_a:      BIT(6, _A); break;
	  case bit6_b:      BIT(6, _B); break;
	  case bit6_c:      BIT(6, _C); break;
	  case bit6_d:      BIT(6, _D); break;
	  case bit6_e:      BIT(6, _E); break;
	  case bit6_h:      BIT(6, _H); break;
	  case bit6_l:      BIT(6, _L); break;
	  case bit6_mhl:    BIT(6, read_mem(_HL)); break;
	  case bit7_a:      BIT(7, _A); break;
	  case bit7_b:      BIT(7, _B); break;
	  case bit7_c:      BIT(7, _C); break;
	  case bit7_d:      BIT(7, _D); break;
	  case bit7_e:      BIT(7, _E); break;
	  case bit7_h:      BIT(7, _H); break;
	  case bit7_l:      BIT(7, _L); break;
	  case bit7_mhl:    BIT(7, read_mem(_HL)); break;
	  case res0_a:      _A = RES(0, _A); break;
	  case res0_b:      _B = RES(0, _B); break;
	  case res0_c:      _C = RES(0, _C); break;
	  case res0_d:      _D = RES(0, _D); break;
	  case res0_e:      _E = RES(0, _E); break;
	  case res0_h:      _H = RES(0, _H); break;
	  case res0_l:      _L = RES(0, _L); break;
	  case res0_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(0, b)); } break;
	  case res1_a:      _A = RES(1, _A); break;
	  case res1_b:      _B = RES(1, _B); break;
	  case res1_c:      _C = RES(1, _C); break;
	  case res1_d:      _D = RES(1, _D); break;
	  case res1_e:      _E = RES(1, _E); break;
	  case res1_h:      _H = RES(1, _H); break;
	  case res1_l:      _L = RES(1, _L); break;
	  case res1_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(1, b)); } break;
	  case res2_a:      _A = RES(2, _A); break;
	  case res2_b:      _B = RES(2, _B); break;
	  case res2_c:      _C = RES(2, _C); break;
	  case res2_d:      _D = RES(2, _D); break;
	  case res2_e:      _E = RES(2, _E); break;
	  case res2_h:      _H = RES(2, _H); break;
	  case res2_l:      _L = RES(2, _L); break;
	  case res2_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(2, b)); } break;
	  case res3_a:      _A = RES(3, _A); break;
	  case res3_b:      _B = RES(3, _B); break;
	  case res3_c:      _C = RES(3, _C); break;
	  case res3_d:      _D = RES(3, _D); break;
	  case res3_e:      _E = RES(3, _E); break;
	  case res3_h:      _H = RES(3, _H); break;
	  case res3_l:      _L = RES(3, _L); break;
	  case res3_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(3, b)); } break;
	  case res4_a:      _A = RES(4, _A); break;
	  case res4_b:      _B = RES(4, _B); break;
	  case res4_c:      _C = RES(4, _C); break;
	  case res4_d:      _D = RES(4, _D); break;
	  case res4_e:      _E = RES(4, _E); break;
	  case res4_h:      _H = RES(4, _H); break;
	  case res4_l:      _L = RES(4, _L); break;
	  case res4_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(4, b)); } break;
	  case res5_a:      _A = RES(5, _A); break;
	  case res5_b:      _B = RES(5, _B); break;
	  case res5_c:      _C = RES(5, _C); break;
	  case res5_d:      _D = RES(5, _D); break;
	  case res5_e:      _E = RES(5, _E); break;
	  case res5_h:      _H = RES(5, _H); break;
	  case res5_l:      _L = RES(5, _L); break;
	  case res5_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(5, b)); } break;
	  case res6_a:      _A = RES(6, _A); break;
	  case res6_b:      _B = RES(6, _B); break;
	  case res6_c:      _C = RES(6, _C); break;
	  case res6_d:      _D = RES(6, _D); break;
	  case res6_e:      _E = RES(6, _E); break;
	  case res6_h:      _H = RES(6, _H); break;
	  case res6_l:      _L = RES(6, _L); break;
	  case res6_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(6, b)); } break;
	  case res7_a:      _A = RES(7, _A); break;
	  case res7_b:      _B = RES(7, _B); break;
	  case res7_c:      _C = RES(7, _C); break;
	  case res7_d:      _D = RES(7, _D); break;
	  case res7_e:      _E = RES(7, _E); break;
	  case res7_h:      _H = RES(7, _H); break;
	  case res7_l:      _L = RES(7, _L); break;
	  case res7_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, RES(7, b)); } break;
	  case rlc_a:       _A = RLC(_A); break;
	  case rlc_b:       _B = RLC(_B); break;
	  case rlc_c:       _C = RLC(_C); break;
	  case rlc_d:       _D = RLC(_D); break;
	  case rlc_e:       _E = RLC(_E); break;
	  case rlc_h:       _H = RLC(_H); break;
	  case rlc_l:       _L = RLC(_L); break;
	  case rlc_mhl:     { UBYTE b = read_mem(_HL); write_mem(_HL, RLC(b)); } break;
	  case rl_a:        _A = RL(_A); break;
	  case rl_b:        _B = RL(_B); break;
	  case rl_c:        _C = RL(_C); break;
	  case rl_d:        _D = RL(_D); break;
	  case rl_e:        _E = RL(_E); break;
	  case rl_h:        _H = RL(_H); break;
	  case rl_l:        _L = RL(_L); break;
	  case rl_mhl:      { UBYTE b = read_mem(_HL); write_mem(_HL, RL(b)); } break;
	  case rrc_a:       _A = RRC(_A); break;
	  case rrc_b:       _B = RRC(_B); break;
	  case rrc_c:       _C = RRC(_C); break;
	  case rrc_d:       _D = RRC(_D); break;
	  case rrc_e:       _E = RRC(_E); break;
	  case rrc_h:       _H = RRC(_H); break;
	  case rrc_l:       _L = RRC(_L); break;
	  case rrc_mhl:     { UBYTE b = read_mem(_HL); write_mem(_HL, RRC(b)); } break;
	  case rr_a:        _A = RR(_A); break;
	  case rr_b:        _B = RR(_B); break;
	  case rr_c:        _C = RR(_C); break;
	  case rr_d:        _D = RR(_D); break;
	  case rr_e:        _E = RR(_E); break;
	  case rr_h:        _H = RR(_H); break;
	  case rr_l:        _L = RR(_L); break;
	  case rr_mhl:      { UBYTE b = read_mem(_HL); write_mem(_HL, RR(b)); } break;
	  case set0_a:      _A = SET(0, _A); break;
	  case set0_b:      _B = SET(0, _B); break;
	  case set0_c:      _C = SET(0, _C); break;
	  case set0_d:      _D = SET(0, _D); break;
	  case set0_e:      _E = SET(0, _E); break;
	  case set0_h:      _H = SET(0, _H); break;
	  case set0_l:      _L = SET(0, _L); break;
	  case set0_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(0, b)); } break;
	  case set1_a:      _A = SET(1, _A); break;
	  case set1_b:      _B = SET(1, _B); break;
	  case set1_c:      _C = SET(1, _C); break;
	  case set1_d:      _D = SET(1, _D); break;
	  case set1_e:      _E = SET(1, _E); break;
	  case set1_h:      _H = SET(1, _H); break;
	  case set1_l:      _L = SET(1, _L); break;
	  case set1_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(1, b)); } break;
	  case set2_a:      _A = SET(2, _A); break;
	  case set2_b:      _B = SET(2, _B); break;
	  case set2_c:      _C = SET(2, _C); break;
	  case set2_d:      _D = SET(2, _D); break;
	  case set2_e:      _E = SET(2, _E); break;
	  case set2_h:      _H = SET(2, _H); break;
	  case set2_l:      _L = SET(2, _L); break;
	  case set2_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(2, b)); } break;
	  case set3_a:      _A = SET(3, _A); break;
	  case set3_b:      _B = SET(3, _B); break;
	  case set3_c:      _C = SET(3, _C); break;
	  case set3_d:      _D = SET(3, _D); break;
	  case set3_e:      _E = SET(3, _E); break;
	  case set3_h:      _H = SET(3, _H); break;
	  case set3_l:      _L = SET(3, _L); break;
	  case set3_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(3, b)); } break;
	  case set4_a:      _A = SET(4, _A); break;
	  case set4_b:      _B = SET(4, _B); break;
	  case set4_c:      _C = SET(4, _C); break;
	  case set4_d:      _D = SET(4, _D); break;
	  case set4_e:      _E = SET(4, _E); break;
	  case set4_h:      _H = SET(4, _H); break;
	  case set4_l:      _L = SET(4, _L); break;
	  case set4_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(4, b)); } break;
	  case set5_a:      _A = SET(5, _A); break;
	  case set5_b:      _B = SET(5, _B); break;
	  case set5_c:      _C = SET(5, _C); break;
	  case set5_d:      _D = SET(5, _D); break;
	  case set5_e:      _E = SET(5, _E); break;
	  case set5_h:      _H = SET(5, _H); break;
	  case set5_l:      _L = SET(5, _L); break;
	  case set5_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(5, b)); } break;
	  case set6_a:      _A = SET(6, _A); break;
	  case set6_b:      _B = SET(6, _B); break;
	  case set6_c:      _C = SET(6, _C); break;
	  case set6_d:      _D = SET(6, _D); break;
	  case set6_e:      _E = SET(6, _E); break;
	  case set6_h:      _H = SET(6, _H); break;
	  case set6_l:      _L = SET(6, _L); break;
	  case set6_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(6, b)); } break;
	  case set7_a:      _A = SET(7, _A); break;
	  case set7_b:      _B = SET(7, _B); break;
	  case set7_c:      _C = SET(7, _C); break;
	  case set7_d:      _D = SET(7, _D); break;
	  case set7_e:      _E = SET(7, _E); break;
	  case set7_h:      _H = SET(7, _H); break;
	  case set7_l:      _L = SET(7, _L); break;
	  case set7_mhl:    { UBYTE b = read_mem(_HL); write_mem(_HL, SET(7, b)); } break;
	  case sla_a:       _A = SLA(_A); break;
	  case sla_b:       _B = SLA(_B); break;
	  case sla_c:       _C = SLA(_C); break;
	  case sla_d:       _D = SLA(_D); break;
	  case sla_e:       _E = SLA(_E); break;
	  case sla_h:       _H = SLA(_H); break;
	  case sla_l:       _L = SLA(_L); break;
	  case sla_mhl:     { UBYTE b = read_mem(_HL); write_mem(_HL, SLA(b)); } break;
	  case sll_a:       _A = SLL(_A); break;
	  case sll_b:       _B = SLL(_B); break;
	  case sll_c:       _C = SLL(_C); break;
	  case sll_d:       _D = SLL(_D); break;
	  case sll_e:       _E = SLL(_E); break;
	  case sll_h:       _H = SLL(_H); break;
	  case sll_l:       _L = SLL(_L); break;
	  case sll_mhl:     { UBYTE b = read_mem(_HL); write_mem(_HL, SLL(b)); } break;
	  case sra_a:       _A = SRA(_A); break;
	  case sra_b:       _B = SRA(_B); break;
	  case sra_c:       _C = SRA(_C); break;
	  case sra_d:       _D = SRA(_D); break;
	  case sra_e:       _E = SRA(_E); break;
	  case sra_h:       _H = SRA(_H); break;
	  case sra_l:       _L = SRA(_L); break;
	  case sra_mhl:     { UBYTE b = read_mem(_HL); write_mem(_HL, SRA(b)); } break;
	  case srl_a:       _A = SRL(_A); break;
	  case srl_b:       _B = SRL(_B); break;
	  case srl_c:       _C = SRL(_C); break;
	  case srl_d:       _D = SRL(_D); break;
	  case srl_e:       _E = SRL(_E); break;
	  case srl_h:       _H = SRL(_H); break;
	  case srl_l:       _L = SRL(_L); break;
	  case srl_mhl:     { UBYTE b = read_mem(_HL); write_mem(_HL, SRL(b)); } break;
	}
}



void Z80::z80_pfx_dd()
{
	register UBYTE bOpCode;

	bOpCode = read_mem(_PC++);
	iCycleCount += cc_xy[bOpCode];
	_R++;
	switch(bOpCode)
	{
	  case adc_a:       ADC(_A); break;
	  case adc_b:       ADC(_B); break;
	  case adc_byte:    ADC(read_mem(_PC++)); break;
	  case adc_c:       ADC(_C); break;
	  case adc_d:       ADC(_D); break;
	  case adc_e:       ADC(_E); break;
	  case adc_h:       ADC(_IXh); break;
	  case adc_l:       ADC(_IXl); break;
	  case adc_mhl:     { signed char o = read_mem(_PC++); ADC(read_mem(_IX+o)); } break;
	  case add_a:       ADD(_A); break;
	  case add_b:       ADD(_B); break;
	  case add_byte:    ADD(read_mem(_PC++)); break;
	  case add_c:       ADD(_C); break;
	  case add_d:       ADD(_D); break;
	  case add_e:       ADD(_E); break;
	  case add_h:       ADD(_IXh); break;
	  case add_hl_bc:   ADD16(IX, BC); break;
	  case add_hl_de:   ADD16(IX, DE); break;
	  case add_hl_hl:   ADD16(IX, IX); break;
	  case add_hl_sp:   ADD16(IX, SP); break;
	  case add_l:       ADD(_IXl); break;
	  case add_mhl:     { signed char o = read_mem(_PC++); ADD(read_mem(_IX+o)); } break;
	  case and_a:       AND(_A); break;
	  case and_b:       AND(_B); break;
	  case and_byte:    AND(read_mem(_PC++)); break;
	  case and_c:       AND(_C); break;
	  case and_d:       AND(_D); break;
	  case and_e:       AND(_E); break;
	  case and_h:       AND(_IXh); break;
	  case and_l:       AND(_IXl); break;
	  case and_mhl:     { signed char o = read_mem(_PC++); AND(read_mem(_IX+o)); } break;
	  case call:        CALL; break;
	  case call_c:      if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_m:      if (_F & Sflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_nc:     if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_nz:     if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_p:      if (!(_F & Sflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_pe:     if (_F & Pflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_po:     if (!(_F & Pflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_z:      if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case ccf:         _F = ((_F & (Sflag | Zflag | Pflag | Cflag)) | ((_F & CF) << 4) | (_A & Xflags)) ^ CF; break;
	  case cpl:         _A ^= 0xff; _F = (_F & (Sflag | Zflag | Pflag | Cflag)) | Hflag | Nflag | (_A & Xflags); break;
	  case cp_a:        CP(_A); break;
	  case cp_b:        CP(_B); break;
	  case cp_byte:     CP(read_mem(_PC++)); break;
	  case cp_c:        CP(_C); break;
	  case cp_d:        CP(_D); break;
	  case cp_e:        CP(_E); break;
	  case cp_h:        CP(_IXh); break;
	  case cp_l:        CP(_IXl); break;
	  case cp_mhl:      { signed char o = read_mem(_PC++); CP(read_mem(_IX+o)); } break;
	  case daa:         DAA; break;
	  case dec_a:       DEC(_A); break;
	  case dec_b:       DEC(_B); break;
	  case dec_bc:      _BC--; iWSAdjust++; break;
	  case dec_c:       DEC(_C); break;
	  case dec_d:       DEC(_D); break;
	  case dec_de:      _DE--; iWSAdjust++; break;
	  case dec_e:       DEC(_E); break;
	  case dec_h:       DEC(_IXh); break;
	  case dec_hl:      _IX--; iWSAdjust++; break;
	  case dec_l:       DEC(_IXl); break;
	  case dec_mhl:     { signed char o = read_mem(_PC++); UBYTE b = read_mem(_IX+o); DEC(b); write_mem(_IX+o, b); } break;
	  case dec_sp:      _SP--; iWSAdjust++; break;
	  case di:          _IFF1 = _IFF2 = 0; z80.EI_issued = 0; break;
	  case djnz:        if (--_B) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; } break;
	  case ei:          z80.EI_issued = 2; break;
	  case exx:         EXX; break;
	  case ex_af_af:    EX(z80.AF, z80.AFx); break;
	  case ex_de_hl:    EX(z80.DE, z80.HL); break;
	  case ex_msp_hl:   EX_SP(IX); iWSAdjust++; break;
	  case halt:        _HALT = 1; _PC--; break;
	  case ina:         { REGPAIR p; p.b.l = read_mem(_PC++); p.b.h = _A; _A = z80_IN_handler(p); } break;
	  case inc_a:       INC(_A); break;
	  case inc_b:       INC(_B); break;
	  case inc_bc:      _BC++; iWSAdjust++; break;
	  case inc_c:       INC(_C); break;
	  case inc_d:       INC(_D); break;
	  case inc_de:      _DE++; iWSAdjust++; break;
	  case inc_e:       INC(_E); break;
	  case inc_h:       INC(_IXh); break;
	  case inc_hl:      _IX++; iWSAdjust++; break;
	  case inc_l:       INC(_IXl); break;
	  case inc_mhl:     { signed char o = read_mem(_PC++); UBYTE b = read_mem(_IX+o); INC(b); write_mem(_IX+o, b); } break;
	  case inc_sp:      _SP++; iWSAdjust++; break;
	  case jp:          JP; break;
	  case jp_c:        if (_F & Cflag) { JP } else { _PC += 2; }; break;
	  case jp_m:        if (_F & Sflag) { JP } else { _PC += 2; }; break;
	  case jp_nc:       if (!(_F & Cflag)) { JP } else { _PC += 2; }; break;
	  case jp_nz:       if (!(_F & Zflag)) { JP } else { _PC += 2; }; break;
	  case jp_p:        if (!(_F & Sflag)) { JP } else { _PC += 2; }; break;
	  case jp_pe:       if (_F & Pflag) { JP } else { _PC += 2; }; break;
	  case jp_po:       if (!(_F & Pflag)) { JP } else { _PC += 2; }; break;
	  case jp_z:        if (_F & Zflag) { JP } else { _PC += 2; }; break;
	  case jr:          JR; break;
	  case jr_c:        if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case jr_nc:       if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case jr_nz:       if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case jr_z:        if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case ld_a_a:      break;
	  case ld_a_b:      _A = _B; break;
	  case ld_a_byte:   _A = read_mem(_PC++); break;
	  case ld_a_c:      _A = _C; break;
	  case ld_a_d:      _A = _D; break;
	  case ld_a_e:      _A = _E; break;
	  case ld_a_h:      _A = _IXh; break;
	  case ld_a_l:      _A = _IXl; break;
	  case ld_a_mbc:    _A = read_mem(_BC); break;
	  case ld_a_mde:    _A = read_mem(_DE); break;
	  case ld_a_mhl:    { signed char o = read_mem(_PC++); _A = read_mem(_IX+o); } break;
	  case ld_a_mword:  { REGPAIR addr; addr.b.l = read_mem(_PC++); addr.b.h = read_mem(_PC++); _A = read_mem(addr.w.l); } break;
	  case ld_bc_word:  z80.BC.b.l = read_mem(_PC++); z80.BC.b.h = read_mem(_PC++); break;
	  case ld_b_a:      _B = _A; break;
	  case ld_b_b:      break;
	  case ld_b_byte:   _B = read_mem(_PC++); break;
	  case ld_b_c:      _B = _C; break;
	  case ld_b_d:      _B = _D; break;
	  case ld_b_e:      _B = _E; break;
	  case ld_b_h:      _B = _IXh; break;
	  case ld_b_l:      _B = _IXl; break;
	  case ld_b_mhl:    { signed char o = read_mem(_PC++); _B = read_mem(_IX+o); } break;
	  case ld_c_a:      _C = _A; break;
	  case ld_c_b:      _C = _B; break;
	  case ld_c_byte:   _C = read_mem(_PC++); break;
	  case ld_c_c:      break;
	  case ld_c_d:      _C = _D; break;
	  case ld_c_e:      _C = _E; break;
	  case ld_c_h:      _C = _IXh; break;
	  case ld_c_l:      _C = _IXl; break;
	  case ld_c_mhl:    { signed char o = read_mem(_PC++); _C = read_mem(_IX+o); } break;
	  case ld_de_word:  z80.DE.b.l = read_mem(_PC++); z80.DE.b.h = read_mem(_PC++); break;
	  case ld_d_a:      _D = _A; break;
	  case ld_d_b:      _D = _B; break;
	  case ld_d_byte:   _D = read_mem(_PC++); break;
	  case ld_d_c:      _D = _C; break;
	  case ld_d_d:      break;
	  case ld_d_e:      _D = _E; break;
	  case ld_d_h:      _D = _IXh; break;
	  case ld_d_l:      _D = _IXl; break;
	  case ld_d_mhl:    { signed char o = read_mem(_PC++); _D = read_mem(_IX+o); } break;
	  case ld_e_a:      _E = _A; break;
	  case ld_e_b:      _E = _B; break;
	  case ld_e_byte:   _E = read_mem(_PC++); break;
	  case ld_e_c:      _E = _C; break;
	  case ld_e_d:      _E = _D; break;
	  case ld_e_e:      break;
	  case ld_e_h:      _E = _IXh; break;
	  case ld_e_l:      _E = _IXl; break;
	  case ld_e_mhl:    { signed char o = read_mem(_PC++); _E = read_mem(_IX+o); } break;
	  case ld_hl_mword: LD16_MEM(IX); break;
	  case ld_hl_word:  z80.IX.b.l = read_mem(_PC++); z80.IX.b.h = read_mem(_PC++); break;
	  case ld_h_a:      _IXh = _A; break;
	  case ld_h_b:      _IXh = _B; break;
	  case ld_h_byte:   _IXh = read_mem(_PC++); break;
	  case ld_h_c:      _IXh = _C; break;
	  case ld_h_d:      _IXh = _D; break;
	  case ld_h_e:      _IXh = _E; break;
	  case ld_h_h:      break;
	  case ld_h_l:      _IXh = _IXl; break;
	  case ld_h_mhl:    { signed char o = read_mem(_PC++); _H = read_mem(_IX+o); } break;
	  case ld_l_a:      _IXl = _A; break;
	  case ld_l_b:      _IXl = _B; break;
	  case ld_l_byte:   _IXl = read_mem(_PC++); break;
	  case ld_l_c:      _IXl = _C; break;
	  case ld_l_d:      _IXl = _D; break;
	  case ld_l_e:      _IXl = _E; break;
	  case ld_l_h:      _IXl = _IXh; break;
	  case ld_l_l:      break;
	  case ld_l_mhl:    { signed char o = read_mem(_PC++); _L = read_mem(_IX+o); } break;
	  case ld_mbc_a:    write_mem(_BC, _A); break;
	  case ld_mde_a:    write_mem(_DE, _A); break;
	  case ld_mhl_a:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _A); } break;
	  case ld_mhl_b:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _B); } break;
	  case ld_mhl_byte: { signed char o = read_mem(_PC++); UBYTE b = read_mem(_PC++); write_mem(_IX+o, b); } break;
	  case ld_mhl_c:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _C); } break;
	  case ld_mhl_d:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _D); } break;
	  case ld_mhl_e:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _E); } break;
	  case ld_mhl_h:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _H); } break;
	  case ld_mhl_l:    { signed char o = read_mem(_PC++); write_mem(_IX+o, _L); } break;
	  case ld_mword_a:  { REGPAIR addr; addr.b.l = read_mem(_PC++); addr.b.h = read_mem(_PC++); write_mem(addr.w.l, _A); } break;
	  case ld_mword_hl: LDMEM_16(IX); break;
	  case ld_pc_hl:    _PC = _IX; break;
	  case ld_sp_hl:    _SP = _IX; iWSAdjust++; break;
	  case ld_sp_word:  z80.SP.b.l = read_mem(_PC++); z80.SP.b.h = read_mem(_PC++); break;
	  case nop:         break;
	  case or_a:        OR(_A); break;
	  case or_b:        OR(_B); break;
	  case or_byte:     OR(read_mem(_PC++)); break;
	  case or_c:        OR(_C); break;
	  case or_d:        OR(_D); break;
	  case or_e:        OR(_E); break;
	  case or_h:        OR(_IXh); break;
	  case or_l:        OR(_IXl); break;
	  case or_mhl:      { signed char o = read_mem(_PC++); OR(read_mem(_IX+o)); } break;
	  case outa:        { REGPAIR p; p.b.l = read_mem(_PC++); p.b.h = _A; z80_OUT_handler(p, _A); } break;
	  case pfx_cb:      z80_pfx_ddcb(); break;
	  case pfx_dd:      z80_pfx_dd(); break;
	  case pfx_ed:      z80_pfx_ed(); break;
	  case pfx_fd:      z80_pfx_fd(); break;
	  case pop_af:      POP(AF); break;
	  case pop_bc:      POP(BC); break;
	  case pop_de:      POP(DE); break;
	  case pop_hl:      POP(IX); break;
	  case push_af:     PUSH(AF); break;
	  case push_bc:     PUSH(BC); break;
	  case push_de:     PUSH(DE); break;
	  case push_hl:     PUSH(IX); break;
	  case ret:         RET; break;
	  case ret_c:       if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_m:       if (_F & Sflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_nc:      if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_nz:      if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_p:       if (!(_F & Sflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_pe:      if (_F & Pflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_po:      if (!(_F & Pflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_z:       if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case rla:         RLA; break;
	  case rlca:        RLCA; break;
	  case rra:         RRA; break;
	  case rrca:        RRCA; break;
	  case rst00:       RST(0x0000); break;
	  case rst08:       RST(0x0008); break;
	  case rst10:       RST(0x0010); break;
	  case rst18:       RST(0x0018); break;
	  case rst20:       RST(0x0020); break;
	  case rst28:       RST(0x0028); break;
	  case rst30:       RST(0x0030); break;
	  case rst38:       RST(0x0038); break;
	  case sbc_a:       SBC(_A); break;
	  case sbc_b:       SBC(_B); break;
	  case sbc_byte:    SBC(read_mem(_PC++)); break;
	  case sbc_c:       SBC(_C); break;
	  case sbc_d:       SBC(_D); break;
	  case sbc_e:       SBC(_E); break;
	  case sbc_h:       SBC(_IXh); break;
	  case sbc_l:       SBC(_IXl); break;
	  case sbc_mhl:     { signed char o = read_mem(_PC++); SBC(read_mem(_IX+o)); } break;
	  case scf:         _F = (_F & (Sflag | Zflag | Pflag)) | Cflag | (_A & Xflags); break;
	  case sub_a:       SUB(_A); break;
	  case sub_b:       SUB(_B); break;
	  case sub_byte:    SUB(read_mem(_PC++)); break;
	  case sub_c:       SUB(_C); break;
	  case sub_d:       SUB(_D); break;
	  case sub_e:       SUB(_E); break;
	  case sub_h:       SUB(_IXh); break;
	  case sub_l:       SUB(_IXl); break;
	  case sub_mhl:     { signed char o = read_mem(_PC++); SUB(read_mem(_IX+o)); } break;
	  case xor_a:       XOR(_A); break;
	  case xor_b:       XOR(_B); break;
	  case xor_byte:    XOR(read_mem(_PC++)); break;
	  case xor_c:       XOR(_C); break;
	  case xor_d:       XOR(_D); break;
	  case xor_e:       XOR(_E); break;
	  case xor_h:       XOR(_IXh); break;
	  case xor_l:       XOR(_IXl); break;
	  case xor_mhl:     { signed char o = read_mem(_PC++); XOR(read_mem(_IX+o)); } break;
	}
}

void Z80::z80_pfx_ddcb()
{
	signed char o;
	register UBYTE bOpCode;

	o = read_mem(_PC++); // offset
	bOpCode = read_mem(_PC++);
	iCycleCount += cc_xycb[bOpCode];
	switch(bOpCode)
	{
	  case bit0_a:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_b:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_c:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_d:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_e:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_h:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_l:      BIT_XY(0, read_mem(_IX+o)); break;
	  case bit0_mhl:    BIT_XY(0, read_mem(_IX+o)); break;
	  case bit1_a:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_b:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_c:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_d:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_e:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_h:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_l:      BIT_XY(1, read_mem(_IX+o)); break;
	  case bit1_mhl:    BIT_XY(1, read_mem(_IX+o)); break;
	  case bit2_a:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_b:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_c:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_d:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_e:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_h:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_l:      BIT_XY(2, read_mem(_IX+o)); break;
	  case bit2_mhl:    BIT_XY(2, read_mem(_IX+o)); break;
	  case bit3_a:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_b:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_c:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_d:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_e:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_h:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_l:      BIT_XY(3, read_mem(_IX+o)); break;
	  case bit3_mhl:    BIT_XY(3, read_mem(_IX+o)); break;
	  case bit4_a:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_b:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_c:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_d:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_e:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_h:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_l:      BIT_XY(4, read_mem(_IX+o)); break;
	  case bit4_mhl:    BIT_XY(4, read_mem(_IX+o)); break;
	  case bit5_a:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_b:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_c:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_d:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_e:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_h:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_l:      BIT_XY(5, read_mem(_IX+o)); break;
	  case bit5_mhl:    BIT_XY(5, read_mem(_IX+o)); break;
	  case bit6_a:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_b:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_c:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_d:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_e:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_h:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_l:      BIT_XY(6, read_mem(_IX+o)); break;
	  case bit6_mhl:    BIT_XY(6, read_mem(_IX+o)); break;
	  case bit7_a:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_b:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_c:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_d:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_e:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_h:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_l:      BIT_XY(7, read_mem(_IX+o)); break;
	  case bit7_mhl:    BIT_XY(7, read_mem(_IX+o)); break;
	  case res0_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(0, _A)); break;
	  case res0_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(0, _B)); break;
	  case res0_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(0, _C)); break;
	  case res0_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(0, _D)); break;
	  case res0_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(0, _E)); break;
	  case res0_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(0, _H)); break;
	  case res0_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(0, _L)); break;
	  case res0_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(0, b)); } break;
	  case res1_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(1, _A)); break;
	  case res1_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(1, _B)); break;
	  case res1_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(1, _C)); break;
	  case res1_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(1, _D)); break;
	  case res1_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(1, _E)); break;
	  case res1_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(1, _H)); break;
	  case res1_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(1, _L)); break;
	  case res1_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(1, b)); } break;
	  case res2_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(2, _A)); break;
	  case res2_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(2, _B)); break;
	  case res2_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(2, _C)); break;
	  case res2_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(2, _D)); break;
	  case res2_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(2, _E)); break;
	  case res2_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(2, _H)); break;
	  case res2_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(2, _L)); break;
	  case res2_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(2, b)); } break;
	  case res3_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(3, _A)); break;
	  case res3_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(3, _B)); break;
	  case res3_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(3, _C)); break;
	  case res3_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(3, _D)); break;
	  case res3_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(3, _E)); break;
	  case res3_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(3, _H)); break;
	  case res3_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(3, _L)); break;
	  case res3_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(3, b)); } break;
	  case res4_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(4, _A)); break;
	  case res4_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(4, _B)); break;
	  case res4_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(4, _C)); break;
	  case res4_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(4, _D)); break;
	  case res4_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(4, _E)); break;
	  case res4_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(4, _H)); break;
	  case res4_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(4, _L)); break;
	  case res4_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(4, b)); } break;
	  case res5_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(5, _A)); break;
	  case res5_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(5, _B)); break;
	  case res5_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(5, _C)); break;
	  case res5_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(5, _D)); break;
	  case res5_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(5, _E)); break;
	  case res5_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(5, _H)); break;
	  case res5_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(5, _L)); break;
	  case res5_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(5, b)); } break;
	  case res6_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(6, _A)); break;
	  case res6_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(6, _B)); break;
	  case res6_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(6, _C)); break;
	  case res6_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(6, _D)); break;
	  case res6_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(6, _E)); break;
	  case res6_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(6, _H)); break;
	  case res6_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(6, _L)); break;
	  case res6_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(6, b)); } break;
	  case res7_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = RES(7, _A)); break;
	  case res7_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = RES(7, _B)); break;
	  case res7_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = RES(7, _C)); break;
	  case res7_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = RES(7, _D)); break;
	  case res7_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = RES(7, _E)); break;
	  case res7_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = RES(7, _H)); break;
	  case res7_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = RES(7, _L)); break;
	  case res7_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RES(7, b)); } break;
	  case rlc_a:       _A = read_mem(_IX+o); _A = RLC(_A); write_mem(_IX+o, _A); break;
	  case rlc_b:       _B = read_mem(_IX+o); _B = RLC(_B); write_mem(_IX+o, _B); break;
	  case rlc_c:       _C = read_mem(_IX+o); _C = RLC(_C); write_mem(_IX+o, _C); break;
	  case rlc_d:       _D = read_mem(_IX+o); _D = RLC(_D); write_mem(_IX+o, _D); break;
	  case rlc_e:       _E = read_mem(_IX+o); _E = RLC(_E); write_mem(_IX+o, _E); break;
	  case rlc_h:       _H = read_mem(_IX+o); _H = RLC(_H); write_mem(_IX+o, _H); break;
	  case rlc_l:       _L = read_mem(_IX+o); _L = RLC(_L); write_mem(_IX+o, _L); break;
	  case rlc_mhl:     { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RLC(b)); } break;
	  case rl_a:        _A = read_mem(_IX+o); _A = RL(_A); write_mem(_IX+o, _A); break;
	  case rl_b:        _B = read_mem(_IX+o); _B = RL(_B); write_mem(_IX+o, _B); break;
	  case rl_c:        _C = read_mem(_IX+o); _C = RL(_C); write_mem(_IX+o, _C); break;
	  case rl_d:        _D = read_mem(_IX+o); _D = RL(_D); write_mem(_IX+o, _D); break;
	  case rl_e:        _E = read_mem(_IX+o); _E = RL(_E); write_mem(_IX+o, _E); break;
	  case rl_h:        _H = read_mem(_IX+o); _H = RL(_H); write_mem(_IX+o, _H); break;
	  case rl_l:        _L = read_mem(_IX+o); _L = RL(_L); write_mem(_IX+o, _L); break;
	  case rl_mhl:      { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RL(b)); } break;
	  case rrc_a:       _A = read_mem(_IX+o); _A = RRC(_A); write_mem(_IX+o, _A); break;
	  case rrc_b:       _B = read_mem(_IX+o); _B = RRC(_B); write_mem(_IX+o, _B); break;
	  case rrc_c:       _C = read_mem(_IX+o); _C = RRC(_C); write_mem(_IX+o, _C); break;
	  case rrc_d:       _D = read_mem(_IX+o); _D = RRC(_D); write_mem(_IX+o, _D); break;
	  case rrc_e:       _E = read_mem(_IX+o); _E = RRC(_E); write_mem(_IX+o, _E); break;
	  case rrc_h:       _H = read_mem(_IX+o); _H = RRC(_H); write_mem(_IX+o, _H); break;
	  case rrc_l:       _L = read_mem(_IX+o); _L = RRC(_L); write_mem(_IX+o, _L); break;
	  case rrc_mhl:     { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RRC(b)); } break;
	  case rr_a:        _A = read_mem(_IX+o); _A = RR(_A); write_mem(_IX+o, _A); break;
	  case rr_b:        _B = read_mem(_IX+o); _B = RR(_B); write_mem(_IX+o, _B); break;
	  case rr_c:        _C = read_mem(_IX+o); _C = RR(_C); write_mem(_IX+o, _C); break;
	  case rr_d:        _D = read_mem(_IX+o); _D = RR(_D); write_mem(_IX+o, _D); break;
	  case rr_e:        _E = read_mem(_IX+o); _E = RR(_E); write_mem(_IX+o, _E); break;
	  case rr_h:        _H = read_mem(_IX+o); _H = RR(_H); write_mem(_IX+o, _H); break;
	  case rr_l:        _L = read_mem(_IX+o); _L = RR(_L); write_mem(_IX+o, _L); break;
	  case rr_mhl:      { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, RR(b)); } break;
	  case set0_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(0, _A)); break;
	  case set0_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(0, _B)); break;
	  case set0_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(0, _C)); break;
	  case set0_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(0, _D)); break;
	  case set0_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(0, _E)); break;
	  case set0_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(0, _H)); break;
	  case set0_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(0, _L)); break;
	  case set0_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(0, b)); } break;
	  case set1_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(1, _A)); break;
	  case set1_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(1, _B)); break;
	  case set1_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(1, _C)); break;
	  case set1_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(1, _D)); break;
	  case set1_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(1, _E)); break;
	  case set1_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(1, _H)); break;
	  case set1_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(1, _L)); break;
	  case set1_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(1, b)); } break;
	  case set2_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(2, _A)); break;
	  case set2_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(2, _B)); break;
	  case set2_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(2, _C)); break;
	  case set2_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(2, _D)); break;
	  case set2_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(2, _E)); break;
	  case set2_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(2, _H)); break;
	  case set2_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(2, _L)); break;
	  case set2_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(2, b)); } break;
	  case set3_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(3, _A)); break;
	  case set3_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(3, _B)); break;
	  case set3_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(3, _C)); break;
	  case set3_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(3, _D)); break;
	  case set3_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(3, _E)); break;
	  case set3_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(3, _H)); break;
	  case set3_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(3, _L)); break;
	  case set3_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(3, b)); } break;
	  case set4_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(4, _A)); break;
	  case set4_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(4, _B)); break;
	  case set4_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(4, _C)); break;
	  case set4_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(4, _D)); break;
	  case set4_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(4, _E)); break;
	  case set4_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(4, _H)); break;
	  case set4_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(4, _L)); break;
	  case set4_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(4, b)); } break;
	  case set5_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(5, _A)); break;
	  case set5_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(5, _B)); break;
	  case set5_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(5, _C)); break;
	  case set5_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(5, _D)); break;
	  case set5_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(5, _E)); break;
	  case set5_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(5, _H)); break;
	  case set5_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(5, _L)); break;
	  case set5_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(5, b)); } break;
	  case set6_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(6, _A)); break;
	  case set6_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(6, _B)); break;
	  case set6_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(6, _C)); break;
	  case set6_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(6, _D)); break;
	  case set6_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(6, _E)); break;
	  case set6_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(6, _H)); break;
	  case set6_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(6, _L)); break;
	  case set6_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(6, b)); } break;
	  case set7_a:      _A = read_mem(_IX+o); write_mem(_IX+o, _A = SET(7, _A)); break;
	  case set7_b:      _B = read_mem(_IX+o); write_mem(_IX+o, _B = SET(7, _B)); break;
	  case set7_c:      _C = read_mem(_IX+o); write_mem(_IX+o, _C = SET(7, _C)); break;
	  case set7_d:      _D = read_mem(_IX+o); write_mem(_IX+o, _D = SET(7, _D)); break;
	  case set7_e:      _E = read_mem(_IX+o); write_mem(_IX+o, _E = SET(7, _E)); break;
	  case set7_h:      _H = read_mem(_IX+o); write_mem(_IX+o, _H = SET(7, _H)); break;
	  case set7_l:      _L = read_mem(_IX+o); write_mem(_IX+o, _L = SET(7, _L)); break;
	  case set7_mhl:    { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SET(7, b)); } break;
	  case sla_a:       _A = read_mem(_IX+o); _A = SLA(_A); write_mem(_IX+o, _A); break;
	  case sla_b:       _B = read_mem(_IX+o); _B = SLA(_B); write_mem(_IX+o, _B); break;
	  case sla_c:       _C = read_mem(_IX+o); _C = SLA(_C); write_mem(_IX+o, _C); break;
	  case sla_d:       _D = read_mem(_IX+o); _D = SLA(_D); write_mem(_IX+o, _D); break;
	  case sla_e:       _E = read_mem(_IX+o); _E = SLA(_E); write_mem(_IX+o, _E); break;
	  case sla_h:       _H = read_mem(_IX+o); _H = SLA(_H); write_mem(_IX+o, _H); break;
	  case sla_l:       _L = read_mem(_IX+o); _L = SLA(_L); write_mem(_IX+o, _L); break;
	  case sla_mhl:     { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SLA(b)); } break;
	  case sll_a:       _A = read_mem(_IX+o); _A = SLL(_A); write_mem(_IX+o, _A); break;
	  case sll_b:       _B = read_mem(_IX+o); _B = SLL(_B); write_mem(_IX+o, _B); break;
	  case sll_c:       _C = read_mem(_IX+o); _C = SLL(_C); write_mem(_IX+o, _C); break;
	  case sll_d:       _D = read_mem(_IX+o); _D = SLL(_D); write_mem(_IX+o, _D); break;
	  case sll_e:       _E = read_mem(_IX+o); _E = SLL(_E); write_mem(_IX+o, _E); break;
	  case sll_h:       _H = read_mem(_IX+o); _H = SLL(_H); write_mem(_IX+o, _H); break;
	  case sll_l:       _L = read_mem(_IX+o); _L = SLL(_L); write_mem(_IX+o, _L); break;
	  case sll_mhl:     { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SLL(b)); } break;
	  case sra_a:       _A = read_mem(_IX+o); _A = SRA(_A); write_mem(_IX+o, _A); break;
	  case sra_b:       _B = read_mem(_IX+o); _B = SRA(_B); write_mem(_IX+o, _B); break;
	  case sra_c:       _C = read_mem(_IX+o); _C = SRA(_C); write_mem(_IX+o, _C); break;
	  case sra_d:       _D = read_mem(_IX+o); _D = SRA(_D); write_mem(_IX+o, _D); break;
	  case sra_e:       _E = read_mem(_IX+o); _E = SRA(_E); write_mem(_IX+o, _E); break;
	  case sra_h:       _H = read_mem(_IX+o); _H = SRA(_H); write_mem(_IX+o, _H); break;
	  case sra_l:       _L = read_mem(_IX+o); _L = SRA(_L); write_mem(_IX+o, _L); break;
	  case sra_mhl:     { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SRA(b)); } break;
	  case srl_a:       _A = read_mem(_IX+o); _A = SRL(_A); write_mem(_IX+o, _A); break;
	  case srl_b:       _B = read_mem(_IX+o); _B = SRL(_B); write_mem(_IX+o, _B); break;
	  case srl_c:       _C = read_mem(_IX+o); _C = SRL(_C); write_mem(_IX+o, _C); break;
	  case srl_d:       _D = read_mem(_IX+o); _D = SRL(_D); write_mem(_IX+o, _D); break;
	  case srl_e:       _E = read_mem(_IX+o); _E = SRL(_E); write_mem(_IX+o, _E); break;
	  case srl_h:       _H = read_mem(_IX+o); _H = SRL(_H); write_mem(_IX+o, _H); break;
	  case srl_l:       _L = read_mem(_IX+o); _L = SRL(_L); write_mem(_IX+o, _L); break;
	  case srl_mhl:     { UBYTE b = read_mem(_IX+o); write_mem(_IX+o, SRL(b)); } break;
	}
}

void Z80::z80_pfx_ed()
{
	register UBYTE bOpCode;

	bOpCode = read_mem(_PC++);
	iCycleCount += cc_ed[bOpCode];
	_R++;
	switch(bOpCode)
	{
	  case adc_hl_bc:   ADC16(BC); break;
	  case adc_hl_de:   ADC16(DE); break;
	  case adc_hl_hl:   ADC16(HL); break;
	  case adc_hl_sp:   ADC16(SP); break;
	  case cpd:         CPD; break;
	  case cpdr:        CPDR; break;
	  case cpi:         CPI; break;
	  case cpir:        CPIR; break;
	  case ed_00:       break;
	  case ed_01:       break;
	  case ed_02:       break;
	  case ed_03:       break;
	  case ed_04:       break;
	  case ed_05:       break;
	  case ed_06:       break;
	  case ed_07:       break;
	  case ed_08:       break;
	  case ed_09:       break;
	  case ed_0a:       break;
	  case ed_0b:       break;
	  case ed_0c:       break;
	  case ed_0d:       break;
	  case ed_0e:       break;
	  case ed_0f:       break;
	  case ed_10:       break;
	  case ed_11:       break;
	  case ed_12:       break;
	  case ed_13:       break;
	  case ed_14:       break;
	  case ed_15:       break;
	  case ed_16:       break;
	  case ed_17:       break;
	  case ed_18:       break;
	  case ed_19:       break;
	  case ed_1a:       break;
	  case ed_1b:       break;
	  case ed_1c:       break;
	  case ed_1d:       break;
	  case ed_1e:       break;
	  case ed_1f:       break;
	  case ed_20:       break;
	  case ed_21:       break;
	  case ed_22:       break;
	  case ed_23:       break;
	  case ed_24:       break;
	  case ed_25:       break;
	  case ed_26:       break;
	  case ed_27:       break;
	  case ed_28:       break;
	  case ed_29:       break;
	  case ed_2a:       break;
	  case ed_2b:       break;
	  case ed_2c:       break;
	  case ed_2d:       break;
	  case ed_2e:       break;
	  case ed_2f:       break;
	  case ed_30:       break;
	  case ed_31:       break;
	  case ed_32:       break;
	  case ed_33:       break;
	  case ed_34:       break;
	  case ed_35:       break;
	  case ed_36:       break;
	  case ed_37:       break;
	  case ed_38:       break;
	  case ed_39:       break;
	  case ed_3a:       break;
	  case ed_3b:       break;
	  case ed_3c:       break;
	  case ed_3d:       break;
	  case ed_3e:       break;
	  case ed_3f:       break;
	  case ed_77:       break;
	  case ed_7f:       break;
	  case ed_80:       break;
	  case ed_81:       break;
	  case ed_82:       break;
	  case ed_83:       break;
	  case ed_84:       break;
	  case ed_85:       break;
	  case ed_86:       break;
	  case ed_87:       break;
	  case ed_88:       break;
	  case ed_89:       break;
	  case ed_8a:       break;
	  case ed_8b:       break;
	  case ed_8c:       break;
	  case ed_8d:       break;
	  case ed_8e:       break;
	  case ed_8f:       break;
	  case ed_90:       break;
	  case ed_91:       break;
	  case ed_92:       break;
	  case ed_93:       break;
	  case ed_94:       break;
	  case ed_95:       break;
	  case ed_96:       break;
	  case ed_97:       break;
	  case ed_98:       break;
	  case ed_99:       break;
	  case ed_9a:       break;
	  case ed_9b:       break;
	  case ed_9c:       break;
	  case ed_9d:       break;
	  case ed_9e:       break;
	  case ed_9f:       break;
	  case ed_a4:       break;
	  case ed_a5:       break;
	  case ed_a6:       break;
	  case ed_a7:       break;
	  case ed_ac:       break;
	  case ed_ad:       break;
	  case ed_ae:       break;
	  case ed_af:       break;
	  case ed_b4:       break;
	  case ed_b5:       break;
	  case ed_b6:       break;
	  case ed_b7:       break;
	  case ed_bc:       break;
	  case ed_bd:       break;
	  case ed_be:       break;
	  case ed_bf:       break;
	  case ed_c0:       break;
	  case ed_c1:       break;
	  case ed_c2:       break;
	  case ed_c3:       break;
	  case ed_c4:       break;
	  case ed_c5:       break;
	  case ed_c6:       break;
	  case ed_c7:       break;
	  case ed_c8:       break;
	  case ed_c9:       break;
	  case ed_ca:       break;
	  case ed_cb:       break;
	  case ed_cc:       break;
	  case ed_cd:       break;
	  case ed_ce:       break;
	  case ed_cf:       break;
	  case ed_d0:       break;
	  case ed_d1:       break;
	  case ed_d2:       break;
	  case ed_d3:       break;
	  case ed_d4:       break;
	  case ed_d5:       break;
	  case ed_d6:       break;
	  case ed_d7:       break;
	  case ed_d8:       break;
	  case ed_d9:       break;
	  case ed_da:       break;
	  case ed_db:       break;
	  case ed_dc:       break;
	  case ed_dd:       break;
	  case ed_de:       break;
	  case ed_df:       break;
	  case ed_e0:       break;
	  case ed_e1:       break;
	  case ed_e2:       break;
	  case ed_e3:       break;
	  case ed_e4:       break;
	  case ed_e5:       break;
	  case ed_e6:       break;
	  case ed_e7:       break;
	  case ed_e8:       break;
	  case ed_e9:       break;
	  case ed_ea:       break;
	  case ed_eb:       break;
	  case ed_ec:       break;
	  case ed_ed:       break;
	  case ed_ee:       break;
	  case ed_ef:       break;
	  case ed_f0:       break;
	  case ed_f1:       break;
	  case ed_f2:       break;
	  case ed_f3:       break;
	  case ed_f4:       break;
	  case ed_f5:       break;
	  case ed_f6:       break;
	  case ed_f7:       break;
	  case ed_f8:       break;
	  case ed_f9:       break;
	  case ed_fa:       break;
	  case ed_fb:       break;
	  case ed_fc:       break;
	  case ed_fd:       break;
	  case ed_fe:       break;
	  case ed_ff:       break;
	  case im_0:        _IM = 0; break;
	  case im_0_1:      _IM = 0; break;
	  case im_0_2:      _IM = 0; break;
	  case im_0_3:      _IM = 0; break;
	  case im_1:        _IM = 1; break;
	  case im_1_1:      _IM = 1; break;
	  case im_2:        _IM = 2; break;
	  case im_2_1:      _IM = 2; break;
	  case ind:         IND; break;
	  case indr:        INDR; break;
	  case ini:         INI; break;
	  case inir:        INIR; break;
	  case in_0_c:      { UBYTE res = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[res]; } break;
	  case in_a_c:      _A = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_A]; break;
	  case in_b_c:      _B = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_B]; break;
	  case in_c_c:      _C = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_C]; break;
	  case in_d_c:      _D = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_D]; break;
	  case in_e_c:      _E = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_E]; break;
	  case in_h_c:      _H = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_H]; break;
	  case in_l_c:      _L = z80_IN_handler(z80.BC); _F = (_F & Cflag) | SZP[_L]; break;
	  case ldd:         LDD; iWSAdjust++; break;
	  case lddr:        LDDR; iWSAdjust++; break;
	  case ldi:         LDI; iWSAdjust++; break;
	  case ldir:        LDIR; iWSAdjust++; break;
	  case ld_a_i:      _A = _I; _F = (_F & Cflag) | SZ[_A] | _IFF2; iWSAdjust++; break;
	  case ld_a_r:      _A = (_R & 0x7f) | _Rb7; _F = (_F & Cflag) | SZ[_A] | _IFF2; iWSAdjust++; break;
	  case ld_EDbc_mword:  LD16_MEM(BC); break;
	  case ld_EDde_mword:  LD16_MEM(DE); break;
	  case ld_EDhl_mword:  LD16_MEM(HL); break;
	  case ld_EDmword_bc:  LDMEM_16(BC); break;
	  case ld_EDmword_de:  LDMEM_16(DE); break;
	  case ld_EDmword_hl:  LDMEM_16(HL); break;
	  case ld_EDmword_sp:  LDMEM_16(SP); break;
	  case ld_EDsp_mword:  LD16_MEM(SP); break;
	  case ld_i_a:      _I = _A; iWSAdjust++; break;
	  case ld_r_a:      _R = _A; _Rb7 = _A & 0x80; iWSAdjust++; break;
	  case neg:         NEG; break;
	  case neg_1:       NEG; break;
	  case neg_2:       NEG; break;
	  case neg_3:       NEG; break;
	  case neg_4:       NEG; break;
	  case neg_5:       NEG; break;
	  case neg_6:       NEG; break;
	  case neg_7:       NEG; break;
	  case otdr:        OTDR; break;
	  case otir:        OTIR; break;
	  case outd:        OUTD; break;
	  case outi:        OUTI; break;
	  case out_c_0:     z80_OUT_handler(z80.BC, 0); break;
	  case out_c_a:     z80_OUT_handler(z80.BC, _A); break;
	  case out_c_b:     z80_OUT_handler(z80.BC, _B); break;
	  case out_c_c:     z80_OUT_handler(z80.BC, _C); break;
	  case out_c_d:     z80_OUT_handler(z80.BC, _D); break;
	  case out_c_e:     z80_OUT_handler(z80.BC, _E); break;
	  case out_c_h:     z80_OUT_handler(z80.BC, _H); break;
	  case out_c_l:     z80_OUT_handler(z80.BC, _L); break;
	  case reti:        _IFF1 = _IFF2; RET; break;
	  case reti_1:      _IFF1 = _IFF2; RET; break;
	  case reti_2:      _IFF1 = _IFF2; RET; break;
	  case reti_3:      _IFF1 = _IFF2; RET; break;
	  case retn:        _IFF1 = _IFF2; RET; break;
	  case retn_1:      _IFF1 = _IFF2; RET; break;
	  case retn_2:      _IFF1 = _IFF2; RET; break;
	  case retn_3:      _IFF1 = _IFF2; RET; break;
	  case rld:         RLD; break;
	  case rrd:         RRD; break;
	  case sbc_hl_bc:   SBC16(BC); break;
	  case sbc_hl_de:   SBC16(DE); break;
	  case sbc_hl_hl:   SBC16(HL); break;
	  case sbc_hl_sp:   SBC16(SP); break;
	}
}

void Z80::z80_pfx_fd()
{
	register UBYTE bOpCode;

	bOpCode = read_mem(_PC++);
	iCycleCount += cc_xy[bOpCode];
	_R++;
	switch(bOpCode)
	{
	  case adc_a:       ADC(_A); break;
	  case adc_b:       ADC(_B); break;
	  case adc_byte:    ADC(read_mem(_PC++)); break;
	  case adc_c:       ADC(_C); break;
	  case adc_d:       ADC(_D); break;
	  case adc_e:       ADC(_E); break;
	  case adc_h:       ADC(_IYh); break;
	  case adc_l:       ADC(_IYl); break;
	  case adc_mhl:     { signed char o = read_mem(_PC++); ADC(read_mem(_IY+o)); } break;
	  case add_a:       ADD(_A); break;
	  case add_b:       ADD(_B); break;
	  case add_byte:    ADD(read_mem(_PC++)); break;
	  case add_c:       ADD(_C); break;
	  case add_d:       ADD(_D); break;
	  case add_e:       ADD(_E); break;
	  case add_h:       ADD(_IYh); break;
	  case add_hl_bc:   ADD16(IY, BC); break;
	  case add_hl_de:   ADD16(IY, DE); break;
	  case add_hl_hl:   ADD16(IY, IY); break;
	  case add_hl_sp:   ADD16(IY, SP); break;
	  case add_l:       ADD(_IYl); break;
	  case add_mhl:     { signed char o = read_mem(_PC++); ADD(read_mem(_IY+o)); } break;
	  case and_a:       AND(_A); break;
	  case and_b:       AND(_B); break;
	  case and_byte:    AND(read_mem(_PC++)); break;
	  case and_c:       AND(_C); break;
	  case and_d:       AND(_D); break;
	  case and_e:       AND(_E); break;
	  case and_h:       AND(_IYh); break;
	  case and_l:       AND(_IYl); break;
	  case and_mhl:     { signed char o = read_mem(_PC++); AND(read_mem(_IY+o)); } break;
	  case call:        CALL; break;
	  case call_c:      if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_m:      if (_F & Sflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_nc:     if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_nz:     if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_p:      if (!(_F & Sflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_pe:     if (_F & Pflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_po:     if (!(_F & Pflag)) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case call_z:      if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; CALL } else { _PC += 2; } break;
	  case ccf:         _F = ((_F & (Sflag | Zflag | Pflag | Cflag)) | ((_F & CF) << 4) | (_A & Xflags)) ^ CF; break;
	  case cpl:         _A ^= 0xff; _F = (_F & (Sflag | Zflag | Pflag | Cflag)) | Hflag | Nflag | (_A & Xflags); break;
	  case cp_a:        CP(_A); break;
	  case cp_b:        CP(_B); break;
	  case cp_byte:     CP(read_mem(_PC++)); break;
	  case cp_c:        CP(_C); break;
	  case cp_d:        CP(_D); break;
	  case cp_e:        CP(_E); break;
	  case cp_h:        CP(_IYh); break;
	  case cp_l:        CP(_IYl); break;
	  case cp_mhl:      { signed char o = read_mem(_PC++); CP(read_mem(_IY+o)); } break;
	  case daa:         DAA; break;
	  case dec_a:       DEC(_A); break;
	  case dec_b:       DEC(_B); break;
	  case dec_bc:      _BC--; iWSAdjust++; break;
	  case dec_c:       DEC(_C); break;
	  case dec_d:       DEC(_D); break;
	  case dec_de:      _DE--; iWSAdjust++; break;
	  case dec_e:       DEC(_E); break;
	  case dec_h:       DEC(_IYh); break;
	  case dec_hl:      _IY--; iWSAdjust++; break;
	  case dec_l:       DEC(_IYl); break;
	  case dec_mhl:     { signed char o = read_mem(_PC++); UBYTE b = read_mem(_IY+o); DEC(b); write_mem(_IY+o, b); } break;
	  case dec_sp:      _SP--; iWSAdjust++; break;
	  case di:          _IFF1 = _IFF2 = 0; z80.EI_issued = 0; break;
	  case djnz:        if (--_B) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; } break;
	  case ei:          z80.EI_issued = 2; break;
	  case exx:         EXX; break;
	  case ex_af_af:    EX(z80.AF, z80.AFx); break;
	  case ex_de_hl:    EX(z80.DE, z80.HL); break;
	  case ex_msp_hl:   EX_SP(IY); iWSAdjust++; break;
	  case halt:        _HALT = 1; _PC--; break;
	  case ina:         { REGPAIR p; p.b.l = read_mem(_PC++); p.b.h = _A; _A = z80_IN_handler(p); } break;
	  case inc_a:       INC(_A); break;
	  case inc_b:       INC(_B); break;
	  case inc_bc:      _BC++; iWSAdjust++; break;
	  case inc_c:       INC(_C); break;
	  case inc_d:       INC(_D); break;
	  case inc_de:      _DE++; iWSAdjust++; break;
	  case inc_e:       INC(_E); break;
	  case inc_h:       INC(_IYh); break;
	  case inc_hl:      _IY++; iWSAdjust++; break;
	  case inc_l:       INC(_IYl); break;
	  case inc_mhl:     { signed char o = read_mem(_PC++); UBYTE b = read_mem(_IY+o); INC(b); write_mem(_IY+o, b); } break;
	  case inc_sp:      _SP++; iWSAdjust++; break;
	  case jp:          JP; break;
	  case jp_c:        if (_F & Cflag) { JP } else { _PC += 2; }; break;
	  case jp_m:        if (_F & Sflag) { JP } else { _PC += 2; }; break;
	  case jp_nc:       if (!(_F & Cflag)) { JP } else { _PC += 2; }; break;
	  case jp_nz:       if (!(_F & Zflag)) { JP } else { _PC += 2; }; break;
	  case jp_p:        if (!(_F & Sflag)) { JP } else { _PC += 2; }; break;
	  case jp_pe:       if (_F & Pflag) { JP } else { _PC += 2; }; break;
	  case jp_po:       if (!(_F & Pflag)) { JP } else { _PC += 2; }; break;
	  case jp_z:        if (_F & Zflag) { JP } else { _PC += 2; }; break;
	  case jr:          JR; break;
	  case jr_c:        if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case jr_nc:       if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case jr_nz:       if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case jr_z:        if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _PC++; }; break;
	  case ld_a_a:      break;
	  case ld_a_b:      _A = _B; break;
	  case ld_a_byte:   _A = read_mem(_PC++); break;
	  case ld_a_c:      _A = _C; break;
	  case ld_a_d:      _A = _D; break;
	  case ld_a_e:      _A = _E; break;
	  case ld_a_h:      _A = _IYh; break;
	  case ld_a_l:      _A = _IYl; break;
	  case ld_a_mbc:    _A = read_mem(_BC); break;
	  case ld_a_mde:    _A = read_mem(_DE); break;
	  case ld_a_mhl:    { signed char o = read_mem(_PC++); _A = read_mem(_IY+o); } break;
	  case ld_a_mword:  { REGPAIR addr; addr.b.l = read_mem(_PC++); addr.b.h = read_mem(_PC++); _A = read_mem(addr.w.l); } break;
	  case ld_bc_word:  z80.BC.b.l = read_mem(_PC++); z80.BC.b.h = read_mem(_PC++); break;
	  case ld_b_a:      _B = _A; break;
	  case ld_b_b:      break;
	  case ld_b_byte:   _B = read_mem(_PC++); break;
	  case ld_b_c:      _B = _C; break;
	  case ld_b_d:      _B = _D; break;
	  case ld_b_e:      _B = _E; break;
	  case ld_b_h:      _B = _IYh; break;
	  case ld_b_l:      _B = _IYl; break;
	  case ld_b_mhl:    { signed char o = read_mem(_PC++); _B = read_mem(_IY+o); } break;
	  case ld_c_a:      _C = _A; break;
	  case ld_c_b:      _C = _B; break;
	  case ld_c_byte:   _C = read_mem(_PC++); break;
	  case ld_c_c:      break;
	  case ld_c_d:      _C = _D; break;
	  case ld_c_e:      _C = _E; break;
	  case ld_c_h:      _C = _IYh; break;
	  case ld_c_l:      _C = _IYl; break;
	  case ld_c_mhl:    { signed char o = read_mem(_PC++); _C = read_mem(_IY+o); } break;
	  case ld_de_word:  z80.DE.b.l = read_mem(_PC++); z80.DE.b.h = read_mem(_PC++); break;
	  case ld_d_a:      _D = _A; break;
	  case ld_d_b:      _D = _B; break;
	  case ld_d_byte:   _D = read_mem(_PC++); break;
	  case ld_d_c:      _D = _C; break;
	  case ld_d_d:      break;
	  case ld_d_e:      _D = _E; break;
	  case ld_d_h:      _D = _IYh; break;
	  case ld_d_l:      _D = _IYl; break;
	  case ld_d_mhl:    { signed char o = read_mem(_PC++); _D = read_mem(_IY+o); } break;
	  case ld_e_a:      _E = _A; break;
	  case ld_e_b:      _E = _B; break;
	  case ld_e_byte:   _E = read_mem(_PC++); break;
	  case ld_e_c:      _E = _C; break;
	  case ld_e_d:      _E = _D; break;
	  case ld_e_e:      break;
	  case ld_e_h:      _E = _IYh; break;
	  case ld_e_l:      _E = _IYl; break;
	  case ld_e_mhl:    { signed char o = read_mem(_PC++); _E = read_mem(_IY+o); } break;
	  case ld_hl_mword: LD16_MEM(IY); break;
	  case ld_hl_word:  z80.IY.b.l = read_mem(_PC++); z80.IY.b.h = read_mem(_PC++); break;
	  case ld_h_a:      _IYh = _A; break;
	  case ld_h_b:      _IYh = _B; break;
	  case ld_h_byte:   _IYh = read_mem(_PC++); break;
	  case ld_h_c:      _IYh = _C; break;
	  case ld_h_d:      _IYh = _D; break;
	  case ld_h_e:      _IYh = _E; break;
	  case ld_h_h:      break;
	  case ld_h_l:      _IYh = _IYl; break;
	  case ld_h_mhl:    { signed char o = read_mem(_PC++); _H = read_mem(_IY+o); } break;
	  case ld_l_a:      _IYl = _A; break;
	  case ld_l_b:      _IYl = _B; break;
	  case ld_l_byte:   _IYl = read_mem(_PC++); break;
	  case ld_l_c:      _IYl = _C; break;
	  case ld_l_d:      _IYl = _D; break;
	  case ld_l_e:      _IYl = _E; break;
	  case ld_l_h:      _IYl = _IYh; break;
	  case ld_l_l:      break;
	  case ld_l_mhl:    { signed char o = read_mem(_PC++); _L = read_mem(_IY+o); } break;
	  case ld_mbc_a:    write_mem(_BC, _A); break;
	  case ld_mde_a:    write_mem(_DE, _A); break;
	  case ld_mhl_a:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _A); } break;
	  case ld_mhl_b:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _B); } break;
	  case ld_mhl_byte: { signed char o = read_mem(_PC++); UBYTE b = read_mem(_PC++); write_mem(_IY+o, b); } break;
	  case ld_mhl_c:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _C); } break;
	  case ld_mhl_d:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _D); } break;
	  case ld_mhl_e:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _E); } break;
	  case ld_mhl_h:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _H); } break;
	  case ld_mhl_l:    { signed char o = read_mem(_PC++); write_mem(_IY+o, _L); } break;
	  case ld_mword_a:  { REGPAIR addr; addr.b.l = read_mem(_PC++); addr.b.h = read_mem(_PC++); write_mem(addr.w.l, _A); } break;
	  case ld_mword_hl: LDMEM_16(IY); break;
	  case ld_pc_hl:    _PC = _IY; break;
	  case ld_sp_hl:    _SP = _IY; iWSAdjust++; break;
	  case ld_sp_word:  z80.SP.b.l = read_mem(_PC++); z80.SP.b.h = read_mem(_PC++); break;
	  case nop:         break;
	  case or_a:        OR(_A); break;
	  case or_b:        OR(_B); break;
	  case or_byte:     OR(read_mem(_PC++)); break;
	  case or_c:        OR(_C); break;
	  case or_d:        OR(_D); break;
	  case or_e:        OR(_E); break;
	  case or_h:        OR(_IYh); break;
	  case or_l:        OR(_IYl); break;
	  case or_mhl:      { signed char o = read_mem(_PC++); OR(read_mem(_IY+o)); } break;
	  case outa:        { REGPAIR p; p.b.l = read_mem(_PC++); p.b.h = _A; z80_OUT_handler(p, _A); } break;
	  case pfx_cb:      z80_pfx_fdcb(); break;
	  case pfx_dd:      z80_pfx_dd(); break;
	  case pfx_ed:      z80_pfx_ed(); break;
	  case pfx_fd:      z80_pfx_fd(); break;
	  case pop_af:      POP(AF); break;
	  case pop_bc:      POP(BC); break;
	  case pop_de:      POP(DE); break;
	  case pop_hl:      POP(IY); break;
	  case push_af:     PUSH(AF); break;
	  case push_bc:     PUSH(BC); break;
	  case push_de:     PUSH(DE); break;
	  case push_hl:     PUSH(IY); break;
	  case ret:         RET; break;
	  case ret_c:       if (_F & Cflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_m:       if (_F & Sflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_nc:      if (!(_F & Cflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_nz:      if (!(_F & Zflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_p:       if (!(_F & Sflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_pe:      if (_F & Pflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_po:      if (!(_F & Pflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case ret_z:       if (_F & Zflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	  case rla:         RLA; break;
	  case rlca:        RLCA; break;
	  case rra:         RRA; break;
	  case rrca:        RRCA; break;
	  case rst00:       RST(0x0000); break;
	  case rst08:       RST(0x0008); break;
	  case rst10:       RST(0x0010); break;
	  case rst18:       RST(0x0018); break;
	  case rst20:       RST(0x0020); break;
	  case rst28:       RST(0x0028); break;
	  case rst30:       RST(0x0030); break;
	  case rst38:       RST(0x0038); break;
	  case sbc_a:       SBC(_A); break;
	  case sbc_b:       SBC(_B); break;
	  case sbc_byte:    SBC(read_mem(_PC++)); break;
	  case sbc_c:       SBC(_C); break;
	  case sbc_d:       SBC(_D); break;
	  case sbc_e:       SBC(_E); break;
	  case sbc_h:       SBC(_IYh); break;
	  case sbc_l:       SBC(_IYl); break;
	  case sbc_mhl:     { signed char o = read_mem(_PC++); SBC(read_mem(_IY+o)); } break;
	  case scf:         _F = (_F & (Sflag | Zflag | Pflag)) | Cflag | (_A & Xflags); break;
	  case sub_a:       SUB(_A); break;
	  case sub_b:       SUB(_B); break;
	  case sub_byte:    SUB(read_mem(_PC++)); break;
	  case sub_c:       SUB(_C); break;
	  case sub_d:       SUB(_D); break;
	  case sub_e:       SUB(_E); break;
	  case sub_h:       SUB(_IYh); break;
	  case sub_l:       SUB(_IYl); break;
	  case sub_mhl:     { signed char o = read_mem(_PC++); SUB(read_mem(_IY+o)); } break;
	  case xor_a:       XOR(_A); break;
	  case xor_b:       XOR(_B); break;
	  case xor_byte:    XOR(read_mem(_PC++)); break;
	  case xor_c:       XOR(_C); break;
	  case xor_d:       XOR(_D); break;
	  case xor_e:       XOR(_E); break;
	  case xor_h:       XOR(_IYh); break;
	  case xor_l:       XOR(_IYl); break;
	  case xor_mhl:     { signed char o = read_mem(_PC++); XOR(read_mem(_IY+o)); } break;
	}
}

void Z80::z80_pfx_fdcb()
{
	signed char o;
	register UBYTE bOpCode;

	o = read_mem(_PC++); // offset
	bOpCode = read_mem(_PC++);
	iCycleCount += cc_xycb[bOpCode];
	switch(bOpCode)
	{
	  case bit0_a:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_b:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_c:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_d:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_e:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_h:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_l:      BIT_XY(0, read_mem(_IY+o)); break;
	  case bit0_mhl:    BIT_XY(0, read_mem(_IY+o)); break;
	  case bit1_a:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_b:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_c:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_d:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_e:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_h:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_l:      BIT_XY(1, read_mem(_IY+o)); break;
	  case bit1_mhl:    BIT_XY(1, read_mem(_IY+o)); break;
	  case bit2_a:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_b:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_c:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_d:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_e:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_h:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_l:      BIT_XY(2, read_mem(_IY+o)); break;
	  case bit2_mhl:    BIT_XY(2, read_mem(_IY+o)); break;
	  case bit3_a:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_b:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_c:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_d:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_e:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_h:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_l:      BIT_XY(3, read_mem(_IY+o)); break;
	  case bit3_mhl:    BIT_XY(3, read_mem(_IY+o)); break;
	  case bit4_a:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_b:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_c:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_d:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_e:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_h:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_l:      BIT_XY(4, read_mem(_IY+o)); break;
	  case bit4_mhl:    BIT_XY(4, read_mem(_IY+o)); break;
	  case bit5_a:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_b:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_c:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_d:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_e:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_h:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_l:      BIT_XY(5, read_mem(_IY+o)); break;
	  case bit5_mhl:    BIT_XY(5, read_mem(_IY+o)); break;
	  case bit6_a:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_b:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_c:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_d:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_e:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_h:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_l:      BIT_XY(6, read_mem(_IY+o)); break;
	  case bit6_mhl:    BIT_XY(6, read_mem(_IY+o)); break;
	  case bit7_a:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_b:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_c:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_d:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_e:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_h:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_l:      BIT_XY(7, read_mem(_IY+o)); break;
	  case bit7_mhl:    BIT_XY(7, read_mem(_IY+o)); break;
	  case res0_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(0, _A)); break;
	  case res0_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(0, _B)); break;
	  case res0_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(0, _C)); break;
	  case res0_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(0, _D)); break;
	  case res0_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(0, _E)); break;
	  case res0_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(0, _H)); break;
	  case res0_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(0, _L)); break;
	  case res0_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(0, b)); } break;
	  case res1_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(1, _A)); break;
	  case res1_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(1, _B)); break;
	  case res1_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(1, _C)); break;
	  case res1_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(1, _D)); break;
	  case res1_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(1, _E)); break;
	  case res1_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(1, _H)); break;
	  case res1_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(1, _L)); break;
	  case res1_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(1, b)); } break;
	  case res2_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(2, _A)); break;
	  case res2_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(2, _B)); break;
	  case res2_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(2, _C)); break;
	  case res2_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(2, _D)); break;
	  case res2_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(2, _E)); break;
	  case res2_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(2, _H)); break;
	  case res2_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(2, _L)); break;
	  case res2_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(2, b)); } break;
	  case res3_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(3, _A)); break;
	  case res3_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(3, _B)); break;
	  case res3_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(3, _C)); break;
	  case res3_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(3, _D)); break;
	  case res3_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(3, _E)); break;
	  case res3_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(3, _H)); break;
	  case res3_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(3, _L)); break;
	  case res3_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(3, b)); } break;
	  case res4_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(4, _A)); break;
	  case res4_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(4, _B)); break;
	  case res4_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(4, _C)); break;
	  case res4_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(4, _D)); break;
	  case res4_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(4, _E)); break;
	  case res4_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(4, _H)); break;
	  case res4_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(4, _L)); break;
	  case res4_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(4, b)); } break;
	  case res5_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(5, _A)); break;
	  case res5_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(5, _B)); break;
	  case res5_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(5, _C)); break;
	  case res5_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(5, _D)); break;
	  case res5_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(5, _E)); break;
	  case res5_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(5, _H)); break;
	  case res5_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(5, _L)); break;
	  case res5_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(5, b)); } break;
	  case res6_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(6, _A)); break;
	  case res6_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(6, _B)); break;
	  case res6_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(6, _C)); break;
	  case res6_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(6, _D)); break;
	  case res6_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(6, _E)); break;
	  case res6_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(6, _H)); break;
	  case res6_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(6, _L)); break;
	  case res6_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(6, b)); } break;
	  case res7_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = RES(7, _A)); break;
	  case res7_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = RES(7, _B)); break;
	  case res7_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = RES(7, _C)); break;
	  case res7_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = RES(7, _D)); break;
	  case res7_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = RES(7, _E)); break;
	  case res7_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = RES(7, _H)); break;
	  case res7_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = RES(7, _L)); break;
	  case res7_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RES(7, b)); } break;
	  case rlc_a:       _A = read_mem(_IY+o); _A = RLC(_A); write_mem(_IY+o, _A); break;
	  case rlc_b:       _B = read_mem(_IY+o); _B = RLC(_B); write_mem(_IY+o, _B); break;
	  case rlc_c:       _C = read_mem(_IY+o); _C = RLC(_C); write_mem(_IY+o, _C); break;
	  case rlc_d:       _D = read_mem(_IY+o); _D = RLC(_D); write_mem(_IY+o, _D); break;
	  case rlc_e:       _E = read_mem(_IY+o); _E = RLC(_E); write_mem(_IY+o, _E); break;
	  case rlc_h:       _H = read_mem(_IY+o); _H = RLC(_H); write_mem(_IY+o, _H); break;
	  case rlc_l:       _L = read_mem(_IY+o); _L = RLC(_L); write_mem(_IY+o, _L); break;
	  case rlc_mhl:     { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RLC(b)); } break;
	  case rl_a:        _A = read_mem(_IY+o); _A = RL(_A); write_mem(_IY+o, _A); break;
	  case rl_b:        _B = read_mem(_IY+o); _B = RL(_B); write_mem(_IY+o, _B); break;
	  case rl_c:        _C = read_mem(_IY+o); _C = RL(_C); write_mem(_IY+o, _C); break;
	  case rl_d:        _D = read_mem(_IY+o); _D = RL(_D); write_mem(_IY+o, _D); break;
	  case rl_e:        _E = read_mem(_IY+o); _E = RL(_E); write_mem(_IY+o, _E); break;
	  case rl_h:        _H = read_mem(_IY+o); _H = RL(_H); write_mem(_IY+o, _H); break;
	  case rl_l:        _L = read_mem(_IY+o); _L = RL(_L); write_mem(_IY+o, _L); break;
	  case rl_mhl:      { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RL(b)); } break;
	  case rrc_a:       _A = read_mem(_IY+o); _A = RRC(_A); write_mem(_IY+o, _A); break;
	  case rrc_b:       _B = read_mem(_IY+o); _B = RRC(_B); write_mem(_IY+o, _B); break;
	  case rrc_c:       _C = read_mem(_IY+o); _C = RRC(_C); write_mem(_IY+o, _C); break;
	  case rrc_d:       _D = read_mem(_IY+o); _D = RRC(_D); write_mem(_IY+o, _D); break;
	  case rrc_e:       _E = read_mem(_IY+o); _E = RRC(_E); write_mem(_IY+o, _E); break;
	  case rrc_h:       _H = read_mem(_IY+o); _H = RRC(_H); write_mem(_IY+o, _H); break;
	  case rrc_l:       _L = read_mem(_IY+o); _L = RRC(_L); write_mem(_IY+o, _L); break;
	  case rrc_mhl:     { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RRC(b)); } break;
	  case rr_a:        _A = read_mem(_IY+o); _A = RR(_A); write_mem(_IY+o, _A); break;
	  case rr_b:        _B = read_mem(_IY+o); _B = RR(_B); write_mem(_IY+o, _B); break;
	  case rr_c:        _C = read_mem(_IY+o); _C = RR(_C); write_mem(_IY+o, _C); break;
	  case rr_d:        _D = read_mem(_IY+o); _D = RR(_D); write_mem(_IY+o, _D); break;
	  case rr_e:        _E = read_mem(_IY+o); _E = RR(_E); write_mem(_IY+o, _E); break;
	  case rr_h:        _H = read_mem(_IY+o); _H = RR(_H); write_mem(_IY+o, _H); break;
	  case rr_l:        _L = read_mem(_IY+o); _L = RR(_L); write_mem(_IY+o, _L); break;
	  case rr_mhl:      { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, RR(b)); } break;
	  case set0_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(0, _A)); break;
	  case set0_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(0, _B)); break;
	  case set0_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(0, _C)); break;
	  case set0_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(0, _D)); break;
	  case set0_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(0, _E)); break;
	  case set0_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(0, _H)); break;
	  case set0_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(0, _L)); break;
	  case set0_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(0, b)); } break;
	  case set1_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(1, _A)); break;
	  case set1_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(1, _B)); break;
	  case set1_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(1, _C)); break;
	  case set1_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(1, _D)); break;
	  case set1_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(1, _E)); break;
	  case set1_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(1, _H)); break;
	  case set1_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(1, _L)); break;
	  case set1_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(1, b)); } break;
	  case set2_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(2, _A)); break;
	  case set2_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(2, _B)); break;
	  case set2_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(2, _C)); break;
	  case set2_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(2, _D)); break;
	  case set2_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(2, _E)); break;
	  case set2_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(2, _H)); break;
	  case set2_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(2, _L)); break;
	  case set2_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(2, b)); } break;
	  case set3_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(3, _A)); break;
	  case set3_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(3, _B)); break;
	  case set3_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(3, _C)); break;
	  case set3_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(3, _D)); break;
	  case set3_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(3, _E)); break;
	  case set3_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(3, _H)); break;
	  case set3_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(3, _L)); break;
	  case set3_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(3, b)); } break;
	  case set4_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(4, _A)); break;
	  case set4_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(4, _B)); break;
	  case set4_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(4, _C)); break;
	  case set4_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(4, _D)); break;
	  case set4_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(4, _E)); break;
	  case set4_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(4, _H)); break;
	  case set4_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(4, _L)); break;
	  case set4_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(4, b)); } break;
	  case set5_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(5, _A)); break;
	  case set5_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(5, _B)); break;
	  case set5_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(5, _C)); break;
	  case set5_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(5, _D)); break;
	  case set5_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(5, _E)); break;
	  case set5_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(5, _H)); break;
	  case set5_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(5, _L)); break;
	  case set5_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(5, b)); } break;
	  case set6_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(6, _A)); break;
	  case set6_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(6, _B)); break;
	  case set6_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(6, _C)); break;
	  case set6_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(6, _D)); break;
	  case set6_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(6, _E)); break;
	  case set6_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(6, _H)); break;
	  case set6_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(6, _L)); break;
	  case set6_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(6, b)); } break;
	  case set7_a:      _A = read_mem(_IY+o); write_mem(_IY+o, _A = SET(7, _A)); break;
	  case set7_b:      _B = read_mem(_IY+o); write_mem(_IY+o, _B = SET(7, _B)); break;
	  case set7_c:      _C = read_mem(_IY+o); write_mem(_IY+o, _C = SET(7, _C)); break;
	  case set7_d:      _D = read_mem(_IY+o); write_mem(_IY+o, _D = SET(7, _D)); break;
	  case set7_e:      _E = read_mem(_IY+o); write_mem(_IY+o, _E = SET(7, _E)); break;
	  case set7_h:      _H = read_mem(_IY+o); write_mem(_IY+o, _H = SET(7, _H)); break;
	  case set7_l:      _L = read_mem(_IY+o); write_mem(_IY+o, _L = SET(7, _L)); break;
	  case set7_mhl:    { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SET(7, b)); } break;
	  case sla_a:       _A = read_mem(_IY+o); _A = SLA(_A); write_mem(_IY+o, _A); break;
	  case sla_b:       _B = read_mem(_IY+o); _B = SLA(_B); write_mem(_IY+o, _B); break;
	  case sla_c:       _C = read_mem(_IY+o); _C = SLA(_C); write_mem(_IY+o, _C); break;
	  case sla_d:       _D = read_mem(_IY+o); _D = SLA(_D); write_mem(_IY+o, _D); break;
	  case sla_e:       _E = read_mem(_IY+o); _E = SLA(_E); write_mem(_IY+o, _E); break;
	  case sla_h:       _H = read_mem(_IY+o); _H = SLA(_H); write_mem(_IY+o, _H); break;
	  case sla_l:       _L = read_mem(_IY+o); _L = SLA(_L); write_mem(_IY+o, _L); break;
	  case sla_mhl:     { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SLA(b)); } break;
	  case sll_a:       _A = read_mem(_IY+o); _A = SLL(_A); write_mem(_IY+o, _A); break;
	  case sll_b:       _B = read_mem(_IY+o); _B = SLL(_B); write_mem(_IY+o, _B); break;
	  case sll_c:       _C = read_mem(_IY+o); _C = SLL(_C); write_mem(_IY+o, _C); break;
	  case sll_d:       _D = read_mem(_IY+o); _D = SLL(_D); write_mem(_IY+o, _D); break;
	  case sll_e:       _E = read_mem(_IY+o); _E = SLL(_E); write_mem(_IY+o, _E); break;
	  case sll_h:       _H = read_mem(_IY+o); _H = SLL(_H); write_mem(_IY+o, _H); break;
	  case sll_l:       _L = read_mem(_IY+o); _L = SLL(_L); write_mem(_IY+o, _L); break;
	  case sll_mhl:     { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SLL(b)); } break;
	  case sra_a:       _A = read_mem(_IY+o); _A = SRA(_A); write_mem(_IY+o, _A); break;
	  case sra_b:       _B = read_mem(_IY+o); _B = SRA(_B); write_mem(_IY+o, _B); break;
	  case sra_c:       _C = read_mem(_IY+o); _C = SRA(_C); write_mem(_IY+o, _C); break;
	  case sra_d:       _D = read_mem(_IY+o); _D = SRA(_D); write_mem(_IY+o, _D); break;
	  case sra_e:       _E = read_mem(_IY+o); _E = SRA(_E); write_mem(_IY+o, _E); break;
	  case sra_h:       _H = read_mem(_IY+o); _H = SRA(_H); write_mem(_IY+o, _H); break;
	  case sra_l:       _L = read_mem(_IY+o); _L = SRA(_L); write_mem(_IY+o, _L); break;
	  case sra_mhl:     { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SRA(b)); } break;
	  case srl_a:       _A = read_mem(_IY+o); _A = SRL(_A); write_mem(_IY+o, _A); break;
	  case srl_b:       _B = read_mem(_IY+o); _B = SRL(_B); write_mem(_IY+o, _B); break;
	  case srl_c:       _C = read_mem(_IY+o); _C = SRL(_C); write_mem(_IY+o, _C); break;
	  case srl_d:       _D = read_mem(_IY+o); _D = SRL(_D); write_mem(_IY+o, _D); break;
	  case srl_e:       _E = read_mem(_IY+o); _E = SRL(_E); write_mem(_IY+o, _E); break;
	  case srl_h:       _H = read_mem(_IY+o); _H = SRL(_H); write_mem(_IY+o, _H); break;
	  case srl_l:       _L = read_mem(_IY+o); _L = SRL(_L); write_mem(_IY+o, _L); break;
	  case srl_mhl:     { UBYTE b = read_mem(_IY+o); write_mem(_IY+o, SRL(b)); } break;
	}
}



