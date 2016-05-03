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

tUBYTE Z80::irep_tmp1[4][4] = {
    {0, 0, 1, 0}, {0, 1, 0, 1}, {1, 0, 1, 1}, {0, 1, 1, 0}};

/* tmp1 value for ind/indr/outd/otdr for [C.1-0][io.1-0] */
tUBYTE Z80::drep_tmp1[4][4] = {
    {0, 1, 0, 0}, {1, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 1}};

/* tmp2 value for all in/out repeated opcodes for B.7-0 */
tUBYTE Z80::breg_tmp2[256] = {
    0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
    0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
    1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1};

tUBYTE Z80::cc_op[256] = {
    4,  12, 8,  8,  4,  4,  8,  4,  4,  12, 8,  8,  4,  4,  8,  4,  12, 12, 8,
    8,  4,  4,  8,  4,  12, 12, 8,  8,  4,  4,  8,  4,  8,  12, 20, 8,  4,  4,
    8,  4,  8,  12, 20, 8,  4,  4,  8,  4,  8,  12, 16, 8,  12, 12, 12, 4,  8,
    12, 16, 8,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,
    4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,
    4,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  8,  8,
    8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,
    4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,
    4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,
    8,  4,  8,  12, 12, 12, 12, 16, 8,  16, 8,  12, 12, 4,  12, 20, 8,  16, 8,
    12, 12, 12, 12, 16, 8,  16, 8,  4,  12, 12, 12, 4,  8,  16, 8,  12, 12, 24,
    12, 16, 8,  16, 8,  4,  12, 4,  12, 4,  8,  16, 8,  12, 12, 4,  12, 16, 8,
    16, 8,  8,  12, 4,  12, 4,  8,  16};

tUBYTE Z80::cc_cb[256] = {
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 8,  4,
    4, 4, 4, 4, 4, 4, 8,  4, 4, 4, 4, 4, 4, 4, 8,  4, 4, 4, 4, 4, 4, 4, 8,  4,
    4, 4, 4, 4, 4, 4, 8,  4, 4, 4, 4, 4, 4, 4, 8,  4, 4, 4, 4, 4, 4, 4, 8,  4,
    4, 4, 4, 4, 4, 4, 8,  4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4,
    4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 4, 4, 4, 4, 12, 4};

tUBYTE Z80::cc_ed[256] = {
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  12, 12, 12, 20, 4,  12, 4,  8,  12, 12, 12, 20,
    4,  12, 4,  8,  12, 12, 12, 20, 4,  12, 4,  8,  12, 12, 12, 20, 4,  12, 4,
    8,  12, 12, 12, 20, 4,  12, 4,  16, 12, 12, 12, 20, 4,  12, 4,  16, 12, 12,
    12, 20, 4,  12, 4,  4,  12, 12, 12, 20, 4,  12, 4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  16, 12, 16, 16, 4,  4,  4,  4,  16, 12, 16,
    16, 4,  4,  4,  4,  16, 12, 16, 16, 4,  4,  4,  4,  16, 12, 16, 16, 4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  4,  4,  4,  4,  4};

tUBYTE Z80::cc_xy[256] = {
    4,  12, 8,  8,  4,  4,  8,  4,  4,  12, 8,  8,  4,  4,  8,  4,  12, 12, 8,
    8,  4,  4,  8,  4,  12, 12, 8,  8,  4,  4,  8,  4,  8,  12, 20, 8,  4,  4,
    8,  4,  8,  12, 20, 8,  4,  4,  8,  4,  8,  12, 16, 8,  20, 20, 20, 4,  8,
    12, 16, 8,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,
    4,  4,  16, 4,  4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,  4,  4,  16,
    4,  4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,  4,  4,  16, 4,  16, 16,
    16, 16, 16, 16, 4,  16, 4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,  4,
    4,  16, 4,  4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,  4,  4,  16, 4,
    4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,
    4,  4,  4,  16, 4,  4,  4,  4,  4,  4,  4,  16, 4,  4,  4,  4,  4,  4,  4,
    16, 4,  8,  12, 12, 12, 12, 16, 8,  16, 8,  12, 12, 4,  12, 20, 8,  16, 8,
    12, 12, 12, 12, 16, 8,  16, 8,  4,  12, 12, 12, 4,  8,  16, 8,  12, 12, 24,
    12, 16, 8,  16, 8,  4,  12, 4,  12, 4,  8,  16, 8,  12, 12, 4,  12, 16, 8,
    16, 8,  8,  12, 4,  12, 4,  8,  16};

tUBYTE Z80::cc_xycb[256] = {
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20};

tUBYTE Z80::cc_ex[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 4, 4, 0, 0, 0, 0, 4, 8, 4, 4, 0, 0, 0, 0,
    8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0,
    8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0,
    8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0};

tUWORD Z80::DAATable[2048] = {
    0x0044, 0x0100, 0x0200, 0x0304, 0x0400, 0x0504, 0x0604, 0x0700, 0x0808,
    0x090C, 0x1010, 0x1114, 0x1214, 0x1310, 0x1414, 0x1510, 0x1000, 0x1104,
    0x1204, 0x1300, 0x1404, 0x1500, 0x1600, 0x1704, 0x180C, 0x1908, 0x2030,
    0x2134, 0x2234, 0x2330, 0x2434, 0x2530, 0x2020, 0x2124, 0x2224, 0x2320,
    0x2424, 0x2520, 0x2620, 0x2724, 0x282C, 0x2928, 0x3034, 0x3130, 0x3230,
    0x3334, 0x3430, 0x3534, 0x3024, 0x3120, 0x3220, 0x3324, 0x3420, 0x3524,
    0x3624, 0x3720, 0x3828, 0x392C, 0x4010, 0x4114, 0x4214, 0x4310, 0x4414,
    0x4510, 0x4000, 0x4104, 0x4204, 0x4300, 0x4404, 0x4500, 0x4600, 0x4704,
    0x480C, 0x4908, 0x5014, 0x5110, 0x5210, 0x5314, 0x5410, 0x5514, 0x5004,
    0x5100, 0x5200, 0x5304, 0x5400, 0x5504, 0x5604, 0x5700, 0x5808, 0x590C,
    0x6034, 0x6130, 0x6230, 0x6334, 0x6430, 0x6534, 0x6024, 0x6120, 0x6220,
    0x6324, 0x6420, 0x6524, 0x6624, 0x6720, 0x6828, 0x692C, 0x7030, 0x7134,
    0x7234, 0x7330, 0x7434, 0x7530, 0x7020, 0x7124, 0x7224, 0x7320, 0x7424,
    0x7520, 0x7620, 0x7724, 0x782C, 0x7928, 0x8090, 0x8194, 0x8294, 0x8390,
    0x8494, 0x8590, 0x8080, 0x8184, 0x8284, 0x8380, 0x8484, 0x8580, 0x8680,
    0x8784, 0x888C, 0x8988, 0x9094, 0x9190, 0x9290, 0x9394, 0x9490, 0x9594,
    0x9084, 0x9180, 0x9280, 0x9384, 0x9480, 0x9584, 0x9684, 0x9780, 0x9888,
    0x998C, 0x0055, 0x0111, 0x0211, 0x0315, 0x0411, 0x0515, 0x0045, 0x0101,
    0x0201, 0x0305, 0x0401, 0x0505, 0x0605, 0x0701, 0x0809, 0x090D, 0x1011,
    0x1115, 0x1215, 0x1311, 0x1415, 0x1511, 0x1001, 0x1105, 0x1205, 0x1301,
    0x1405, 0x1501, 0x1601, 0x1705, 0x180D, 0x1909, 0x2031, 0x2135, 0x2235,
    0x2331, 0x2435, 0x2531, 0x2021, 0x2125, 0x2225, 0x2321, 0x2425, 0x2521,
    0x2621, 0x2725, 0x282D, 0x2929, 0x3035, 0x3131, 0x3231, 0x3335, 0x3431,
    0x3535, 0x3025, 0x3121, 0x3221, 0x3325, 0x3421, 0x3525, 0x3625, 0x3721,
    0x3829, 0x392D, 0x4011, 0x4115, 0x4215, 0x4311, 0x4415, 0x4511, 0x4001,
    0x4105, 0x4205, 0x4301, 0x4405, 0x4501, 0x4601, 0x4705, 0x480D, 0x4909,
    0x5015, 0x5111, 0x5211, 0x5315, 0x5411, 0x5515, 0x5005, 0x5101, 0x5201,
    0x5305, 0x5401, 0x5505, 0x5605, 0x5701, 0x5809, 0x590D, 0x6035, 0x6131,
    0x6231, 0x6335, 0x6431, 0x6535, 0x6025, 0x6121, 0x6221, 0x6325, 0x6421,
    0x6525, 0x6625, 0x6721, 0x6829, 0x692D, 0x7031, 0x7135, 0x7235, 0x7331,
    0x7435, 0x7531, 0x7021, 0x7125, 0x7225, 0x7321, 0x7425, 0x7521, 0x7621,
    0x7725, 0x782D, 0x7929, 0x8091, 0x8195, 0x8295, 0x8391, 0x8495, 0x8591,
    0x8081, 0x8185, 0x8285, 0x8381, 0x8485, 0x8581, 0x8681, 0x8785, 0x888D,
    0x8989, 0x9095, 0x9191, 0x9291, 0x9395, 0x9491, 0x9595, 0x9085, 0x9181,
    0x9281, 0x9385, 0x9481, 0x9585, 0x9685, 0x9781, 0x9889, 0x998D, 0xA0B5,
    0xA1B1, 0xA2B1, 0xA3B5, 0xA4B1, 0xA5B5, 0xA0A5, 0xA1A1, 0xA2A1, 0xA3A5,
    0xA4A1, 0xA5A5, 0xA6A5, 0xA7A1, 0xA8A9, 0xA9AD, 0xB0B1, 0xB1B5, 0xB2B5,
    0xB3B1, 0xB4B5, 0xB5B1, 0xB0A1, 0xB1A5, 0xB2A5, 0xB3A1, 0xB4A5, 0xB5A1,
    0xB6A1, 0xB7A5, 0xB8AD, 0xB9A9, 0xC095, 0xC191, 0xC291, 0xC395, 0xC491,
    0xC595, 0xC085, 0xC181, 0xC281, 0xC385, 0xC481, 0xC585, 0xC685, 0xC781,
    0xC889, 0xC98D, 0xD091, 0xD195, 0xD295, 0xD391, 0xD495, 0xD591, 0xD081,
    0xD185, 0xD285, 0xD381, 0xD485, 0xD581, 0xD681, 0xD785, 0xD88D, 0xD989,
    0xE0B1, 0xE1B5, 0xE2B5, 0xE3B1, 0xE4B5, 0xE5B1, 0xE0A1, 0xE1A5, 0xE2A5,
    0xE3A1, 0xE4A5, 0xE5A1, 0xE6A1, 0xE7A5, 0xE8AD, 0xE9A9, 0xF0B5, 0xF1B1,
    0xF2B1, 0xF3B5, 0xF4B1, 0xF5B5, 0xF0A5, 0xF1A1, 0xF2A1, 0xF3A5, 0xF4A1,
    0xF5A5, 0xF6A5, 0xF7A1, 0xF8A9, 0xF9AD, 0x0055, 0x0111, 0x0211, 0x0315,
    0x0411, 0x0515, 0x0045, 0x0101, 0x0201, 0x0305, 0x0401, 0x0505, 0x0605,
    0x0701, 0x0809, 0x090D, 0x1011, 0x1115, 0x1215, 0x1311, 0x1415, 0x1511,
    0x1001, 0x1105, 0x1205, 0x1301, 0x1405, 0x1501, 0x1601, 0x1705, 0x180D,
    0x1909, 0x2031, 0x2135, 0x2235, 0x2331, 0x2435, 0x2531, 0x2021, 0x2125,
    0x2225, 0x2321, 0x2425, 0x2521, 0x2621, 0x2725, 0x282D, 0x2929, 0x3035,
    0x3131, 0x3231, 0x3335, 0x3431, 0x3535, 0x3025, 0x3121, 0x3221, 0x3325,
    0x3421, 0x3525, 0x3625, 0x3721, 0x3829, 0x392D, 0x4011, 0x4115, 0x4215,
    0x4311, 0x4415, 0x4511, 0x4001, 0x4105, 0x4205, 0x4301, 0x4405, 0x4501,
    0x4601, 0x4705, 0x480D, 0x4909, 0x5015, 0x5111, 0x5211, 0x5315, 0x5411,
    0x5515, 0x5005, 0x5101, 0x5201, 0x5305, 0x5401, 0x5505, 0x5605, 0x5701,
    0x5809, 0x590D, 0x6035, 0x6131, 0x6231, 0x6335, 0x6431, 0x6535, 0x0604,
    0x0700, 0x0808, 0x090C, 0x0A0C, 0x0B08, 0x0C0C, 0x0D08, 0x0E08, 0x0F0C,
    0x1010, 0x1114, 0x1214, 0x1310, 0x1414, 0x1510, 0x1600, 0x1704, 0x180C,
    0x1908, 0x1A08, 0x1B0C, 0x1C08, 0x1D0C, 0x1E0C, 0x1F08, 0x2030, 0x2134,
    0x2234, 0x2330, 0x2434, 0x2530, 0x2620, 0x2724, 0x282C, 0x2928, 0x2A28,
    0x2B2C, 0x2C28, 0x2D2C, 0x2E2C, 0x2F28, 0x3034, 0x3130, 0x3230, 0x3334,
    0x3430, 0x3534, 0x3624, 0x3720, 0x3828, 0x392C, 0x3A2C, 0x3B28, 0x3C2C,
    0x3D28, 0x3E28, 0x3F2C, 0x4010, 0x4114, 0x4214, 0x4310, 0x4414, 0x4510,
    0x4600, 0x4704, 0x480C, 0x4908, 0x4A08, 0x4B0C, 0x4C08, 0x4D0C, 0x4E0C,
    0x4F08, 0x5014, 0x5110, 0x5210, 0x5314, 0x5410, 0x5514, 0x5604, 0x5700,
    0x5808, 0x590C, 0x5A0C, 0x5B08, 0x5C0C, 0x5D08, 0x5E08, 0x5F0C, 0x6034,
    0x6130, 0x6230, 0x6334, 0x6430, 0x6534, 0x6624, 0x6720, 0x6828, 0x692C,
    0x6A2C, 0x6B28, 0x6C2C, 0x6D28, 0x6E28, 0x6F2C, 0x7030, 0x7134, 0x7234,
    0x7330, 0x7434, 0x7530, 0x7620, 0x7724, 0x782C, 0x7928, 0x7A28, 0x7B2C,
    0x7C28, 0x7D2C, 0x7E2C, 0x7F28, 0x8090, 0x8194, 0x8294, 0x8390, 0x8494,
    0x8590, 0x8680, 0x8784, 0x888C, 0x8988, 0x8A88, 0x8B8C, 0x8C88, 0x8D8C,
    0x8E8C, 0x8F88, 0x9094, 0x9190, 0x9290, 0x9394, 0x9490, 0x9594, 0x9684,
    0x9780, 0x9888, 0x998C, 0x9A8C, 0x9B88, 0x9C8C, 0x9D88, 0x9E88, 0x9F8C,
    0x0055, 0x0111, 0x0211, 0x0315, 0x0411, 0x0515, 0x0605, 0x0701, 0x0809,
    0x090D, 0x0A0D, 0x0B09, 0x0C0D, 0x0D09, 0x0E09, 0x0F0D, 0x1011, 0x1115,
    0x1215, 0x1311, 0x1415, 0x1511, 0x1601, 0x1705, 0x180D, 0x1909, 0x1A09,
    0x1B0D, 0x1C09, 0x1D0D, 0x1E0D, 0x1F09, 0x2031, 0x2135, 0x2235, 0x2331,
    0x2435, 0x2531, 0x2621, 0x2725, 0x282D, 0x2929, 0x2A29, 0x2B2D, 0x2C29,
    0x2D2D, 0x2E2D, 0x2F29, 0x3035, 0x3131, 0x3231, 0x3335, 0x3431, 0x3535,
    0x3625, 0x3721, 0x3829, 0x392D, 0x3A2D, 0x3B29, 0x3C2D, 0x3D29, 0x3E29,
    0x3F2D, 0x4011, 0x4115, 0x4215, 0x4311, 0x4415, 0x4511, 0x4601, 0x4705,
    0x480D, 0x4909, 0x4A09, 0x4B0D, 0x4C09, 0x4D0D, 0x4E0D, 0x4F09, 0x5015,
    0x5111, 0x5211, 0x5315, 0x5411, 0x5515, 0x5605, 0x5701, 0x5809, 0x590D,
    0x5A0D, 0x5B09, 0x5C0D, 0x5D09, 0x5E09, 0x5F0D, 0x6035, 0x6131, 0x6231,
    0x6335, 0x6431, 0x6535, 0x6625, 0x6721, 0x6829, 0x692D, 0x6A2D, 0x6B29,
    0x6C2D, 0x6D29, 0x6E29, 0x6F2D, 0x7031, 0x7135, 0x7235, 0x7331, 0x7435,
    0x7531, 0x7621, 0x7725, 0x782D, 0x7929, 0x7A29, 0x7B2D, 0x7C29, 0x7D2D,
    0x7E2D, 0x7F29, 0x8091, 0x8195, 0x8295, 0x8391, 0x8495, 0x8591, 0x8681,
    0x8785, 0x888D, 0x8989, 0x8A89, 0x8B8D, 0x8C89, 0x8D8D, 0x8E8D, 0x8F89,
    0x9095, 0x9191, 0x9291, 0x9395, 0x9491, 0x9595, 0x9685, 0x9781, 0x9889,
    0x998D, 0x9A8D, 0x9B89, 0x9C8D, 0x9D89, 0x9E89, 0x9F8D, 0xA0B5, 0xA1B1,
    0xA2B1, 0xA3B5, 0xA4B1, 0xA5B5, 0xA6A5, 0xA7A1, 0xA8A9, 0xA9AD, 0xAAAD,
    0xABA9, 0xACAD, 0xADA9, 0xAEA9, 0xAFAD, 0xB0B1, 0xB1B5, 0xB2B5, 0xB3B1,
    0xB4B5, 0xB5B1, 0xB6A1, 0xB7A5, 0xB8AD, 0xB9A9, 0xBAA9, 0xBBAD, 0xBCA9,
    0xBDAD, 0xBEAD, 0xBFA9, 0xC095, 0xC191, 0xC291, 0xC395, 0xC491, 0xC595,
    0xC685, 0xC781, 0xC889, 0xC98D, 0xCA8D, 0xCB89, 0xCC8D, 0xCD89, 0xCE89,
    0xCF8D, 0xD091, 0xD195, 0xD295, 0xD391, 0xD495, 0xD591, 0xD681, 0xD785,
    0xD88D, 0xD989, 0xDA89, 0xDB8D, 0xDC89, 0xDD8D, 0xDE8D, 0xDF89, 0xE0B1,
    0xE1B5, 0xE2B5, 0xE3B1, 0xE4B5, 0xE5B1, 0xE6A1, 0xE7A5, 0xE8AD, 0xE9A9,
    0xEAA9, 0xEBAD, 0xECA9, 0xEDAD, 0xEEAD, 0xEFA9, 0xF0B5, 0xF1B1, 0xF2B1,
    0xF3B5, 0xF4B1, 0xF5B5, 0xF6A5, 0xF7A1, 0xF8A9, 0xF9AD, 0xFAAD, 0xFBA9,
    0xFCAD, 0xFDA9, 0xFEA9, 0xFFAD, 0x0055, 0x0111, 0x0211, 0x0315, 0x0411,
    0x0515, 0x0605, 0x0701, 0x0809, 0x090D, 0x0A0D, 0x0B09, 0x0C0D, 0x0D09,
    0x0E09, 0x0F0D, 0x1011, 0x1115, 0x1215, 0x1311, 0x1415, 0x1511, 0x1601,
    0x1705, 0x180D, 0x1909, 0x1A09, 0x1B0D, 0x1C09, 0x1D0D, 0x1E0D, 0x1F09,
    0x2031, 0x2135, 0x2235, 0x2331, 0x2435, 0x2531, 0x2621, 0x2725, 0x282D,
    0x2929, 0x2A29, 0x2B2D, 0x2C29, 0x2D2D, 0x2E2D, 0x2F29, 0x3035, 0x3131,
    0x3231, 0x3335, 0x3431, 0x3535, 0x3625, 0x3721, 0x3829, 0x392D, 0x3A2D,
    0x3B29, 0x3C2D, 0x3D29, 0x3E29, 0x3F2D, 0x4011, 0x4115, 0x4215, 0x4311,
    0x4415, 0x4511, 0x4601, 0x4705, 0x480D, 0x4909, 0x4A09, 0x4B0D, 0x4C09,
    0x4D0D, 0x4E0D, 0x4F09, 0x5015, 0x5111, 0x5211, 0x5315, 0x5411, 0x5515,
    0x5605, 0x5701, 0x5809, 0x590D, 0x5A0D, 0x5B09, 0x5C0D, 0x5D09, 0x5E09,
    0x5F0D, 0x6035, 0x6131, 0x6231, 0x6335, 0x6431, 0x6535, 0x0046, 0x0102,
    0x0202, 0x0306, 0x0402, 0x0506, 0x0606, 0x0702, 0x080A, 0x090E, 0x0402,
    0x0506, 0x0606, 0x0702, 0x080A, 0x090E, 0x1002, 0x1106, 0x1206, 0x1302,
    0x1406, 0x1502, 0x1602, 0x1706, 0x180E, 0x190A, 0x1406, 0x1502, 0x1602,
    0x1706, 0x180E, 0x190A, 0x2022, 0x2126, 0x2226, 0x2322, 0x2426, 0x2522,
    0x2622, 0x2726, 0x282E, 0x292A, 0x2426, 0x2522, 0x2622, 0x2726, 0x282E,
    0x292A, 0x3026, 0x3122, 0x3222, 0x3326, 0x3422, 0x3526, 0x3626, 0x3722,
    0x382A, 0x392E, 0x3422, 0x3526, 0x3626, 0x3722, 0x382A, 0x392E, 0x4002,
    0x4106, 0x4206, 0x4302, 0x4406, 0x4502, 0x4602, 0x4706, 0x480E, 0x490A,
    0x4406, 0x4502, 0x4602, 0x4706, 0x480E, 0x490A, 0x5006, 0x5102, 0x5202,
    0x5306, 0x5402, 0x5506, 0x5606, 0x5702, 0x580A, 0x590E, 0x5402, 0x5506,
    0x5606, 0x5702, 0x580A, 0x590E, 0x6026, 0x6122, 0x6222, 0x6326, 0x6422,
    0x6526, 0x6626, 0x6722, 0x682A, 0x692E, 0x6422, 0x6526, 0x6626, 0x6722,
    0x682A, 0x692E, 0x7022, 0x7126, 0x7226, 0x7322, 0x7426, 0x7522, 0x7622,
    0x7726, 0x782E, 0x792A, 0x7426, 0x7522, 0x7622, 0x7726, 0x782E, 0x792A,
    0x8082, 0x8186, 0x8286, 0x8382, 0x8486, 0x8582, 0x8682, 0x8786, 0x888E,
    0x898A, 0x8486, 0x8582, 0x8682, 0x8786, 0x888E, 0x898A, 0x9086, 0x9182,
    0x9282, 0x9386, 0x9482, 0x9586, 0x9686, 0x9782, 0x988A, 0x998E, 0x3423,
    0x3527, 0x3627, 0x3723, 0x382B, 0x392F, 0x4003, 0x4107, 0x4207, 0x4303,
    0x4407, 0x4503, 0x4603, 0x4707, 0x480F, 0x490B, 0x4407, 0x4503, 0x4603,
    0x4707, 0x480F, 0x490B, 0x5007, 0x5103, 0x5203, 0x5307, 0x5403, 0x5507,
    0x5607, 0x5703, 0x580B, 0x590F, 0x5403, 0x5507, 0x5607, 0x5703, 0x580B,
    0x590F, 0x6027, 0x6123, 0x6223, 0x6327, 0x6423, 0x6527, 0x6627, 0x6723,
    0x682B, 0x692F, 0x6423, 0x6527, 0x6627, 0x6723, 0x682B, 0x692F, 0x7023,
    0x7127, 0x7227, 0x7323, 0x7427, 0x7523, 0x7623, 0x7727, 0x782F, 0x792B,
    0x7427, 0x7523, 0x7623, 0x7727, 0x782F, 0x792B, 0x8083, 0x8187, 0x8287,
    0x8383, 0x8487, 0x8583, 0x8683, 0x8787, 0x888F, 0x898B, 0x8487, 0x8583,
    0x8683, 0x8787, 0x888F, 0x898B, 0x9087, 0x9183, 0x9283, 0x9387, 0x9483,
    0x9587, 0x9687, 0x9783, 0x988B, 0x998F, 0x9483, 0x9587, 0x9687, 0x9783,
    0x988B, 0x998F, 0xA0A7, 0xA1A3, 0xA2A3, 0xA3A7, 0xA4A3, 0xA5A7, 0xA6A7,
    0xA7A3, 0xA8AB, 0xA9AF, 0xA4A3, 0xA5A7, 0xA6A7, 0xA7A3, 0xA8AB, 0xA9AF,
    0xB0A3, 0xB1A7, 0xB2A7, 0xB3A3, 0xB4A7, 0xB5A3, 0xB6A3, 0xB7A7, 0xB8AF,
    0xB9AB, 0xB4A7, 0xB5A3, 0xB6A3, 0xB7A7, 0xB8AF, 0xB9AB, 0xC087, 0xC183,
    0xC283, 0xC387, 0xC483, 0xC587, 0xC687, 0xC783, 0xC88B, 0xC98F, 0xC483,
    0xC587, 0xC687, 0xC783, 0xC88B, 0xC98F, 0xD083, 0xD187, 0xD287, 0xD383,
    0xD487, 0xD583, 0xD683, 0xD787, 0xD88F, 0xD98B, 0xD487, 0xD583, 0xD683,
    0xD787, 0xD88F, 0xD98B, 0xE0A3, 0xE1A7, 0xE2A7, 0xE3A3, 0xE4A7, 0xE5A3,
    0xE6A3, 0xE7A7, 0xE8AF, 0xE9AB, 0xE4A7, 0xE5A3, 0xE6A3, 0xE7A7, 0xE8AF,
    0xE9AB, 0xF0A7, 0xF1A3, 0xF2A3, 0xF3A7, 0xF4A3, 0xF5A7, 0xF6A7, 0xF7A3,
    0xF8AB, 0xF9AF, 0xF4A3, 0xF5A7, 0xF6A7, 0xF7A3, 0xF8AB, 0xF9AF, 0x0047,
    0x0103, 0x0203, 0x0307, 0x0403, 0x0507, 0x0607, 0x0703, 0x080B, 0x090F,
    0x0403, 0x0507, 0x0607, 0x0703, 0x080B, 0x090F, 0x1003, 0x1107, 0x1207,
    0x1303, 0x1407, 0x1503, 0x1603, 0x1707, 0x180F, 0x190B, 0x1407, 0x1503,
    0x1603, 0x1707, 0x180F, 0x190B, 0x2023, 0x2127, 0x2227, 0x2323, 0x2427,
    0x2523, 0x2623, 0x2727, 0x282F, 0x292B, 0x2427, 0x2523, 0x2623, 0x2727,
    0x282F, 0x292B, 0x3027, 0x3123, 0x3223, 0x3327, 0x3423, 0x3527, 0x3627,
    0x3723, 0x382B, 0x392F, 0x3423, 0x3527, 0x3627, 0x3723, 0x382B, 0x392F,
    0x4003, 0x4107, 0x4207, 0x4303, 0x4407, 0x4503, 0x4603, 0x4707, 0x480F,
    0x490B, 0x4407, 0x4503, 0x4603, 0x4707, 0x480F, 0x490B, 0x5007, 0x5103,
    0x5203, 0x5307, 0x5403, 0x5507, 0x5607, 0x5703, 0x580B, 0x590F, 0x5403,
    0x5507, 0x5607, 0x5703, 0x580B, 0x590F, 0x6027, 0x6123, 0x6223, 0x6327,
    0x6423, 0x6527, 0x6627, 0x6723, 0x682B, 0x692F, 0x6423, 0x6527, 0x6627,
    0x6723, 0x682B, 0x692F, 0x7023, 0x7127, 0x7227, 0x7323, 0x7427, 0x7523,
    0x7623, 0x7727, 0x782F, 0x792B, 0x7427, 0x7523, 0x7623, 0x7727, 0x782F,
    0x792B, 0x8083, 0x8187, 0x8287, 0x8383, 0x8487, 0x8583, 0x8683, 0x8787,
    0x888F, 0x898B, 0x8487, 0x8583, 0x8683, 0x8787, 0x888F, 0x898B, 0x9087,
    0x9183, 0x9283, 0x9387, 0x9483, 0x9587, 0x9687, 0x9783, 0x988B, 0x998F,
    0x9483, 0x9587, 0x9687, 0x9783, 0x988B, 0x998F, 0xFABE, 0xFBBA, 0xFCBE,
    0xFDBA, 0xFEBA, 0xFFBE, 0x0046, 0x0102, 0x0202, 0x0306, 0x0402, 0x0506,
    0x0606, 0x0702, 0x080A, 0x090E, 0x0A1E, 0x0B1A, 0x0C1E, 0x0D1A, 0x0E1A,
    0x0F1E, 0x1002, 0x1106, 0x1206, 0x1302, 0x1406, 0x1502, 0x1602, 0x1706,
    0x180E, 0x190A, 0x1A1A, 0x1B1E, 0x1C1A, 0x1D1E, 0x1E1E, 0x1F1A, 0x2022,
    0x2126, 0x2226, 0x2322, 0x2426, 0x2522, 0x2622, 0x2726, 0x282E, 0x292A,
    0x2A3A, 0x2B3E, 0x2C3A, 0x2D3E, 0x2E3E, 0x2F3A, 0x3026, 0x3122, 0x3222,
    0x3326, 0x3422, 0x3526, 0x3626, 0x3722, 0x382A, 0x392E, 0x3A3E, 0x3B3A,
    0x3C3E, 0x3D3A, 0x3E3A, 0x3F3E, 0x4002, 0x4106, 0x4206, 0x4302, 0x4406,
    0x4502, 0x4602, 0x4706, 0x480E, 0x490A, 0x4A1A, 0x4B1E, 0x4C1A, 0x4D1E,
    0x4E1E, 0x4F1A, 0x5006, 0x5102, 0x5202, 0x5306, 0x5402, 0x5506, 0x5606,
    0x5702, 0x580A, 0x590E, 0x5A1E, 0x5B1A, 0x5C1E, 0x5D1A, 0x5E1A, 0x5F1E,
    0x6026, 0x6122, 0x6222, 0x6326, 0x6422, 0x6526, 0x6626, 0x6722, 0x682A,
    0x692E, 0x6A3E, 0x6B3A, 0x6C3E, 0x6D3A, 0x6E3A, 0x6F3E, 0x7022, 0x7126,
    0x7226, 0x7322, 0x7426, 0x7522, 0x7622, 0x7726, 0x782E, 0x792A, 0x7A3A,
    0x7B3E, 0x7C3A, 0x7D3E, 0x7E3E, 0x7F3A, 0x8082, 0x8186, 0x8286, 0x8382,
    0x8486, 0x8582, 0x8682, 0x8786, 0x888E, 0x898A, 0x8A9A, 0x8B9E, 0x8C9A,
    0x8D9E, 0x8E9E, 0x8F9A, 0x9086, 0x9182, 0x9282, 0x9386, 0x3423, 0x3527,
    0x3627, 0x3723, 0x382B, 0x392F, 0x3A3F, 0x3B3B, 0x3C3F, 0x3D3B, 0x3E3B,
    0x3F3F, 0x4003, 0x4107, 0x4207, 0x4303, 0x4407, 0x4503, 0x4603, 0x4707,
    0x480F, 0x490B, 0x4A1B, 0x4B1F, 0x4C1B, 0x4D1F, 0x4E1F, 0x4F1B, 0x5007,
    0x5103, 0x5203, 0x5307, 0x5403, 0x5507, 0x5607, 0x5703, 0x580B, 0x590F,
    0x5A1F, 0x5B1B, 0x5C1F, 0x5D1B, 0x5E1B, 0x5F1F, 0x6027, 0x6123, 0x6223,
    0x6327, 0x6423, 0x6527, 0x6627, 0x6723, 0x682B, 0x692F, 0x6A3F, 0x6B3B,
    0x6C3F, 0x6D3B, 0x6E3B, 0x6F3F, 0x7023, 0x7127, 0x7227, 0x7323, 0x7427,
    0x7523, 0x7623, 0x7727, 0x782F, 0x792B, 0x7A3B, 0x7B3F, 0x7C3B, 0x7D3F,
    0x7E3F, 0x7F3B, 0x8083, 0x8187, 0x8287, 0x8383, 0x8487, 0x8583, 0x8683,
    0x8787, 0x888F, 0x898B, 0x8A9B, 0x8B9F, 0x8C9B, 0x8D9F, 0x8E9F, 0x8F9B,
    0x9087, 0x9183, 0x9283, 0x9387, 0x9483, 0x9587, 0x9687, 0x9783, 0x988B,
    0x998F, 0x9A9F, 0x9B9B, 0x9C9F, 0x9D9B, 0x9E9B, 0x9F9F, 0xA0A7, 0xA1A3,
    0xA2A3, 0xA3A7, 0xA4A3, 0xA5A7, 0xA6A7, 0xA7A3, 0xA8AB, 0xA9AF, 0xAABF,
    0xABBB, 0xACBF, 0xADBB, 0xAEBB, 0xAFBF, 0xB0A3, 0xB1A7, 0xB2A7, 0xB3A3,
    0xB4A7, 0xB5A3, 0xB6A3, 0xB7A7, 0xB8AF, 0xB9AB, 0xBABB, 0xBBBF, 0xBCBB,
    0xBDBF, 0xBEBF, 0xBFBB, 0xC087, 0xC183, 0xC283, 0xC387, 0xC483, 0xC587,
    0xC687, 0xC783, 0xC88B, 0xC98F, 0xCA9F, 0xCB9B, 0xCC9F, 0xCD9B, 0xCE9B,
    0xCF9F, 0xD083, 0xD187, 0xD287, 0xD383, 0xD487, 0xD583, 0xD683, 0xD787,
    0xD88F, 0xD98B, 0xDA9B, 0xDB9F, 0xDC9B, 0xDD9F, 0xDE9F, 0xDF9B, 0xE0A3,
    0xE1A7, 0xE2A7, 0xE3A3, 0xE4A7, 0xE5A3, 0xE6A3, 0xE7A7, 0xE8AF, 0xE9AB,
    0xEABB, 0xEBBF, 0xECBB, 0xEDBF, 0xEEBF, 0xEFBB, 0xF0A7, 0xF1A3, 0xF2A3,
    0xF3A7, 0xF4A3, 0xF5A7, 0xF6A7, 0xF7A3, 0xF8AB, 0xF9AF, 0xFABF, 0xFBBB,
    0xFCBF, 0xFDBB, 0xFEBB, 0xFFBF, 0x0047, 0x0103, 0x0203, 0x0307, 0x0403,
    0x0507, 0x0607, 0x0703, 0x080B, 0x090F, 0x0A1F, 0x0B1B, 0x0C1F, 0x0D1B,
    0x0E1B, 0x0F1F, 0x1003, 0x1107, 0x1207, 0x1303, 0x1407, 0x1503, 0x1603,
    0x1707, 0x180F, 0x190B, 0x1A1B, 0x1B1F, 0x1C1B, 0x1D1F, 0x1E1F, 0x1F1B,
    0x2023, 0x2127, 0x2227, 0x2323, 0x2427, 0x2523, 0x2623, 0x2727, 0x282F,
    0x292B, 0x2A3B, 0x2B3F, 0x2C3B, 0x2D3F, 0x2E3F, 0x2F3B, 0x3027, 0x3123,
    0x3223, 0x3327, 0x3423, 0x3527, 0x3627, 0x3723, 0x382B, 0x392F, 0x3A3F,
    0x3B3B, 0x3C3F, 0x3D3B, 0x3E3B, 0x3F3F, 0x4003, 0x4107, 0x4207, 0x4303,
    0x4407, 0x4503, 0x4603, 0x4707, 0x480F, 0x490B, 0x4A1B, 0x4B1F, 0x4C1B,
    0x4D1F, 0x4E1F, 0x4F1B, 0x5007, 0x5103, 0x5203, 0x5307, 0x5403, 0x5507,
    0x5607, 0x5703, 0x580B, 0x590F, 0x5A1F, 0x5B1B, 0x5C1F, 0x5D1B, 0x5E1B,
    0x5F1F, 0x6027, 0x6123, 0x6223, 0x6327, 0x6423, 0x6527, 0x6627, 0x6723,
    0x682B, 0x692F, 0x6A3F, 0x6B3B, 0x6C3F, 0x6D3B, 0x6E3B, 0x6F3F, 0x7023,
    0x7127, 0x7227, 0x7323, 0x7427, 0x7523, 0x7623, 0x7727, 0x782F, 0x792B,
    0x7A3B, 0x7B3F, 0x7C3B, 0x7D3F, 0x7E3F, 0x7F3B, 0x8083, 0x8187, 0x8287,
    0x8383, 0x8487, 0x8583, 0x8683, 0x8787, 0x888F, 0x898B, 0x8A9B, 0x8B9F,
    0x8C9B, 0x8D9F, 0x8E9F, 0x8F9B, 0x9087, 0x9183, 0x9283, 0x9387, 0x9483,
    0x9587, 0x9687, 0x9783, 0x988B, 0x998F};

inline tUBYTE Z80::RES(tUBYTE bit, tUBYTE val) { return val & ~(1 << bit); }

inline tUBYTE Z80::RLC(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x80) ? Cflag : 0;
    res = ((res << 1) | (res >> 7)) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::RL(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x80) ? Cflag : 0;
    res = ((res << 1) | (z80.AF.b.l & Cflag)) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::RRC(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x01) ? Cflag : 0;
    res = ((res >> 1) | (res << 7)) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::RR(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x01) ? Cflag : 0;
    res = ((res >> 1) | (z80.AF.b.l << 7)) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::SET(tUBYTE bit, tUBYTE val) { return val | (1 << bit); }

inline tUBYTE Z80::SLA(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x80) ? Cflag : 0;
    res = (res << 1) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::SLL(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x80) ? Cflag : 0;
    res = ((res << 1) | 0x01) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::SRA(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x01) ? Cflag : 0;
    res = ((res >> 1) | (res & 0x80)) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

inline tUBYTE Z80::SRL(tUBYTE val)
{
    unsigned res = val;
    unsigned carry = (res & 0x01) ? Cflag : 0;
    res = (res >> 1) & 0xff;
    z80.AF.b.l = SZP[res] | carry;
    return res;
}

Z80::Z80() { init(); }

void Z80::init()
{
    iCycleCount = 0;
    iCycleCountSum = 0;
    iCycleCountInit = 0;
    iWSAdjust = 0;
    dwMF2ExitAddr = 0xffffffff;
    dwMF2Flags = 0;
    mStop = false;

    z80.init();

    init_tables();
}

void Z80::init_tables()
{
    int i, p;

    for (i = 0; i < 256; i++)
    {
        p = 0;
        if (i & 0x01)
            ++p;
        if (i & 0x02)
            ++p;
        if (i & 0x04)
            ++p;
        if (i & 0x08)
            ++p;
        if (i & 0x10)
            ++p;
        if (i & 0x20)
            ++p;
        if (i & 0x40)
            ++p;
        if (i & 0x80)
            ++p;

        SZ[i] = i ? i & Sflag : Zflag;
        SZ[i] |= (i & Xflags);
        SZ_BIT[i] = i ? i & Sflag : Zflag | Pflag;
        SZ_BIT[i] |= (i & Xflags);
        SZP[i] = SZ[i] | ((p & 1) ? 0 : Pflag);
        SZHV_inc[i] = SZ[i];
        if (i == 0x80)
            SZHV_inc[i] |= Vflag;
        if ((i & 0x0f) == 0x00)
            SZHV_inc[i] |= Hflag;
        SZHV_dec[i] = SZ[i] | Nflag;
        if (i == 0x7f)
            SZHV_dec[i] |= Vflag;
        if ((i & 0x0f) == 0x0f)
            SZHV_dec[i] |= Hflag;
    }
}

void Z80::mf2stop()
{
    z80.R++;
    z80.IFF1 = 0;
    z80.EI_issued = 0;
    iCycleCount = 20;
    if (iWSAdjust)
        iCycleCount -= 4;
    dwMF2ExitAddr = z80.PC.d;
    RST(0x0066); // MF2 stop button causes a Z80 NMI
    z80_wait_states dwMF2Flags = MF2_ACTIVE | MF2_RUNNING;
}

int Z80::execute(int cc)
{
    register tUBYTE bOpCode;

    iCycleCountInit = cc;
    iCycleCountSum = 0;

    while (z80.PC.d != z80.break_point) // loop until break point
    {

        if (dwMF2Flags & MF2_RUNNING)
        {
            if (z80.PC.d == dwMF2ExitAddr) // have we returned from the MF2?
            {
                dwMF2Flags = MF2_INVISIBLE; // clear running flag and make the
                                            // MF2 'invisible'
            }
        }

        bOpCode = read_mem(z80.PC.w.l++);
        iCycleCount = cc_op[bOpCode];
        z80.R++;
        switch (bOpCode)
        {
            case adc_a:
                ADC(z80.AF.b.h);
                break;
            case adc_b:
                ADC(z80.BC.b.h);
                break;
            case adc_byte:
                ADC(read_mem(z80.PC.w.l++));
                break;
            case adc_c:
                ADC(z80.BC.b.l);
                break;
            case adc_d:
                ADC(z80.DE.b.h);
                break;
            case adc_e:
                ADC(z80.DE.b.l);
                break;
            case adc_h:
                ADC(z80.HL.b.h);
                break;
            case adc_l:
                ADC(z80.HL.b.l);
                break;
            case adc_mhl:
                ADC(read_mem(z80.HL.w.l));
                break;
            case add_a:
                ADD(z80.AF.b.h);
                break;
            case add_b:
                ADD(z80.BC.b.h);
                break;
            case add_byte:
                ADD(read_mem(z80.PC.w.l++));
                break;
            case add_c:
                ADD(z80.BC.b.l);
                break;
            case add_d:
                ADD(z80.DE.b.h);
                break;
            case add_e:
                ADD(z80.DE.b.l);
                break;
            case add_h:
                ADD(z80.HL.b.h);
                break;
            case add_hl_bc:
                ADD16(HL, BC);
                break;
            case add_hl_de:
                ADD16(HL, DE);
                break;
            case add_hl_hl:
                ADD16(HL, HL);
                break;
            case add_hl_sp:
                ADD16(HL, SP);
                break;
            case add_l:
                ADD(z80.HL.b.l);
                break;
            case add_mhl:
                ADD(read_mem(z80.HL.w.l));
                break;
            case and_a:
                AND(z80.AF.b.h);
                break;
            case and_b:
                AND(z80.BC.b.h);
                break;
            case and_byte:
                AND(read_mem(z80.PC.w.l++));
                break;
            case and_c:
                AND(z80.BC.b.l);
                break;
            case and_d:
                AND(z80.DE.b.h);
                break;
            case and_e:
                AND(z80.DE.b.l);
                break;
            case and_h:
                AND(z80.HL.b.h);
                break;
            case and_l:
                AND(z80.HL.b.l);
                break;
            case and_mhl:
                AND(read_mem(z80.HL.w.l));
                break;
            case call:
                CALL;
                break;
            case call_c:
                if (z80.AF.b.l & Cflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_m:
                if (z80.AF.b.l & Sflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_nc:
                if (!(z80.AF.b.l & Cflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_nz:
                if (!(z80.AF.b.l & Zflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_p:
                if (!(z80.AF.b.l & Sflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_pe:
                if (z80.AF.b.l & Pflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_po:
                if (!(z80.AF.b.l & Pflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case call_z:
                if (z80.AF.b.l & Zflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    CALL
                }
                else
                {
                    z80.PC.w.l += 2;
                }
                break;
            case ccf:
                z80.AF.b.l =
                    ((z80.AF.b.l & (Sflag | Zflag | Pflag | Cflag)) |
                     ((z80.AF.b.l & CF) << 4) | (z80.AF.b.h & Xflags)) ^
                    CF;
                break;
            case cpl:
                z80.AF.b.h ^= 0xff;
                z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag | Cflag)) |
                             Hflag | Nflag | (z80.AF.b.h & Xflags);
                break;
            case cp_a:
                CP(z80.AF.b.h);
                break;
            case cp_b:
                CP(z80.BC.b.h);
                break;
            case cp_byte:
                CP(read_mem(z80.PC.w.l++));
                break;
            case cp_c:
                CP(z80.BC.b.l);
                break;
            case cp_d:
                CP(z80.DE.b.h);
                break;
            case cp_e:
                CP(z80.DE.b.l);
                break;
            case cp_h:
                CP(z80.HL.b.h);
                break;
            case cp_l:
                CP(z80.HL.b.l);
                break;
            case cp_mhl:
                CP(read_mem(z80.HL.w.l));
                break;
            case daa:
                DAA;
                break;
            case dec_a:
                DEC(z80.AF.b.h);
                break;
            case dec_b:
                DEC(z80.BC.b.h);
                break;
            case dec_bc:
                z80.BC.w.l--;
                iWSAdjust++;
                break;
            case dec_c:
                DEC(z80.BC.b.l);
                break;
            case dec_d:
                DEC(z80.DE.b.h);
                break;
            case dec_de:
                z80.DE.w.l--;
                iWSAdjust++;
                break;
            case dec_e:
                DEC(z80.DE.b.l);
                break;
            case dec_h:
                DEC(z80.HL.b.h);
                break;
            case dec_hl:
                z80.HL.w.l--;
                iWSAdjust++;
                break;
            case dec_l:
                DEC(z80.HL.b.l);
                break;
            case dec_mhl:
            {
                tUBYTE b = read_mem(z80.HL.w.l);
                DEC(b);
                write_mem(z80.HL.w.l, b);
            }
            break;
            case dec_sp:
                z80.SP.w.l--;
                iWSAdjust++;
                break;
            case di:
                z80.IFF1 = z80.IFF2 = 0;
                z80.EI_issued = 0;
                break;
            case djnz:
                if (--z80.BC.b.h)
                {
                    iCycleCount += cc_ex[bOpCode];
                    JR
                }
                else
                {
                    z80.PC.w.l++;
                }
                break;
            case ei:
                z80.EI_issued = 2;
                break;
            case exx:
                EXX;
                break;
            case ex_af_af:
                EX(z80.AF, z80.AFx);
                break;
            case ex_de_hl:
                EX(z80.DE, z80.HL);
                break;
            case ex_msp_hl:
                EX_SP(HL);
                iWSAdjust++;
                break;
            case halt:
                z80.HALT = 1;
                z80.PC.w.l--;
                break;
            case ina:
            {
                tREGPAIR p;
                p.b.l = read_mem(z80.PC.w.l++);
                p.b.h = z80.AF.b.h;
                z80.AF.b.h = z80_IN_handler(p);
            }
            break;
            case inc_a:
                INC(z80.AF.b.h);
                break;
            case inc_b:
                INC(z80.BC.b.h);
                break;
            case inc_bc:
                z80.BC.w.l++;
                iWSAdjust++;
                break;
            case inc_c:
                INC(z80.BC.b.l);
                break;
            case inc_d:
                INC(z80.DE.b.h);
                break;
            case inc_de:
                z80.DE.w.l++;
                iWSAdjust++;
                break;
            case inc_e:
                INC(z80.DE.b.l);
                break;
            case inc_h:
                INC(z80.HL.b.h);
                break;
            case inc_hl:
                z80.HL.w.l++;
                iWSAdjust++;
                break;
            case inc_l:
                INC(z80.HL.b.l);
                break;
            case inc_mhl:
            {
                tUBYTE b = read_mem(z80.HL.w.l);
                INC(b);
                write_mem(z80.HL.w.l, b);
            }
            break;
            case inc_sp:
                z80.SP.w.l++;
                iWSAdjust++;
                break;
            case jp:
                JP;
                break;
            case jp_c:
                if (z80.AF.b.l & Cflag)
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_m:
                if (z80.AF.b.l & Sflag)
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_nc:
                if (!(z80.AF.b.l & Cflag))
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_nz:
                if (!(z80.AF.b.l & Zflag))
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_p:
                if (!(z80.AF.b.l & Sflag))
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_pe:
                if (z80.AF.b.l & Pflag)
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_po:
                if (!(z80.AF.b.l & Pflag))
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jp_z:
                if (z80.AF.b.l & Zflag)
                {
                    JP
                }
                else
                {
                    z80.PC.w.l += 2;
                };
                break;
            case jr:
                JR;
                break;
            case jr_c:
                if (z80.AF.b.l & Cflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    JR
                }
                else
                {
                    z80.PC.w.l++;
                };
                break;
            case jr_nc:
                if (!(z80.AF.b.l & Cflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    JR
                }
                else
                {
                    z80.PC.w.l++;
                };
                break;
            case jr_nz:
                if (!(z80.AF.b.l & Zflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    JR
                }
                else
                {
                    z80.PC.w.l++;
                };
                break;
            case jr_z:
                if (z80.AF.b.l & Zflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    JR
                }
                else
                {
                    z80.PC.w.l++;
                };
                break;
            case ld_a_a:
                break;
            case ld_a_b:
                z80.AF.b.h = z80.BC.b.h;
                break;
            case ld_a_byte:
                z80.AF.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_a_c:
                z80.AF.b.h = z80.BC.b.l;
                break;
            case ld_a_d:
                z80.AF.b.h = z80.DE.b.h;
                break;
            case ld_a_e:
                z80.AF.b.h = z80.DE.b.l;
                break;
            case ld_a_h:
                z80.AF.b.h = z80.HL.b.h;
                break;
            case ld_a_l:
                z80.AF.b.h = z80.HL.b.l;
                break;
            case ld_a_mbc:
                z80.AF.b.h = read_mem(z80.BC.w.l);
                break;
            case ld_a_mde:
                z80.AF.b.h = read_mem(z80.DE.w.l);
                break;
            case ld_a_mhl:
                z80.AF.b.h = read_mem(z80.HL.w.l);
                break;
            case ld_a_mword:
            {
                tREGPAIR addr;
                addr.b.l = read_mem(z80.PC.w.l++);
                addr.b.h = read_mem(z80.PC.w.l++);
                z80.AF.b.h = read_mem(addr.w.l);
            }
            break;
            case ld_bc_word:
                z80.BC.b.l = read_mem(z80.PC.w.l++);
                z80.BC.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_b_a:
                z80.BC.b.h = z80.AF.b.h;
                break;
            case ld_b_b:
                break;
            case ld_b_byte:
                z80.BC.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_b_c:
                z80.BC.b.h = z80.BC.b.l;
                break;
            case ld_b_d:
                z80.BC.b.h = z80.DE.b.h;
                break;
            case ld_b_e:
                z80.BC.b.h = z80.DE.b.l;
                break;
            case ld_b_h:
                z80.BC.b.h = z80.HL.b.h;
                break;
            case ld_b_l:
                z80.BC.b.h = z80.HL.b.l;
                break;
            case ld_b_mhl:
                z80.BC.b.h = read_mem(z80.HL.w.l);
                break;
            case ld_c_a:
                z80.BC.b.l = z80.AF.b.h;
                break;
            case ld_c_b:
                z80.BC.b.l = z80.BC.b.h;
                break;
            case ld_c_byte:
                z80.BC.b.l = read_mem(z80.PC.w.l++);
                break;
            case ld_c_c:
                break;
            case ld_c_d:
                z80.BC.b.l = z80.DE.b.h;
                break;
            case ld_c_e:
                z80.BC.b.l = z80.DE.b.l;
                break;
            case ld_c_h:
                z80.BC.b.l = z80.HL.b.h;
                break;
            case ld_c_l:
                z80.BC.b.l = z80.HL.b.l;
                break;
            case ld_c_mhl:
                z80.BC.b.l = read_mem(z80.HL.w.l);
                break;
            case ld_de_word:
                z80.DE.b.l = read_mem(z80.PC.w.l++);
                z80.DE.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_d_a:
                z80.DE.b.h = z80.AF.b.h;
                break;
            case ld_d_b:
                z80.DE.b.h = z80.BC.b.h;
                break;
            case ld_d_byte:
                z80.DE.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_d_c:
                z80.DE.b.h = z80.BC.b.l;
                break;
            case ld_d_d:
                break;
            case ld_d_e:
                z80.DE.b.h = z80.DE.b.l;
                break;
            case ld_d_h:
                z80.DE.b.h = z80.HL.b.h;
                break;
            case ld_d_l:
                z80.DE.b.h = z80.HL.b.l;
                break;
            case ld_d_mhl:
                z80.DE.b.h = read_mem(z80.HL.w.l);
                break;
            case ld_e_a:
                z80.DE.b.l = z80.AF.b.h;
                break;
            case ld_e_b:
                z80.DE.b.l = z80.BC.b.h;
                break;
            case ld_e_byte:
                z80.DE.b.l = read_mem(z80.PC.w.l++);
                break;
            case ld_e_c:
                z80.DE.b.l = z80.BC.b.l;
                break;
            case ld_e_d:
                z80.DE.b.l = z80.DE.b.h;
                break;
            case ld_e_e:
                break;
            case ld_e_h:
                z80.DE.b.l = z80.HL.b.h;
                break;
            case ld_e_l:
                z80.DE.b.l = z80.HL.b.l;
                break;
            case ld_e_mhl:
                z80.DE.b.l = read_mem(z80.HL.w.l);
                break;
            case ld_hl_mword:
                LD16_MEM(HL);
                break;
            case ld_hl_word:
                z80.HL.b.l = read_mem(z80.PC.w.l++);
                z80.HL.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_h_a:
                z80.HL.b.h = z80.AF.b.h;
                break;
            case ld_h_b:
                z80.HL.b.h = z80.BC.b.h;
                break;
            case ld_h_byte:
                z80.HL.b.h = read_mem(z80.PC.w.l++);
                break;
            case ld_h_c:
                z80.HL.b.h = z80.BC.b.l;
                break;
            case ld_h_d:
                z80.HL.b.h = z80.DE.b.h;
                break;
            case ld_h_e:
                z80.HL.b.h = z80.DE.b.l;
                break;
            case ld_h_h:
                break;
            case ld_h_l:
                z80.HL.b.h = z80.HL.b.l;
                break;
            case ld_h_mhl:
                z80.HL.b.h = read_mem(z80.HL.w.l);
                break;
            case ld_l_a:
                z80.HL.b.l = z80.AF.b.h;
                break;
            case ld_l_b:
                z80.HL.b.l = z80.BC.b.h;
                break;
            case ld_l_byte:
                z80.HL.b.l = read_mem(z80.PC.w.l++);
                break;
            case ld_l_c:
                z80.HL.b.l = z80.BC.b.l;
                break;
            case ld_l_d:
                z80.HL.b.l = z80.DE.b.h;
                break;
            case ld_l_e:
                z80.HL.b.l = z80.DE.b.l;
                break;
            case ld_l_h:
                z80.HL.b.l = z80.HL.b.h;
                break;
            case ld_l_l:
                break;
            case ld_l_mhl:
                z80.HL.b.l = read_mem(z80.HL.w.l);
                break;
            case ld_mbc_a:
                write_mem(z80.BC.w.l, z80.AF.b.h);
                break;
            case ld_mde_a:
                write_mem(z80.DE.w.l, z80.AF.b.h);
                break;
            case ld_mhl_a:
                write_mem(z80.HL.w.l, z80.AF.b.h);
                break;
            case ld_mhl_b:
                write_mem(z80.HL.w.l, z80.BC.b.h);
                break;
            case ld_mhl_byte:
            {
                tUBYTE b = read_mem(z80.PC.w.l++);
                write_mem(z80.HL.w.l, b);
            }
            break;
            case ld_mhl_c:
                write_mem(z80.HL.w.l, z80.BC.b.l);
                break;
            case ld_mhl_d:
                write_mem(z80.HL.w.l, z80.DE.b.h);
                break;
            case ld_mhl_e:
                write_mem(z80.HL.w.l, z80.DE.b.l);
                break;
            case ld_mhl_h:
                write_mem(z80.HL.w.l, z80.HL.b.h);
                break;
            case ld_mhl_l:
                write_mem(z80.HL.w.l, z80.HL.b.l);
                break;
            case ld_mword_a:
            {
                tREGPAIR addr;
                addr.b.l = read_mem(z80.PC.w.l++);
                addr.b.h = read_mem(z80.PC.w.l++);
                write_mem(addr.w.l, z80.AF.b.h);
            }
            break;
            case ld_mword_hl:
                LDMEM_16(HL);
                break;
            case ld_pc_hl:
                z80.PC.w.l = z80.HL.w.l;
                break;
            case ld_sp_hl:
                z80.SP.w.l = z80.HL.w.l;
                iWSAdjust++;
                break;
            case ld_sp_word:
                z80.SP.b.l = read_mem(z80.PC.w.l++);
                z80.SP.b.h = read_mem(z80.PC.w.l++);
                break;
            case nop:
                break;
            case or_a:
                OR(z80.AF.b.h);
                break;
            case or_b:
                OR(z80.BC.b.h);
                break;
            case or_byte:
                OR(read_mem(z80.PC.w.l++));
                break;
            case or_c:
                OR(z80.BC.b.l);
                break;
            case or_d:
                OR(z80.DE.b.h);
                break;
            case or_e:
                OR(z80.DE.b.l);
                break;
            case or_h:
                OR(z80.HL.b.h);
                break;
            case or_l:
                OR(z80.HL.b.l);
                break;
            case or_mhl:
                OR(read_mem(z80.HL.w.l));
                break;
            case outa:
            {
                tREGPAIR p;
                p.b.l = read_mem(z80.PC.w.l++);
                p.b.h = z80.AF.b.h;
                z80_OUT_handler(p, z80.AF.b.h);
            }
            break;
            case pfx_cb:
                z80_pfx_cb();
                break;
            case pfx_dd:
                z80_pfx_dd();
                break;
            case pfx_ed:
                z80_pfx_ed();
                break;
            case pfx_fd:
                z80_pfx_fd();
                break;
            case pop_af:
                POP(AF);
                break;
            case pop_bc:
                POP(BC);
                break;
            case pop_de:
                POP(DE);
                break;
            case pop_hl:
                POP(HL);
                break;
            case push_af:
                PUSH(AF);
                break;
            case push_bc:
                PUSH(BC);
                break;
            case push_de:
                PUSH(DE);
                break;
            case push_hl:
                PUSH(HL);
                break;
            case ret:
                RET;
                break;
            case ret_c:
                if (z80.AF.b.l & Cflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_m:
                if (z80.AF.b.l & Sflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_nc:
                if (!(z80.AF.b.l & Cflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_nz:
                if (!(z80.AF.b.l & Zflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_p:
                if (!(z80.AF.b.l & Sflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_pe:
                if (z80.AF.b.l & Pflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_po:
                if (!(z80.AF.b.l & Pflag))
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case ret_z:
                if (z80.AF.b.l & Zflag)
                {
                    iCycleCount += cc_ex[bOpCode];
                    RET
                }
                else
                {
                    iWSAdjust++;
                };
                break;
            case rla:
                RLA;
                break;
            case rlca:
                RLCA;
                break;
            case rra:
                RRA;
                break;
            case rrca:
                RRCA;
                break;
            case rst00:
                RST(0x0000);
                break;
            case rst08:
                RST(0x0008);
                break;
            case rst10:
                RST(0x0010);
                break;
            case rst18:
                RST(0x0018);
                break;
            case rst20:
                RST(0x0020);
                break;
            case rst28:
                RST(0x0028);
                break;
            case rst30:
                RST(0x0030);
                break;
            case rst38:
                RST(0x0038);
                break;
            case sbc_a:
                SBC(z80.AF.b.h);
                break;
            case sbc_b:
                SBC(z80.BC.b.h);
                break;
            case sbc_byte:
                SBC(read_mem(z80.PC.w.l++));
                break;
            case sbc_c:
                SBC(z80.BC.b.l);
                break;
            case sbc_d:
                SBC(z80.DE.b.h);
                break;
            case sbc_e:
                SBC(z80.DE.b.l);
                break;
            case sbc_h:
                SBC(z80.HL.b.h);
                break;
            case sbc_l:
                SBC(z80.HL.b.l);
                break;
            case sbc_mhl:
                SBC(read_mem(z80.HL.w.l));
                break;
            case scf:
                z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | Cflag |
                             (z80.AF.b.h & Xflags);
                break;
            case sub_a:
                SUB(z80.AF.b.h);
                break;
            case sub_b:
                SUB(z80.BC.b.h);
                break;
            case sub_byte:
                SUB(read_mem(z80.PC.w.l++));
                break;
            case sub_c:
                SUB(z80.BC.b.l);
                break;
            case sub_d:
                SUB(z80.DE.b.h);
                break;
            case sub_e:
                SUB(z80.DE.b.l);
                break;
            case sub_h:
                SUB(z80.HL.b.h);
                break;
            case sub_l:
                SUB(z80.HL.b.l);
                break;
            case sub_mhl:
                SUB(read_mem(z80.HL.w.l));
                break;
            case xor_a:
                XOR(z80.AF.b.h);
                break;
            case xor_b:
                XOR(z80.BC.b.h);
                break;
            case xor_byte:
                XOR(read_mem(z80.PC.w.l++));
                break;
            case xor_c:
                XOR(z80.BC.b.l);
                break;
            case xor_d:
                XOR(z80.DE.b.h);
                break;
            case xor_e:
                XOR(z80.DE.b.l);
                break;
            case xor_h:
                XOR(z80.HL.b.h);
                break;
            case xor_l:
                XOR(z80.HL.b.l);
                break;
            case xor_mhl:
                XOR(read_mem(z80.HL.w.l));
                break;
        }

        z80_wait_states

            if (z80.EI_issued) // EI 'delay' in effect?
        {
            if (--z80.EI_issued == 0)
            {
                z80.IFF1 = z80.IFF2 = Pflag; // set interrupt flip-flops
                if (z80.int_pending)
                    z80_int_handler
            }
        }
        else if (z80.int_pending) z80_int_handler // any interrupts pending?

            iWSAdjust = 0;

        if (cpc.sound().bufferFull())
        {
            return iCycleCountSum;
        } // @todo put this in STOP

        if (mStop) // video emulation finished building frame?
        {
            mStop = false;
            return iCycleCountSum; // exit emulation loop
        }
        else if (iCycleCountSum >= iCycleCountInit)
            return iCycleCountSum;

        /*
        if (z80.trace)                  // tracing instructions?
        {
            z80.trace = 0;              // reset trace condition
            return EC_TRACE;            // exit emulation loop
        }
        else if (vdu.frameCompleted())  // video emulation finished building
        frame?
        {
            return ECz80.AF.b.lRAMEz80.BC.b.lOMPLETE;   // exit emulation loop
        }
        else if (cpc.cycleCount() <= 0) // emulation loop ran for one frame?
        {
            return ECz80.BC.b.lYCLEz80.BC.b.lOUNT;      // exit emulation loop
        }
        */
    }
    return EC_BREAKPOINT;
}

void Z80::z80_pfx_cb()
{
    register tUBYTE bOpCode;

    bOpCode = read_mem(z80.PC.w.l++);
    iCycleCount += cc_cb[bOpCode];
    z80.R++;
    switch (bOpCode)
    {
        case bit0_a:
            BIT(0, z80.AF.b.h);
            break;
        case bit0_b:
            BIT(0, z80.BC.b.h);
            break;
        case bit0_c:
            BIT(0, z80.BC.b.l);
            break;
        case bit0_d:
            BIT(0, z80.DE.b.h);
            break;
        case bit0_e:
            BIT(0, z80.DE.b.l);
            break;
        case bit0_h:
            BIT(0, z80.HL.b.h);
            break;
        case bit0_l:
            BIT(0, z80.HL.b.l);
            break;
        case bit0_mhl:
            BIT(0, read_mem(z80.HL.w.l));
            break;
        case bit1_a:
            BIT(1, z80.AF.b.h);
            break;
        case bit1_b:
            BIT(1, z80.BC.b.h);
            break;
        case bit1_c:
            BIT(1, z80.BC.b.l);
            break;
        case bit1_d:
            BIT(1, z80.DE.b.h);
            break;
        case bit1_e:
            BIT(1, z80.DE.b.l);
            break;
        case bit1_h:
            BIT(1, z80.HL.b.h);
            break;
        case bit1_l:
            BIT(1, z80.HL.b.l);
            break;
        case bit1_mhl:
            BIT(1, read_mem(z80.HL.w.l));
            break;
        case bit2_a:
            BIT(2, z80.AF.b.h);
            break;
        case bit2_b:
            BIT(2, z80.BC.b.h);
            break;
        case bit2_c:
            BIT(2, z80.BC.b.l);
            break;
        case bit2_d:
            BIT(2, z80.DE.b.h);
            break;
        case bit2_e:
            BIT(2, z80.DE.b.l);
            break;
        case bit2_h:
            BIT(2, z80.HL.b.h);
            break;
        case bit2_l:
            BIT(2, z80.HL.b.l);
            break;
        case bit2_mhl:
            BIT(2, read_mem(z80.HL.w.l));
            break;
        case bit3_a:
            BIT(3, z80.AF.b.h);
            break;
        case bit3_b:
            BIT(3, z80.BC.b.h);
            break;
        case bit3_c:
            BIT(3, z80.BC.b.l);
            break;
        case bit3_d:
            BIT(3, z80.DE.b.h);
            break;
        case bit3_e:
            BIT(3, z80.DE.b.l);
            break;
        case bit3_h:
            BIT(3, z80.HL.b.h);
            break;
        case bit3_l:
            BIT(3, z80.HL.b.l);
            break;
        case bit3_mhl:
            BIT(3, read_mem(z80.HL.w.l));
            break;
        case bit4_a:
            BIT(4, z80.AF.b.h);
            break;
        case bit4_b:
            BIT(4, z80.BC.b.h);
            break;
        case bit4_c:
            BIT(4, z80.BC.b.l);
            break;
        case bit4_d:
            BIT(4, z80.DE.b.h);
            break;
        case bit4_e:
            BIT(4, z80.DE.b.l);
            break;
        case bit4_h:
            BIT(4, z80.HL.b.h);
            break;
        case bit4_l:
            BIT(4, z80.HL.b.l);
            break;
        case bit4_mhl:
            BIT(4, read_mem(z80.HL.w.l));
            break;
        case bit5_a:
            BIT(5, z80.AF.b.h);
            break;
        case bit5_b:
            BIT(5, z80.BC.b.h);
            break;
        case bit5_c:
            BIT(5, z80.BC.b.l);
            break;
        case bit5_d:
            BIT(5, z80.DE.b.h);
            break;
        case bit5_e:
            BIT(5, z80.DE.b.l);
            break;
        case bit5_h:
            BIT(5, z80.HL.b.h);
            break;
        case bit5_l:
            BIT(5, z80.HL.b.l);
            break;
        case bit5_mhl:
            BIT(5, read_mem(z80.HL.w.l));
            break;
        case bit6_a:
            BIT(6, z80.AF.b.h);
            break;
        case bit6_b:
            BIT(6, z80.BC.b.h);
            break;
        case bit6_c:
            BIT(6, z80.BC.b.l);
            break;
        case bit6_d:
            BIT(6, z80.DE.b.h);
            break;
        case bit6_e:
            BIT(6, z80.DE.b.l);
            break;
        case bit6_h:
            BIT(6, z80.HL.b.h);
            break;
        case bit6_l:
            BIT(6, z80.HL.b.l);
            break;
        case bit6_mhl:
            BIT(6, read_mem(z80.HL.w.l));
            break;
        case bit7_a:
            BIT(7, z80.AF.b.h);
            break;
        case bit7_b:
            BIT(7, z80.BC.b.h);
            break;
        case bit7_c:
            BIT(7, z80.BC.b.l);
            break;
        case bit7_d:
            BIT(7, z80.DE.b.h);
            break;
        case bit7_e:
            BIT(7, z80.DE.b.l);
            break;
        case bit7_h:
            BIT(7, z80.HL.b.h);
            break;
        case bit7_l:
            BIT(7, z80.HL.b.l);
            break;
        case bit7_mhl:
            BIT(7, read_mem(z80.HL.w.l));
            break;
        case res0_a:
            z80.AF.b.h = RES(0, z80.AF.b.h);
            break;
        case res0_b:
            z80.BC.b.h = RES(0, z80.BC.b.h);
            break;
        case res0_c:
            z80.BC.b.l = RES(0, z80.BC.b.l);
            break;
        case res0_d:
            z80.DE.b.h = RES(0, z80.DE.b.h);
            break;
        case res0_e:
            z80.DE.b.l = RES(0, z80.DE.b.l);
            break;
        case res0_h:
            z80.HL.b.h = RES(0, z80.HL.b.h);
            break;
        case res0_l:
            z80.HL.b.l = RES(0, z80.HL.b.l);
            break;
        case res0_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(0, b));
        }
        break;
        case res1_a:
            z80.AF.b.h = RES(1, z80.AF.b.h);
            break;
        case res1_b:
            z80.BC.b.h = RES(1, z80.BC.b.h);
            break;
        case res1_c:
            z80.BC.b.l = RES(1, z80.BC.b.l);
            break;
        case res1_d:
            z80.DE.b.h = RES(1, z80.DE.b.h);
            break;
        case res1_e:
            z80.DE.b.l = RES(1, z80.DE.b.l);
            break;
        case res1_h:
            z80.HL.b.h = RES(1, z80.HL.b.h);
            break;
        case res1_l:
            z80.HL.b.l = RES(1, z80.HL.b.l);
            break;
        case res1_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(1, b));
        }
        break;
        case res2_a:
            z80.AF.b.h = RES(2, z80.AF.b.h);
            break;
        case res2_b:
            z80.BC.b.h = RES(2, z80.BC.b.h);
            break;
        case res2_c:
            z80.BC.b.l = RES(2, z80.BC.b.l);
            break;
        case res2_d:
            z80.DE.b.h = RES(2, z80.DE.b.h);
            break;
        case res2_e:
            z80.DE.b.l = RES(2, z80.DE.b.l);
            break;
        case res2_h:
            z80.HL.b.h = RES(2, z80.HL.b.h);
            break;
        case res2_l:
            z80.HL.b.l = RES(2, z80.HL.b.l);
            break;
        case res2_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(2, b));
        }
        break;
        case res3_a:
            z80.AF.b.h = RES(3, z80.AF.b.h);
            break;
        case res3_b:
            z80.BC.b.h = RES(3, z80.BC.b.h);
            break;
        case res3_c:
            z80.BC.b.l = RES(3, z80.BC.b.l);
            break;
        case res3_d:
            z80.DE.b.h = RES(3, z80.DE.b.h);
            break;
        case res3_e:
            z80.DE.b.l = RES(3, z80.DE.b.l);
            break;
        case res3_h:
            z80.HL.b.h = RES(3, z80.HL.b.h);
            break;
        case res3_l:
            z80.HL.b.l = RES(3, z80.HL.b.l);
            break;
        case res3_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(3, b));
        }
        break;
        case res4_a:
            z80.AF.b.h = RES(4, z80.AF.b.h);
            break;
        case res4_b:
            z80.BC.b.h = RES(4, z80.BC.b.h);
            break;
        case res4_c:
            z80.BC.b.l = RES(4, z80.BC.b.l);
            break;
        case res4_d:
            z80.DE.b.h = RES(4, z80.DE.b.h);
            break;
        case res4_e:
            z80.DE.b.l = RES(4, z80.DE.b.l);
            break;
        case res4_h:
            z80.HL.b.h = RES(4, z80.HL.b.h);
            break;
        case res4_l:
            z80.HL.b.l = RES(4, z80.HL.b.l);
            break;
        case res4_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(4, b));
        }
        break;
        case res5_a:
            z80.AF.b.h = RES(5, z80.AF.b.h);
            break;
        case res5_b:
            z80.BC.b.h = RES(5, z80.BC.b.h);
            break;
        case res5_c:
            z80.BC.b.l = RES(5, z80.BC.b.l);
            break;
        case res5_d:
            z80.DE.b.h = RES(5, z80.DE.b.h);
            break;
        case res5_e:
            z80.DE.b.l = RES(5, z80.DE.b.l);
            break;
        case res5_h:
            z80.HL.b.h = RES(5, z80.HL.b.h);
            break;
        case res5_l:
            z80.HL.b.l = RES(5, z80.HL.b.l);
            break;
        case res5_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(5, b));
        }
        break;
        case res6_a:
            z80.AF.b.h = RES(6, z80.AF.b.h);
            break;
        case res6_b:
            z80.BC.b.h = RES(6, z80.BC.b.h);
            break;
        case res6_c:
            z80.BC.b.l = RES(6, z80.BC.b.l);
            break;
        case res6_d:
            z80.DE.b.h = RES(6, z80.DE.b.h);
            break;
        case res6_e:
            z80.DE.b.l = RES(6, z80.DE.b.l);
            break;
        case res6_h:
            z80.HL.b.h = RES(6, z80.HL.b.h);
            break;
        case res6_l:
            z80.HL.b.l = RES(6, z80.HL.b.l);
            break;
        case res6_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(6, b));
        }
        break;
        case res7_a:
            z80.AF.b.h = RES(7, z80.AF.b.h);
            break;
        case res7_b:
            z80.BC.b.h = RES(7, z80.BC.b.h);
            break;
        case res7_c:
            z80.BC.b.l = RES(7, z80.BC.b.l);
            break;
        case res7_d:
            z80.DE.b.h = RES(7, z80.DE.b.h);
            break;
        case res7_e:
            z80.DE.b.l = RES(7, z80.DE.b.l);
            break;
        case res7_h:
            z80.HL.b.h = RES(7, z80.HL.b.h);
            break;
        case res7_l:
            z80.HL.b.l = RES(7, z80.HL.b.l);
            break;
        case res7_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RES(7, b));
        }
        break;
        case rlc_a:
            z80.AF.b.h = RLC(z80.AF.b.h);
            break;
        case rlc_b:
            z80.BC.b.h = RLC(z80.BC.b.h);
            break;
        case rlc_c:
            z80.BC.b.l = RLC(z80.BC.b.l);
            break;
        case rlc_d:
            z80.DE.b.h = RLC(z80.DE.b.h);
            break;
        case rlc_e:
            z80.DE.b.l = RLC(z80.DE.b.l);
            break;
        case rlc_h:
            z80.HL.b.h = RLC(z80.HL.b.h);
            break;
        case rlc_l:
            z80.HL.b.l = RLC(z80.HL.b.l);
            break;
        case rlc_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RLC(b));
        }
        break;
        case rl_a:
            z80.AF.b.h = RL(z80.AF.b.h);
            break;
        case rl_b:
            z80.BC.b.h = RL(z80.BC.b.h);
            break;
        case rl_c:
            z80.BC.b.l = RL(z80.BC.b.l);
            break;
        case rl_d:
            z80.DE.b.h = RL(z80.DE.b.h);
            break;
        case rl_e:
            z80.DE.b.l = RL(z80.DE.b.l);
            break;
        case rl_h:
            z80.HL.b.h = RL(z80.HL.b.h);
            break;
        case rl_l:
            z80.HL.b.l = RL(z80.HL.b.l);
            break;
        case rl_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RL(b));
        }
        break;
        case rrc_a:
            z80.AF.b.h = RRC(z80.AF.b.h);
            break;
        case rrc_b:
            z80.BC.b.h = RRC(z80.BC.b.h);
            break;
        case rrc_c:
            z80.BC.b.l = RRC(z80.BC.b.l);
            break;
        case rrc_d:
            z80.DE.b.h = RRC(z80.DE.b.h);
            break;
        case rrc_e:
            z80.DE.b.l = RRC(z80.DE.b.l);
            break;
        case rrc_h:
            z80.HL.b.h = RRC(z80.HL.b.h);
            break;
        case rrc_l:
            z80.HL.b.l = RRC(z80.HL.b.l);
            break;
        case rrc_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RRC(b));
        }
        break;
        case rr_a:
            z80.AF.b.h = RR(z80.AF.b.h);
            break;
        case rr_b:
            z80.BC.b.h = RR(z80.BC.b.h);
            break;
        case rr_c:
            z80.BC.b.l = RR(z80.BC.b.l);
            break;
        case rr_d:
            z80.DE.b.h = RR(z80.DE.b.h);
            break;
        case rr_e:
            z80.DE.b.l = RR(z80.DE.b.l);
            break;
        case rr_h:
            z80.HL.b.h = RR(z80.HL.b.h);
            break;
        case rr_l:
            z80.HL.b.l = RR(z80.HL.b.l);
            break;
        case rr_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, RR(b));
        }
        break;
        case set0_a:
            z80.AF.b.h = SET(0, z80.AF.b.h);
            break;
        case set0_b:
            z80.BC.b.h = SET(0, z80.BC.b.h);
            break;
        case set0_c:
            z80.BC.b.l = SET(0, z80.BC.b.l);
            break;
        case set0_d:
            z80.DE.b.h = SET(0, z80.DE.b.h);
            break;
        case set0_e:
            z80.DE.b.l = SET(0, z80.DE.b.l);
            break;
        case set0_h:
            z80.HL.b.h = SET(0, z80.HL.b.h);
            break;
        case set0_l:
            z80.HL.b.l = SET(0, z80.HL.b.l);
            break;
        case set0_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(0, b));
        }
        break;
        case set1_a:
            z80.AF.b.h = SET(1, z80.AF.b.h);
            break;
        case set1_b:
            z80.BC.b.h = SET(1, z80.BC.b.h);
            break;
        case set1_c:
            z80.BC.b.l = SET(1, z80.BC.b.l);
            break;
        case set1_d:
            z80.DE.b.h = SET(1, z80.DE.b.h);
            break;
        case set1_e:
            z80.DE.b.l = SET(1, z80.DE.b.l);
            break;
        case set1_h:
            z80.HL.b.h = SET(1, z80.HL.b.h);
            break;
        case set1_l:
            z80.HL.b.l = SET(1, z80.HL.b.l);
            break;
        case set1_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(1, b));
        }
        break;
        case set2_a:
            z80.AF.b.h = SET(2, z80.AF.b.h);
            break;
        case set2_b:
            z80.BC.b.h = SET(2, z80.BC.b.h);
            break;
        case set2_c:
            z80.BC.b.l = SET(2, z80.BC.b.l);
            break;
        case set2_d:
            z80.DE.b.h = SET(2, z80.DE.b.h);
            break;
        case set2_e:
            z80.DE.b.l = SET(2, z80.DE.b.l);
            break;
        case set2_h:
            z80.HL.b.h = SET(2, z80.HL.b.h);
            break;
        case set2_l:
            z80.HL.b.l = SET(2, z80.HL.b.l);
            break;
        case set2_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(2, b));
        }
        break;
        case set3_a:
            z80.AF.b.h = SET(3, z80.AF.b.h);
            break;
        case set3_b:
            z80.BC.b.h = SET(3, z80.BC.b.h);
            break;
        case set3_c:
            z80.BC.b.l = SET(3, z80.BC.b.l);
            break;
        case set3_d:
            z80.DE.b.h = SET(3, z80.DE.b.h);
            break;
        case set3_e:
            z80.DE.b.l = SET(3, z80.DE.b.l);
            break;
        case set3_h:
            z80.HL.b.h = SET(3, z80.HL.b.h);
            break;
        case set3_l:
            z80.HL.b.l = SET(3, z80.HL.b.l);
            break;
        case set3_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(3, b));
        }
        break;
        case set4_a:
            z80.AF.b.h = SET(4, z80.AF.b.h);
            break;
        case set4_b:
            z80.BC.b.h = SET(4, z80.BC.b.h);
            break;
        case set4_c:
            z80.BC.b.l = SET(4, z80.BC.b.l);
            break;
        case set4_d:
            z80.DE.b.h = SET(4, z80.DE.b.h);
            break;
        case set4_e:
            z80.DE.b.l = SET(4, z80.DE.b.l);
            break;
        case set4_h:
            z80.HL.b.h = SET(4, z80.HL.b.h);
            break;
        case set4_l:
            z80.HL.b.l = SET(4, z80.HL.b.l);
            break;
        case set4_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(4, b));
        }
        break;
        case set5_a:
            z80.AF.b.h = SET(5, z80.AF.b.h);
            break;
        case set5_b:
            z80.BC.b.h = SET(5, z80.BC.b.h);
            break;
        case set5_c:
            z80.BC.b.l = SET(5, z80.BC.b.l);
            break;
        case set5_d:
            z80.DE.b.h = SET(5, z80.DE.b.h);
            break;
        case set5_e:
            z80.DE.b.l = SET(5, z80.DE.b.l);
            break;
        case set5_h:
            z80.HL.b.h = SET(5, z80.HL.b.h);
            break;
        case set5_l:
            z80.HL.b.l = SET(5, z80.HL.b.l);
            break;
        case set5_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(5, b));
        }
        break;
        case set6_a:
            z80.AF.b.h = SET(6, z80.AF.b.h);
            break;
        case set6_b:
            z80.BC.b.h = SET(6, z80.BC.b.h);
            break;
        case set6_c:
            z80.BC.b.l = SET(6, z80.BC.b.l);
            break;
        case set6_d:
            z80.DE.b.h = SET(6, z80.DE.b.h);
            break;
        case set6_e:
            z80.DE.b.l = SET(6, z80.DE.b.l);
            break;
        case set6_h:
            z80.HL.b.h = SET(6, z80.HL.b.h);
            break;
        case set6_l:
            z80.HL.b.l = SET(6, z80.HL.b.l);
            break;
        case set6_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(6, b));
        }
        break;
        case set7_a:
            z80.AF.b.h = SET(7, z80.AF.b.h);
            break;
        case set7_b:
            z80.BC.b.h = SET(7, z80.BC.b.h);
            break;
        case set7_c:
            z80.BC.b.l = SET(7, z80.BC.b.l);
            break;
        case set7_d:
            z80.DE.b.h = SET(7, z80.DE.b.h);
            break;
        case set7_e:
            z80.DE.b.l = SET(7, z80.DE.b.l);
            break;
        case set7_h:
            z80.HL.b.h = SET(7, z80.HL.b.h);
            break;
        case set7_l:
            z80.HL.b.l = SET(7, z80.HL.b.l);
            break;
        case set7_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SET(7, b));
        }
        break;
        case sla_a:
            z80.AF.b.h = SLA(z80.AF.b.h);
            break;
        case sla_b:
            z80.BC.b.h = SLA(z80.BC.b.h);
            break;
        case sla_c:
            z80.BC.b.l = SLA(z80.BC.b.l);
            break;
        case sla_d:
            z80.DE.b.h = SLA(z80.DE.b.h);
            break;
        case sla_e:
            z80.DE.b.l = SLA(z80.DE.b.l);
            break;
        case sla_h:
            z80.HL.b.h = SLA(z80.HL.b.h);
            break;
        case sla_l:
            z80.HL.b.l = SLA(z80.HL.b.l);
            break;
        case sla_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SLA(b));
        }
        break;
        case sll_a:
            z80.AF.b.h = SLL(z80.AF.b.h);
            break;
        case sll_b:
            z80.BC.b.h = SLL(z80.BC.b.h);
            break;
        case sll_c:
            z80.BC.b.l = SLL(z80.BC.b.l);
            break;
        case sll_d:
            z80.DE.b.h = SLL(z80.DE.b.h);
            break;
        case sll_e:
            z80.DE.b.l = SLL(z80.DE.b.l);
            break;
        case sll_h:
            z80.HL.b.h = SLL(z80.HL.b.h);
            break;
        case sll_l:
            z80.HL.b.l = SLL(z80.HL.b.l);
            break;
        case sll_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SLL(b));
        }
        break;
        case sra_a:
            z80.AF.b.h = SRA(z80.AF.b.h);
            break;
        case sra_b:
            z80.BC.b.h = SRA(z80.BC.b.h);
            break;
        case sra_c:
            z80.BC.b.l = SRA(z80.BC.b.l);
            break;
        case sra_d:
            z80.DE.b.h = SRA(z80.DE.b.h);
            break;
        case sra_e:
            z80.DE.b.l = SRA(z80.DE.b.l);
            break;
        case sra_h:
            z80.HL.b.h = SRA(z80.HL.b.h);
            break;
        case sra_l:
            z80.HL.b.l = SRA(z80.HL.b.l);
            break;
        case sra_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SRA(b));
        }
        break;
        case srl_a:
            z80.AF.b.h = SRL(z80.AF.b.h);
            break;
        case srl_b:
            z80.BC.b.h = SRL(z80.BC.b.h);
            break;
        case srl_c:
            z80.BC.b.l = SRL(z80.BC.b.l);
            break;
        case srl_d:
            z80.DE.b.h = SRL(z80.DE.b.h);
            break;
        case srl_e:
            z80.DE.b.l = SRL(z80.DE.b.l);
            break;
        case srl_h:
            z80.HL.b.h = SRL(z80.HL.b.h);
            break;
        case srl_l:
            z80.HL.b.l = SRL(z80.HL.b.l);
            break;
        case srl_mhl:
        {
            tUBYTE b = read_mem(z80.HL.w.l);
            write_mem(z80.HL.w.l, SRL(b));
        }
        break;
    }
}

void Z80::z80_pfx_dd()
{
    register tUBYTE bOpCode;

    bOpCode = read_mem(z80.PC.w.l++);
    iCycleCount += cc_xy[bOpCode];
    z80.R++;
    switch (bOpCode)
    {
        case adc_a:
            ADC(z80.AF.b.h);
            break;
        case adc_b:
            ADC(z80.BC.b.h);
            break;
        case adc_byte:
            ADC(read_mem(z80.PC.w.l++));
            break;
        case adc_c:
            ADC(z80.BC.b.l);
            break;
        case adc_d:
            ADC(z80.DE.b.h);
            break;
        case adc_e:
            ADC(z80.DE.b.l);
            break;
        case adc_h:
            ADC(z80.IX.b.h);
            break;
        case adc_l:
            ADC(z80.IX.b.l);
            break;
        case adc_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            ADC(read_mem(z80.IX.w.l + o));
        }
        break;
        case add_a:
            ADD(z80.AF.b.h);
            break;
        case add_b:
            ADD(z80.BC.b.h);
            break;
        case add_byte:
            ADD(read_mem(z80.PC.w.l++));
            break;
        case add_c:
            ADD(z80.BC.b.l);
            break;
        case add_d:
            ADD(z80.DE.b.h);
            break;
        case add_e:
            ADD(z80.DE.b.l);
            break;
        case add_h:
            ADD(z80.IX.b.h);
            break;
        case add_hl_bc:
            ADD16(IX, BC);
            break;
        case add_hl_de:
            ADD16(IX, DE);
            break;
        case add_hl_hl:
            ADD16(IX, IX);
            break;
        case add_hl_sp:
            ADD16(IX, SP);
            break;
        case add_l:
            ADD(z80.IX.b.l);
            break;
        case add_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            ADD(read_mem(z80.IX.w.l + o));
        }
        break;
        case and_a:
            AND(z80.AF.b.h);
            break;
        case and_b:
            AND(z80.BC.b.h);
            break;
        case and_byte:
            AND(read_mem(z80.PC.w.l++));
            break;
        case and_c:
            AND(z80.BC.b.l);
            break;
        case and_d:
            AND(z80.DE.b.h);
            break;
        case and_e:
            AND(z80.DE.b.l);
            break;
        case and_h:
            AND(z80.IX.b.h);
            break;
        case and_l:
            AND(z80.IX.b.l);
            break;
        case and_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            AND(read_mem(z80.IX.w.l + o));
        }
        break;
        case call:
            CALL;
            break;
        case call_c:
            if (z80.AF.b.l & Cflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_m:
            if (z80.AF.b.l & Sflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_p:
            if (!(z80.AF.b.l & Sflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_pe:
            if (z80.AF.b.l & Pflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_po:
            if (!(z80.AF.b.l & Pflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_z:
            if (z80.AF.b.l & Zflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case ccf:
            z80.AF.b.l = ((z80.AF.b.l & (Sflag | Zflag | Pflag | Cflag)) |
                          ((z80.AF.b.l & CF) << 4) | (z80.AF.b.h & Xflags)) ^
                         CF;
            break;
        case cpl:
            z80.AF.b.h ^= 0xff;
            z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag | Cflag)) |
                         Hflag | Nflag | (z80.AF.b.h & Xflags);
            break;
        case cp_a:
            CP(z80.AF.b.h);
            break;
        case cp_b:
            CP(z80.BC.b.h);
            break;
        case cp_byte:
            CP(read_mem(z80.PC.w.l++));
            break;
        case cp_c:
            CP(z80.BC.b.l);
            break;
        case cp_d:
            CP(z80.DE.b.h);
            break;
        case cp_e:
            CP(z80.DE.b.l);
            break;
        case cp_h:
            CP(z80.IX.b.h);
            break;
        case cp_l:
            CP(z80.IX.b.l);
            break;
        case cp_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            CP(read_mem(z80.IX.w.l + o));
        }
        break;
        case daa:
            DAA;
            break;
        case dec_a:
            DEC(z80.AF.b.h);
            break;
        case dec_b:
            DEC(z80.BC.b.h);
            break;
        case dec_bc:
            z80.BC.w.l--;
            iWSAdjust++;
            break;
        case dec_c:
            DEC(z80.BC.b.l);
            break;
        case dec_d:
            DEC(z80.DE.b.h);
            break;
        case dec_de:
            z80.DE.w.l--;
            iWSAdjust++;
            break;
        case dec_e:
            DEC(z80.DE.b.l);
            break;
        case dec_h:
            DEC(z80.IX.b.h);
            break;
        case dec_hl:
            z80.IX.w.l--;
            iWSAdjust++;
            break;
        case dec_l:
            DEC(z80.IX.b.l);
            break;
        case dec_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            tUBYTE b = read_mem(z80.IX.w.l + o);
            DEC(b);
            write_mem(z80.IX.w.l + o, b);
        }
        break;
        case dec_sp:
            z80.SP.w.l--;
            iWSAdjust++;
            break;
        case di:
            z80.IFF1 = z80.IFF2 = 0;
            z80.EI_issued = 0;
            break;
        case djnz:
            if (--z80.BC.b.h)
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            }
            break;
        case ei:
            z80.EI_issued = 2;
            break;
        case exx:
            EXX;
            break;
        case ex_af_af:
            EX(z80.AF, z80.AFx);
            break;
        case ex_de_hl:
            EX(z80.DE, z80.HL);
            break;
        case ex_msp_hl:
            EX_SP(IX);
            iWSAdjust++;
            break;
        case halt:
            z80.HALT = 1;
            z80.PC.w.l--;
            break;
        case ina:
        {
            tREGPAIR p;
            p.b.l = read_mem(z80.PC.w.l++);
            p.b.h = z80.AF.b.h;
            z80.AF.b.h = z80_IN_handler(p);
        }
        break;
        case inc_a:
            INC(z80.AF.b.h);
            break;
        case inc_b:
            INC(z80.BC.b.h);
            break;
        case inc_bc:
            z80.BC.w.l++;
            iWSAdjust++;
            break;
        case inc_c:
            INC(z80.BC.b.l);
            break;
        case inc_d:
            INC(z80.DE.b.h);
            break;
        case inc_de:
            z80.DE.w.l++;
            iWSAdjust++;
            break;
        case inc_e:
            INC(z80.DE.b.l);
            break;
        case inc_h:
            INC(z80.IX.b.h);
            break;
        case inc_hl:
            z80.IX.w.l++;
            iWSAdjust++;
            break;
        case inc_l:
            INC(z80.IX.b.l);
            break;
        case inc_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            tUBYTE b = read_mem(z80.IX.w.l + o);
            INC(b);
            write_mem(z80.IX.w.l + o, b);
        }
        break;
        case inc_sp:
            z80.SP.w.l++;
            iWSAdjust++;
            break;
        case jp:
            JP;
            break;
        case jp_c:
            if (z80.AF.b.l & Cflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_m:
            if (z80.AF.b.l & Sflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_p:
            if (!(z80.AF.b.l & Sflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_pe:
            if (z80.AF.b.l & Pflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_po:
            if (!(z80.AF.b.l & Pflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_z:
            if (z80.AF.b.l & Zflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jr:
            JR;
            break;
        case jr_c:
            if (z80.AF.b.l & Cflag)
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case jr_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case jr_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case jr_z:
            if (z80.AF.b.l & Zflag)
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case ld_a_a:
            break;
        case ld_a_b:
            z80.AF.b.h = z80.BC.b.h;
            break;
        case ld_a_byte:
            z80.AF.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_a_c:
            z80.AF.b.h = z80.BC.b.l;
            break;
        case ld_a_d:
            z80.AF.b.h = z80.DE.b.h;
            break;
        case ld_a_e:
            z80.AF.b.h = z80.DE.b.l;
            break;
        case ld_a_h:
            z80.AF.b.h = z80.IX.b.h;
            break;
        case ld_a_l:
            z80.AF.b.h = z80.IX.b.l;
            break;
        case ld_a_mbc:
            z80.AF.b.h = read_mem(z80.BC.w.l);
            break;
        case ld_a_mde:
            z80.AF.b.h = read_mem(z80.DE.w.l);
            break;
        case ld_a_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_a_mword:
        {
            tREGPAIR addr;
            addr.b.l = read_mem(z80.PC.w.l++);
            addr.b.h = read_mem(z80.PC.w.l++);
            z80.AF.b.h = read_mem(addr.w.l);
        }
        break;
        case ld_bc_word:
            z80.BC.b.l = read_mem(z80.PC.w.l++);
            z80.BC.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_b_a:
            z80.BC.b.h = z80.AF.b.h;
            break;
        case ld_b_b:
            break;
        case ld_b_byte:
            z80.BC.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_b_c:
            z80.BC.b.h = z80.BC.b.l;
            break;
        case ld_b_d:
            z80.BC.b.h = z80.DE.b.h;
            break;
        case ld_b_e:
            z80.BC.b.h = z80.DE.b.l;
            break;
        case ld_b_h:
            z80.BC.b.h = z80.IX.b.h;
            break;
        case ld_b_l:
            z80.BC.b.h = z80.IX.b.l;
            break;
        case ld_b_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_c_a:
            z80.BC.b.l = z80.AF.b.h;
            break;
        case ld_c_b:
            z80.BC.b.l = z80.BC.b.h;
            break;
        case ld_c_byte:
            z80.BC.b.l = read_mem(z80.PC.w.l++);
            break;
        case ld_c_c:
            break;
        case ld_c_d:
            z80.BC.b.l = z80.DE.b.h;
            break;
        case ld_c_e:
            z80.BC.b.l = z80.DE.b.l;
            break;
        case ld_c_h:
            z80.BC.b.l = z80.IX.b.h;
            break;
        case ld_c_l:
            z80.BC.b.l = z80.IX.b.l;
            break;
        case ld_c_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_de_word:
            z80.DE.b.l = read_mem(z80.PC.w.l++);
            z80.DE.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_d_a:
            z80.DE.b.h = z80.AF.b.h;
            break;
        case ld_d_b:
            z80.DE.b.h = z80.BC.b.h;
            break;
        case ld_d_byte:
            z80.DE.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_d_c:
            z80.DE.b.h = z80.BC.b.l;
            break;
        case ld_d_d:
            break;
        case ld_d_e:
            z80.DE.b.h = z80.DE.b.l;
            break;
        case ld_d_h:
            z80.DE.b.h = z80.IX.b.h;
            break;
        case ld_d_l:
            z80.DE.b.h = z80.IX.b.l;
            break;
        case ld_d_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_e_a:
            z80.DE.b.l = z80.AF.b.h;
            break;
        case ld_e_b:
            z80.DE.b.l = z80.BC.b.h;
            break;
        case ld_e_byte:
            z80.DE.b.l = read_mem(z80.PC.w.l++);
            break;
        case ld_e_c:
            z80.DE.b.l = z80.BC.b.l;
            break;
        case ld_e_d:
            z80.DE.b.l = z80.DE.b.h;
            break;
        case ld_e_e:
            break;
        case ld_e_h:
            z80.DE.b.l = z80.IX.b.h;
            break;
        case ld_e_l:
            z80.DE.b.l = z80.IX.b.l;
            break;
        case ld_e_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_hl_mword:
            LD16_MEM(IX);
            break;
        case ld_hl_word:
            z80.IX.b.l = read_mem(z80.PC.w.l++);
            z80.IX.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_h_a:
            z80.IX.b.h = z80.AF.b.h;
            break;
        case ld_h_b:
            z80.IX.b.h = z80.BC.b.h;
            break;
        case ld_h_byte:
            z80.IX.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_h_c:
            z80.IX.b.h = z80.BC.b.l;
            break;
        case ld_h_d:
            z80.IX.b.h = z80.DE.b.h;
            break;
        case ld_h_e:
            z80.IX.b.h = z80.DE.b.l;
            break;
        case ld_h_h:
            break;
        case ld_h_l:
            z80.IX.b.h = z80.IX.b.l;
            break;
        case ld_h_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_l_a:
            z80.IX.b.l = z80.AF.b.h;
            break;
        case ld_l_b:
            z80.IX.b.l = z80.BC.b.h;
            break;
        case ld_l_byte:
            z80.IX.b.l = read_mem(z80.PC.w.l++);
            break;
        case ld_l_c:
            z80.IX.b.l = z80.BC.b.l;
            break;
        case ld_l_d:
            z80.IX.b.l = z80.DE.b.h;
            break;
        case ld_l_e:
            z80.IX.b.l = z80.DE.b.l;
            break;
        case ld_l_h:
            z80.IX.b.l = z80.IX.b.h;
            break;
        case ld_l_l:
            break;
        case ld_l_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
        }
        break;
        case ld_mbc_a:
            write_mem(z80.BC.w.l, z80.AF.b.h);
            break;
        case ld_mde_a:
            write_mem(z80.DE.w.l, z80.AF.b.h);
            break;
        case ld_mhl_a:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
        }
        break;
        case ld_mhl_b:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
        }
        break;
        case ld_mhl_byte:
        {
            signed char o = read_mem(z80.PC.w.l++);
            tUBYTE b = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, b);
        }
        break;
        case ld_mhl_c:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
        }
        break;
        case ld_mhl_d:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
        }
        break;
        case ld_mhl_e:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
        }
        break;
        case ld_mhl_h:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
        }
        break;
        case ld_mhl_l:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
        }
        break;
        case ld_mword_a:
        {
            tREGPAIR addr;
            addr.b.l = read_mem(z80.PC.w.l++);
            addr.b.h = read_mem(z80.PC.w.l++);
            write_mem(addr.w.l, z80.AF.b.h);
        }
        break;
        case ld_mword_hl:
            LDMEM_16(IX);
            break;
        case ld_pc_hl:
            z80.PC.w.l = z80.IX.w.l;
            break;
        case ld_sp_hl:
            z80.SP.w.l = z80.IX.w.l;
            iWSAdjust++;
            break;
        case ld_sp_word:
            z80.SP.b.l = read_mem(z80.PC.w.l++);
            z80.SP.b.h = read_mem(z80.PC.w.l++);
            break;
        case nop:
            break;
        case or_a:
            OR(z80.AF.b.h);
            break;
        case or_b:
            OR(z80.BC.b.h);
            break;
        case or_byte:
            OR(read_mem(z80.PC.w.l++));
            break;
        case or_c:
            OR(z80.BC.b.l);
            break;
        case or_d:
            OR(z80.DE.b.h);
            break;
        case or_e:
            OR(z80.DE.b.l);
            break;
        case or_h:
            OR(z80.IX.b.h);
            break;
        case or_l:
            OR(z80.IX.b.l);
            break;
        case or_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            OR(read_mem(z80.IX.w.l + o));
        }
        break;
        case outa:
        {
            tREGPAIR p;
            p.b.l = read_mem(z80.PC.w.l++);
            p.b.h = z80.AF.b.h;
            z80_OUT_handler(p, z80.AF.b.h);
        }
        break;
        case pfx_cb:
            z80_pfx_ddcb();
            break;
        case pfx_dd:
            z80_pfx_dd();
            break;
        case pfx_ed:
            z80_pfx_ed();
            break;
        case pfx_fd:
            z80_pfx_fd();
            break;
        case pop_af:
            POP(AF);
            break;
        case pop_bc:
            POP(BC);
            break;
        case pop_de:
            POP(DE);
            break;
        case pop_hl:
            POP(IX);
            break;
        case push_af:
            PUSH(AF);
            break;
        case push_bc:
            PUSH(BC);
            break;
        case push_de:
            PUSH(DE);
            break;
        case push_hl:
            PUSH(IX);
            break;
        case ret:
            RET;
            break;
        case ret_c:
            if (z80.AF.b.l & Cflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_m:
            if (z80.AF.b.l & Sflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_p:
            if (!(z80.AF.b.l & Sflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_pe:
            if (z80.AF.b.l & Pflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_po:
            if (!(z80.AF.b.l & Pflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_z:
            if (z80.AF.b.l & Zflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case rla:
            RLA;
            break;
        case rlca:
            RLCA;
            break;
        case rra:
            RRA;
            break;
        case rrca:
            RRCA;
            break;
        case rst00:
            RST(0x0000);
            break;
        case rst08:
            RST(0x0008);
            break;
        case rst10:
            RST(0x0010);
            break;
        case rst18:
            RST(0x0018);
            break;
        case rst20:
            RST(0x0020);
            break;
        case rst28:
            RST(0x0028);
            break;
        case rst30:
            RST(0x0030);
            break;
        case rst38:
            RST(0x0038);
            break;
        case sbc_a:
            SBC(z80.AF.b.h);
            break;
        case sbc_b:
            SBC(z80.BC.b.h);
            break;
        case sbc_byte:
            SBC(read_mem(z80.PC.w.l++));
            break;
        case sbc_c:
            SBC(z80.BC.b.l);
            break;
        case sbc_d:
            SBC(z80.DE.b.h);
            break;
        case sbc_e:
            SBC(z80.DE.b.l);
            break;
        case sbc_h:
            SBC(z80.IX.b.h);
            break;
        case sbc_l:
            SBC(z80.IX.b.l);
            break;
        case sbc_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            SBC(read_mem(z80.IX.w.l + o));
        }
        break;
        case scf:
            z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | Cflag |
                         (z80.AF.b.h & Xflags);
            break;
        case sub_a:
            SUB(z80.AF.b.h);
            break;
        case sub_b:
            SUB(z80.BC.b.h);
            break;
        case sub_byte:
            SUB(read_mem(z80.PC.w.l++));
            break;
        case sub_c:
            SUB(z80.BC.b.l);
            break;
        case sub_d:
            SUB(z80.DE.b.h);
            break;
        case sub_e:
            SUB(z80.DE.b.l);
            break;
        case sub_h:
            SUB(z80.IX.b.h);
            break;
        case sub_l:
            SUB(z80.IX.b.l);
            break;
        case sub_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            SUB(read_mem(z80.IX.w.l + o));
        }
        break;
        case xor_a:
            XOR(z80.AF.b.h);
            break;
        case xor_b:
            XOR(z80.BC.b.h);
            break;
        case xor_byte:
            XOR(read_mem(z80.PC.w.l++));
            break;
        case xor_c:
            XOR(z80.BC.b.l);
            break;
        case xor_d:
            XOR(z80.DE.b.h);
            break;
        case xor_e:
            XOR(z80.DE.b.l);
            break;
        case xor_h:
            XOR(z80.IX.b.h);
            break;
        case xor_l:
            XOR(z80.IX.b.l);
            break;
        case xor_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            XOR(read_mem(z80.IX.w.l + o));
        }
        break;
    }
}

void Z80::z80_pfx_ddcb()
{
    signed char o;
    register tUBYTE bOpCode;

    o = read_mem(z80.PC.w.l++); // offset
    bOpCode = read_mem(z80.PC.w.l++);
    iCycleCount += cc_xycb[bOpCode];
    switch (bOpCode)
    {
        case bit0_a:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_b:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_c:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_d:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_e:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_h:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_l:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit0_mhl:
            BIT_XY(0, read_mem(z80.IX.w.l + o));
            break;
        case bit1_a:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_b:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_c:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_d:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_e:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_h:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_l:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit1_mhl:
            BIT_XY(1, read_mem(z80.IX.w.l + o));
            break;
        case bit2_a:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_b:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_c:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_d:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_e:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_h:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_l:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit2_mhl:
            BIT_XY(2, read_mem(z80.IX.w.l + o));
            break;
        case bit3_a:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_b:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_c:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_d:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_e:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_h:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_l:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit3_mhl:
            BIT_XY(3, read_mem(z80.IX.w.l + o));
            break;
        case bit4_a:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_b:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_c:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_d:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_e:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_h:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_l:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit4_mhl:
            BIT_XY(4, read_mem(z80.IX.w.l + o));
            break;
        case bit5_a:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_b:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_c:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_d:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_e:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_h:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_l:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit5_mhl:
            BIT_XY(5, read_mem(z80.IX.w.l + o));
            break;
        case bit6_a:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_b:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_c:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_d:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_e:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_h:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_l:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit6_mhl:
            BIT_XY(6, read_mem(z80.IX.w.l + o));
            break;
        case bit7_a:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_b:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_c:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_d:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_e:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_h:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_l:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case bit7_mhl:
            BIT_XY(7, read_mem(z80.IX.w.l + o));
            break;
        case res0_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(0, z80.AF.b.h));
            break;
        case res0_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(0, z80.BC.b.h));
            break;
        case res0_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(0, z80.BC.b.l));
            break;
        case res0_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(0, z80.DE.b.h));
            break;
        case res0_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(0, z80.DE.b.l));
            break;
        case res0_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(0, z80.HL.b.h));
            break;
        case res0_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(0, z80.HL.b.l));
            break;
        case res0_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(0, b));
        }
        break;
        case res1_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(1, z80.AF.b.h));
            break;
        case res1_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(1, z80.BC.b.h));
            break;
        case res1_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(1, z80.BC.b.l));
            break;
        case res1_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(1, z80.DE.b.h));
            break;
        case res1_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(1, z80.DE.b.l));
            break;
        case res1_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(1, z80.HL.b.h));
            break;
        case res1_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(1, z80.HL.b.l));
            break;
        case res1_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(1, b));
        }
        break;
        case res2_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(2, z80.AF.b.h));
            break;
        case res2_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(2, z80.BC.b.h));
            break;
        case res2_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(2, z80.BC.b.l));
            break;
        case res2_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(2, z80.DE.b.h));
            break;
        case res2_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(2, z80.DE.b.l));
            break;
        case res2_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(2, z80.HL.b.h));
            break;
        case res2_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(2, z80.HL.b.l));
            break;
        case res2_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(2, b));
        }
        break;
        case res3_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(3, z80.AF.b.h));
            break;
        case res3_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(3, z80.BC.b.h));
            break;
        case res3_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(3, z80.BC.b.l));
            break;
        case res3_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(3, z80.DE.b.h));
            break;
        case res3_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(3, z80.DE.b.l));
            break;
        case res3_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(3, z80.HL.b.h));
            break;
        case res3_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(3, z80.HL.b.l));
            break;
        case res3_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(3, b));
        }
        break;
        case res4_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(4, z80.AF.b.h));
            break;
        case res4_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(4, z80.BC.b.h));
            break;
        case res4_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(4, z80.BC.b.l));
            break;
        case res4_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(4, z80.DE.b.h));
            break;
        case res4_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(4, z80.DE.b.l));
            break;
        case res4_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(4, z80.HL.b.h));
            break;
        case res4_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(4, z80.HL.b.l));
            break;
        case res4_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(4, b));
        }
        break;
        case res5_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(5, z80.AF.b.h));
            break;
        case res5_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(5, z80.BC.b.h));
            break;
        case res5_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(5, z80.BC.b.l));
            break;
        case res5_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(5, z80.DE.b.h));
            break;
        case res5_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(5, z80.DE.b.l));
            break;
        case res5_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(5, z80.HL.b.h));
            break;
        case res5_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(5, z80.HL.b.l));
            break;
        case res5_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(5, b));
        }
        break;
        case res6_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(6, z80.AF.b.h));
            break;
        case res6_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(6, z80.BC.b.h));
            break;
        case res6_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(6, z80.BC.b.l));
            break;
        case res6_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(6, z80.DE.b.h));
            break;
        case res6_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(6, z80.DE.b.l));
            break;
        case res6_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(6, z80.HL.b.h));
            break;
        case res6_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(6, z80.HL.b.l));
            break;
        case res6_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(6, b));
        }
        break;
        case res7_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = RES(7, z80.AF.b.h));
            break;
        case res7_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = RES(7, z80.BC.b.h));
            break;
        case res7_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = RES(7, z80.BC.b.l));
            break;
        case res7_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = RES(7, z80.DE.b.h));
            break;
        case res7_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = RES(7, z80.DE.b.l));
            break;
        case res7_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = RES(7, z80.HL.b.h));
            break;
        case res7_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = RES(7, z80.HL.b.l));
            break;
        case res7_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RES(7, b));
        }
        break;
        case rlc_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = RLC(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case rlc_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = RLC(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case rlc_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = RLC(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case rlc_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = RLC(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case rlc_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = RLC(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case rlc_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = RLC(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case rlc_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = RLC(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case rlc_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RLC(b));
        }
        break;
        case rl_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = RL(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case rl_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = RL(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case rl_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = RL(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case rl_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = RL(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case rl_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = RL(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case rl_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = RL(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case rl_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = RL(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case rl_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RL(b));
        }
        break;
        case rrc_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = RRC(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case rrc_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = RRC(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case rrc_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = RRC(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case rrc_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = RRC(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case rrc_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = RRC(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case rrc_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = RRC(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case rrc_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = RRC(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case rrc_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RRC(b));
        }
        break;
        case rr_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = RR(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case rr_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = RR(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case rr_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = RR(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case rr_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = RR(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case rr_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = RR(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case rr_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = RR(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case rr_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = RR(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case rr_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, RR(b));
        }
        break;
        case set0_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(0, z80.AF.b.h));
            break;
        case set0_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(0, z80.BC.b.h));
            break;
        case set0_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(0, z80.BC.b.l));
            break;
        case set0_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(0, z80.DE.b.h));
            break;
        case set0_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(0, z80.DE.b.l));
            break;
        case set0_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(0, z80.HL.b.h));
            break;
        case set0_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(0, z80.HL.b.l));
            break;
        case set0_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(0, b));
        }
        break;
        case set1_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(1, z80.AF.b.h));
            break;
        case set1_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(1, z80.BC.b.h));
            break;
        case set1_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(1, z80.BC.b.l));
            break;
        case set1_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(1, z80.DE.b.h));
            break;
        case set1_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(1, z80.DE.b.l));
            break;
        case set1_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(1, z80.HL.b.h));
            break;
        case set1_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(1, z80.HL.b.l));
            break;
        case set1_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(1, b));
        }
        break;
        case set2_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(2, z80.AF.b.h));
            break;
        case set2_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(2, z80.BC.b.h));
            break;
        case set2_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(2, z80.BC.b.l));
            break;
        case set2_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(2, z80.DE.b.h));
            break;
        case set2_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(2, z80.DE.b.l));
            break;
        case set2_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(2, z80.HL.b.h));
            break;
        case set2_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(2, z80.HL.b.l));
            break;
        case set2_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(2, b));
        }
        break;
        case set3_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(3, z80.AF.b.h));
            break;
        case set3_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(3, z80.BC.b.h));
            break;
        case set3_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(3, z80.BC.b.l));
            break;
        case set3_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(3, z80.DE.b.h));
            break;
        case set3_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(3, z80.DE.b.l));
            break;
        case set3_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(3, z80.HL.b.h));
            break;
        case set3_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(3, z80.HL.b.l));
            break;
        case set3_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(3, b));
        }
        break;
        case set4_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(4, z80.AF.b.h));
            break;
        case set4_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(4, z80.BC.b.h));
            break;
        case set4_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(4, z80.BC.b.l));
            break;
        case set4_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(4, z80.DE.b.h));
            break;
        case set4_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(4, z80.DE.b.l));
            break;
        case set4_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(4, z80.HL.b.h));
            break;
        case set4_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(4, z80.HL.b.l));
            break;
        case set4_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(4, b));
        }
        break;
        case set5_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(5, z80.AF.b.h));
            break;
        case set5_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(5, z80.BC.b.h));
            break;
        case set5_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(5, z80.BC.b.l));
            break;
        case set5_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(5, z80.DE.b.h));
            break;
        case set5_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(5, z80.DE.b.l));
            break;
        case set5_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(5, z80.HL.b.h));
            break;
        case set5_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(5, z80.HL.b.l));
            break;
        case set5_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(5, b));
        }
        break;
        case set6_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(6, z80.AF.b.h));
            break;
        case set6_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(6, z80.BC.b.h));
            break;
        case set6_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(6, z80.BC.b.l));
            break;
        case set6_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(6, z80.DE.b.h));
            break;
        case set6_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(6, z80.DE.b.l));
            break;
        case set6_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(6, z80.HL.b.h));
            break;
        case set6_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(6, z80.HL.b.l));
            break;
        case set6_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(6, b));
        }
        break;
        case set7_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.AF.b.h = SET(7, z80.AF.b.h));
            break;
        case set7_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.h = SET(7, z80.BC.b.h));
            break;
        case set7_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.BC.b.l = SET(7, z80.BC.b.l));
            break;
        case set7_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.h = SET(7, z80.DE.b.h));
            break;
        case set7_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.DE.b.l = SET(7, z80.DE.b.l));
            break;
        case set7_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.h = SET(7, z80.HL.b.h));
            break;
        case set7_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, z80.HL.b.l = SET(7, z80.HL.b.l));
            break;
        case set7_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SET(7, b));
        }
        break;
        case sla_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = SLA(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case sla_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = SLA(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case sla_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = SLA(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case sla_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = SLA(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case sla_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = SLA(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case sla_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = SLA(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case sla_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = SLA(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case sla_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SLA(b));
        }
        break;
        case sll_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = SLL(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case sll_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = SLL(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case sll_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = SLL(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case sll_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = SLL(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case sll_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = SLL(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case sll_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = SLL(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case sll_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = SLL(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case sll_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SLL(b));
        }
        break;
        case sra_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = SRA(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case sra_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = SRA(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case sra_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = SRA(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case sra_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = SRA(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case sra_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = SRA(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case sra_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = SRA(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case sra_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = SRA(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case sra_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SRA(b));
        }
        break;
        case srl_a:
            z80.AF.b.h = read_mem(z80.IX.w.l + o);
            z80.AF.b.h = SRL(z80.AF.b.h);
            write_mem(z80.IX.w.l + o, z80.AF.b.h);
            break;
        case srl_b:
            z80.BC.b.h = read_mem(z80.IX.w.l + o);
            z80.BC.b.h = SRL(z80.BC.b.h);
            write_mem(z80.IX.w.l + o, z80.BC.b.h);
            break;
        case srl_c:
            z80.BC.b.l = read_mem(z80.IX.w.l + o);
            z80.BC.b.l = SRL(z80.BC.b.l);
            write_mem(z80.IX.w.l + o, z80.BC.b.l);
            break;
        case srl_d:
            z80.DE.b.h = read_mem(z80.IX.w.l + o);
            z80.DE.b.h = SRL(z80.DE.b.h);
            write_mem(z80.IX.w.l + o, z80.DE.b.h);
            break;
        case srl_e:
            z80.DE.b.l = read_mem(z80.IX.w.l + o);
            z80.DE.b.l = SRL(z80.DE.b.l);
            write_mem(z80.IX.w.l + o, z80.DE.b.l);
            break;
        case srl_h:
            z80.HL.b.h = read_mem(z80.IX.w.l + o);
            z80.HL.b.h = SRL(z80.HL.b.h);
            write_mem(z80.IX.w.l + o, z80.HL.b.h);
            break;
        case srl_l:
            z80.HL.b.l = read_mem(z80.IX.w.l + o);
            z80.HL.b.l = SRL(z80.HL.b.l);
            write_mem(z80.IX.w.l + o, z80.HL.b.l);
            break;
        case srl_mhl:
        {
            tUBYTE b = read_mem(z80.IX.w.l + o);
            write_mem(z80.IX.w.l + o, SRL(b));
        }
        break;
    }
}

void Z80::z80_pfx_ed()
{
    register tUBYTE bOpCode;

    bOpCode = read_mem(z80.PC.w.l++);
    iCycleCount += cc_ed[bOpCode];
    z80.R++;
    switch (bOpCode)
    {
        case adc_hl_bc:
            ADC16(BC);
            break;
        case adc_hl_de:
            ADC16(DE);
            break;
        case adc_hl_hl:
            ADC16(HL);
            break;
        case adc_hl_sp:
            ADC16(SP);
            break;
        case cpd:
            CPD;
            break;
        case cpdr:
            CPDR;
            break;
        case cpi:
            CPI;
            break;
        case cpir:
            CPIR;
            break;
        case ed_00:
            break;
        case ed_01:
            break;
        case ed_02:
            break;
        case ed_03:
            break;
        case ed_04:
            break;
        case ed_05:
            break;
        case ed_06:
            break;
        case ed_07:
            break;
        case ed_08:
            break;
        case ed_09:
            break;
        case ed_0a:
            break;
        case ed_0b:
            break;
        case ed_0c:
            break;
        case ed_0d:
            break;
        case ed_0e:
            break;
        case ed_0f:
            break;
        case ed_10:
            break;
        case ed_11:
            break;
        case ed_12:
            break;
        case ed_13:
            break;
        case ed_14:
            break;
        case ed_15:
            break;
        case ed_16:
            break;
        case ed_17:
            break;
        case ed_18:
            break;
        case ed_19:
            break;
        case ed_1a:
            break;
        case ed_1b:
            break;
        case ed_1c:
            break;
        case ed_1d:
            break;
        case ed_1e:
            break;
        case ed_1f:
            break;
        case ed_20:
            break;
        case ed_21:
            break;
        case ed_22:
            break;
        case ed_23:
            break;
        case ed_24:
            break;
        case ed_25:
            break;
        case ed_26:
            break;
        case ed_27:
            break;
        case ed_28:
            break;
        case ed_29:
            break;
        case ed_2a:
            break;
        case ed_2b:
            break;
        case ed_2c:
            break;
        case ed_2d:
            break;
        case ed_2e:
            break;
        case ed_2f:
            break;
        case ed_30:
            break;
        case ed_31:
            break;
        case ed_32:
            break;
        case ed_33:
            break;
        case ed_34:
            break;
        case ed_35:
            break;
        case ed_36:
            break;
        case ed_37:
            break;
        case ed_38:
            break;
        case ed_39:
            break;
        case ed_3a:
            break;
        case ed_3b:
            break;
        case ed_3c:
            break;
        case ed_3d:
            break;
        case ed_3e:
            break;
        case ed_3f:
            break;
        case ed_77:
            break;
        case ed_7f:
            break;
        case ed_80:
            break;
        case ed_81:
            break;
        case ed_82:
            break;
        case ed_83:
            break;
        case ed_84:
            break;
        case ed_85:
            break;
        case ed_86:
            break;
        case ed_87:
            break;
        case ed_88:
            break;
        case ed_89:
            break;
        case ed_8a:
            break;
        case ed_8b:
            break;
        case ed_8c:
            break;
        case ed_8d:
            break;
        case ed_8e:
            break;
        case ed_8f:
            break;
        case ed_90:
            break;
        case ed_91:
            break;
        case ed_92:
            break;
        case ed_93:
            break;
        case ed_94:
            break;
        case ed_95:
            break;
        case ed_96:
            break;
        case ed_97:
            break;
        case ed_98:
            break;
        case ed_99:
            break;
        case ed_9a:
            break;
        case ed_9b:
            break;
        case ed_9c:
            break;
        case ed_9d:
            break;
        case ed_9e:
            break;
        case ed_9f:
            break;
        case ed_a4:
            break;
        case ed_a5:
            break;
        case ed_a6:
            break;
        case ed_a7:
            break;
        case ed_ac:
            break;
        case ed_ad:
            break;
        case ed_ae:
            break;
        case ed_af:
            break;
        case ed_b4:
            break;
        case ed_b5:
            break;
        case ed_b6:
            break;
        case ed_b7:
            break;
        case ed_bc:
            break;
        case ed_bd:
            break;
        case ed_be:
            break;
        case ed_bf:
            break;
        case ed_c0:
            break;
        case ed_c1:
            break;
        case ed_c2:
            break;
        case ed_c3:
            break;
        case ed_c4:
            break;
        case ed_c5:
            break;
        case ed_c6:
            break;
        case ed_c7:
            break;
        case ed_c8:
            break;
        case ed_c9:
            break;
        case ed_ca:
            break;
        case ed_cb:
            break;
        case ed_cc:
            break;
        case ed_cd:
            break;
        case ed_ce:
            break;
        case ed_cf:
            break;
        case ed_d0:
            break;
        case ed_d1:
            break;
        case ed_d2:
            break;
        case ed_d3:
            break;
        case ed_d4:
            break;
        case ed_d5:
            break;
        case ed_d6:
            break;
        case ed_d7:
            break;
        case ed_d8:
            break;
        case ed_d9:
            break;
        case ed_da:
            break;
        case ed_db:
            break;
        case ed_dc:
            break;
        case ed_dd:
            break;
        case ed_de:
            break;
        case ed_df:
            break;
        case ed_e0:
            break;
        case ed_e1:
            break;
        case ed_e2:
            break;
        case ed_e3:
            break;
        case ed_e4:
            break;
        case ed_e5:
            break;
        case ed_e6:
            break;
        case ed_e7:
            break;
        case ed_e8:
            break;
        case ed_e9:
            break;
        case ed_ea:
            break;
        case ed_eb:
            break;
        case ed_ec:
            break;
        case ed_ed:
            break;
        case ed_ee:
            break;
        case ed_ef:
            break;
        case ed_f0:
            break;
        case ed_f1:
            break;
        case ed_f2:
            break;
        case ed_f3:
            break;
        case ed_f4:
            break;
        case ed_f5:
            break;
        case ed_f6:
            break;
        case ed_f7:
            break;
        case ed_f8:
            break;
        case ed_f9:
            break;
        case ed_fa:
            break;
        case ed_fb:
            break;
        case ed_fc:
            break;
        case ed_fd:
            break;
        case ed_fe:
            break;
        case ed_ff:
            break;
        case im_0:
            z80.IM = 0;
            break;
        case im_0_1:
            z80.IM = 0;
            break;
        case im_0_2:
            z80.IM = 0;
            break;
        case im_0_3:
            z80.IM = 0;
            break;
        case im_1:
            z80.IM = 1;
            break;
        case im_1_1:
            z80.IM = 1;
            break;
        case im_2:
            z80.IM = 2;
            break;
        case im_2_1:
            z80.IM = 2;
            break;
        case ind:
            IND;
            break;
        case indr:
            INDR;
            break;
        case ini:
            INI;
            break;
        case inir:
            INIR;
            break;
        case in_0_c:
        {
            tUBYTE res = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[res];
        }
        break;
        case in_a_c:
            z80.AF.b.h = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.AF.b.h];
            break;
        case in_b_c:
            z80.BC.b.h = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.BC.b.h];
            break;
        case in_c_c:
            z80.BC.b.l = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.BC.b.l];
            break;
        case in_d_c:
            z80.DE.b.h = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.DE.b.h];
            break;
        case in_e_c:
            z80.DE.b.l = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.DE.b.l];
            break;
        case in_h_c:
            z80.HL.b.h = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.HL.b.h];
            break;
        case in_l_c:
            z80.HL.b.l = z80_IN_handler(z80.BC);
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZP[z80.HL.b.l];
            break;
        case ldd:
            LDD;
            iWSAdjust++;
            break;
        case lddr:
            LDDR;
            iWSAdjust++;
            break;
        case ldi:
            LDI;
            iWSAdjust++;
            break;
        case ldir:
            LDIR;
            iWSAdjust++;
            break;
        case ld_a_i:
            z80.AF.b.h = MF2_RUNNING;
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZ[z80.AF.b.h] | z80.IFF2;
            iWSAdjust++;
            break;
        case ld_a_r:
            z80.AF.b.h = (z80.R & 0x7f) | z80.HALT;
            z80.AF.b.l = (z80.AF.b.l & Cflag) | SZ[z80.AF.b.h] | z80.IFF2;
            iWSAdjust++;
            break;
        case ld_EDbc_mword:
            LD16_MEM(BC);
            break;
        case ld_EDde_mword:
            LD16_MEM(DE);
            break;
        case ld_EDhl_mword:
            LD16_MEM(HL);
            break;
        case ld_EDmword_bc:
            LDMEM_16(BC);
            break;
        case ld_EDmword_de:
            LDMEM_16(DE);
            break;
        case ld_EDmword_hl:
            LDMEM_16(HL);
            break;
        case ld_EDmword_sp:
            LDMEM_16(SP);
            break;
        case ld_EDsp_mword:
            LD16_MEM(SP);
            break;
        case ld_i_a:
            z80.I = z80.AF.b.h;
            iWSAdjust++;
            break;
        case ld_r_a:
            z80.R = z80.AF.b.h;
            z80.HALT = z80.AF.b.h & 0x80;
            iWSAdjust++;
            break;
        case neg:
            NEG;
            break;
        case neg_1:
            NEG;
            break;
        case neg_2:
            NEG;
            break;
        case neg_3:
            NEG;
            break;
        case neg_4:
            NEG;
            break;
        case neg_5:
            NEG;
            break;
        case neg_6:
            NEG;
            break;
        case neg_7:
            NEG;
            break;
        case otdr:
            OTDR;
            break;
        case otir:
            OTIR;
            break;
        case outd:
            OUTD;
            break;
        case outi:
            OUTI;
            break;
        case out_c_0:
            z80_OUT_handler(z80.BC, 0);
            break;
        case out_c_a:
            z80_OUT_handler(z80.BC, z80.AF.b.h);
            break;
        case out_c_b:
            z80_OUT_handler(z80.BC, z80.BC.b.h);
            break;
        case out_c_c:
            z80_OUT_handler(z80.BC, z80.BC.b.l);
            break;
        case out_c_d:
            z80_OUT_handler(z80.BC, z80.DE.b.h);
            break;
        case out_c_e:
            z80_OUT_handler(z80.BC, z80.DE.b.l);
            break;
        case out_c_h:
            z80_OUT_handler(z80.BC, z80.HL.b.h);
            break;
        case out_c_l:
            z80_OUT_handler(z80.BC, z80.HL.b.l);
            break;
        case reti:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case reti_1:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case reti_2:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case reti_3:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case retn:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case retn_1:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case retn_2:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case retn_3:
            z80.IFF1 = z80.IFF2;
            RET;
            break;
        case rld:
            RLD;
            break;
        case rrd:
            RRD;
            break;
        case sbc_hl_bc:
            SBC16(BC);
            break;
        case sbc_hl_de:
            SBC16(DE);
            break;
        case sbc_hl_hl:
            SBC16(HL);
            break;
        case sbc_hl_sp:
            SBC16(SP);
            break;
    }
}

void Z80::z80_pfx_fd()
{
    register tUBYTE bOpCode;

    bOpCode = read_mem(z80.PC.w.l++);
    iCycleCount += cc_xy[bOpCode];
    z80.R++;
    switch (bOpCode)
    {
        case adc_a:
            ADC(z80.AF.b.h);
            break;
        case adc_b:
            ADC(z80.BC.b.h);
            break;
        case adc_byte:
            ADC(read_mem(z80.PC.w.l++));
            break;
        case adc_c:
            ADC(z80.BC.b.l);
            break;
        case adc_d:
            ADC(z80.DE.b.h);
            break;
        case adc_e:
            ADC(z80.DE.b.l);
            break;
        case adc_h:
            ADC(z80.IY.b.h);
            break;
        case adc_l:
            ADC(z80.IY.b.l);
            break;
        case adc_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            ADC(read_mem(z80.IY.w.l + o));
        }
        break;
        case add_a:
            ADD(z80.AF.b.h);
            break;
        case add_b:
            ADD(z80.BC.b.h);
            break;
        case add_byte:
            ADD(read_mem(z80.PC.w.l++));
            break;
        case add_c:
            ADD(z80.BC.b.l);
            break;
        case add_d:
            ADD(z80.DE.b.h);
            break;
        case add_e:
            ADD(z80.DE.b.l);
            break;
        case add_h:
            ADD(z80.IY.b.h);
            break;
        case add_hl_bc:
            ADD16(IY, BC);
            break;
        case add_hl_de:
            ADD16(IY, DE);
            break;
        case add_hl_hl:
            ADD16(IY, IY);
            break;
        case add_hl_sp:
            ADD16(IY, SP);
            break;
        case add_l:
            ADD(z80.IY.b.l);
            break;
        case add_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            ADD(read_mem(z80.IY.w.l + o));
        }
        break;
        case and_a:
            AND(z80.AF.b.h);
            break;
        case and_b:
            AND(z80.BC.b.h);
            break;
        case and_byte:
            AND(read_mem(z80.PC.w.l++));
            break;
        case and_c:
            AND(z80.BC.b.l);
            break;
        case and_d:
            AND(z80.DE.b.h);
            break;
        case and_e:
            AND(z80.DE.b.l);
            break;
        case and_h:
            AND(z80.IY.b.h);
            break;
        case and_l:
            AND(z80.IY.b.l);
            break;
        case and_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            AND(read_mem(z80.IY.w.l + o));
        }
        break;
        case call:
            CALL;
            break;
        case call_c:
            if (z80.AF.b.l & Cflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_m:
            if (z80.AF.b.l & Sflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_p:
            if (!(z80.AF.b.l & Sflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_pe:
            if (z80.AF.b.l & Pflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_po:
            if (!(z80.AF.b.l & Pflag))
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case call_z:
            if (z80.AF.b.l & Zflag)
            {
                iCycleCount += cc_ex[bOpCode];
                CALL
            }
            else
            {
                z80.PC.w.l += 2;
            }
            break;
        case ccf:
            z80.AF.b.l = ((z80.AF.b.l & (Sflag | Zflag | Pflag | Cflag)) |
                          ((z80.AF.b.l & CF) << 4) | (z80.AF.b.h & Xflags)) ^
                         CF;
            break;
        case cpl:
            z80.AF.b.h ^= 0xff;
            z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag | Cflag)) |
                         Hflag | Nflag | (z80.AF.b.h & Xflags);
            break;
        case cp_a:
            CP(z80.AF.b.h);
            break;
        case cp_b:
            CP(z80.BC.b.h);
            break;
        case cp_byte:
            CP(read_mem(z80.PC.w.l++));
            break;
        case cp_c:
            CP(z80.BC.b.l);
            break;
        case cp_d:
            CP(z80.DE.b.h);
            break;
        case cp_e:
            CP(z80.DE.b.l);
            break;
        case cp_h:
            CP(z80.IY.b.h);
            break;
        case cp_l:
            CP(z80.IY.b.l);
            break;
        case cp_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            CP(read_mem(z80.IY.w.l + o));
        }
        break;
        case daa:
            DAA;
            break;
        case dec_a:
            DEC(z80.AF.b.h);
            break;
        case dec_b:
            DEC(z80.BC.b.h);
            break;
        case dec_bc:
            z80.BC.w.l--;
            iWSAdjust++;
            break;
        case dec_c:
            DEC(z80.BC.b.l);
            break;
        case dec_d:
            DEC(z80.DE.b.h);
            break;
        case dec_de:
            z80.DE.w.l--;
            iWSAdjust++;
            break;
        case dec_e:
            DEC(z80.DE.b.l);
            break;
        case dec_h:
            DEC(z80.IY.b.h);
            break;
        case dec_hl:
            z80.IY.w.l--;
            iWSAdjust++;
            break;
        case dec_l:
            DEC(z80.IY.b.l);
            break;
        case dec_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            tUBYTE b = read_mem(z80.IY.w.l + o);
            DEC(b);
            write_mem(z80.IY.w.l + o, b);
        }
        break;
        case dec_sp:
            z80.SP.w.l--;
            iWSAdjust++;
            break;
        case di:
            z80.IFF1 = z80.IFF2 = 0;
            z80.EI_issued = 0;
            break;
        case djnz:
            if (--z80.BC.b.h)
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            }
            break;
        case ei:
            z80.EI_issued = 2;
            break;
        case exx:
            EXX;
            break;
        case ex_af_af:
            EX(z80.AF, z80.AFx);
            break;
        case ex_de_hl:
            EX(z80.DE, z80.HL);
            break;
        case ex_msp_hl:
            EX_SP(IY);
            iWSAdjust++;
            break;
        case halt:
            z80.HALT = 1;
            z80.PC.w.l--;
            break;
        case ina:
        {
            tREGPAIR p;
            p.b.l = read_mem(z80.PC.w.l++);
            p.b.h = z80.AF.b.h;
            z80.AF.b.h = z80_IN_handler(p);
        }
        break;
        case inc_a:
            INC(z80.AF.b.h);
            break;
        case inc_b:
            INC(z80.BC.b.h);
            break;
        case inc_bc:
            z80.BC.w.l++;
            iWSAdjust++;
            break;
        case inc_c:
            INC(z80.BC.b.l);
            break;
        case inc_d:
            INC(z80.DE.b.h);
            break;
        case inc_de:
            z80.DE.w.l++;
            iWSAdjust++;
            break;
        case inc_e:
            INC(z80.DE.b.l);
            break;
        case inc_h:
            INC(z80.IY.b.h);
            break;
        case inc_hl:
            z80.IY.w.l++;
            iWSAdjust++;
            break;
        case inc_l:
            INC(z80.IY.b.l);
            break;
        case inc_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            tUBYTE b = read_mem(z80.IY.w.l + o);
            INC(b);
            write_mem(z80.IY.w.l + o, b);
        }
        break;
        case inc_sp:
            z80.SP.w.l++;
            iWSAdjust++;
            break;
        case jp:
            JP;
            break;
        case jp_c:
            if (z80.AF.b.l & Cflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_m:
            if (z80.AF.b.l & Sflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_p:
            if (!(z80.AF.b.l & Sflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_pe:
            if (z80.AF.b.l & Pflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_po:
            if (!(z80.AF.b.l & Pflag))
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jp_z:
            if (z80.AF.b.l & Zflag)
            {
                JP
            }
            else
            {
                z80.PC.w.l += 2;
            };
            break;
        case jr:
            JR;
            break;
        case jr_c:
            if (z80.AF.b.l & Cflag)
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case jr_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case jr_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case jr_z:
            if (z80.AF.b.l & Zflag)
            {
                iCycleCount += cc_ex[bOpCode];
                JR
            }
            else
            {
                z80.PC.w.l++;
            };
            break;
        case ld_a_a:
            break;
        case ld_a_b:
            z80.AF.b.h = z80.BC.b.h;
            break;
        case ld_a_byte:
            z80.AF.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_a_c:
            z80.AF.b.h = z80.BC.b.l;
            break;
        case ld_a_d:
            z80.AF.b.h = z80.DE.b.h;
            break;
        case ld_a_e:
            z80.AF.b.h = z80.DE.b.l;
            break;
        case ld_a_h:
            z80.AF.b.h = z80.IY.b.h;
            break;
        case ld_a_l:
            z80.AF.b.h = z80.IY.b.l;
            break;
        case ld_a_mbc:
            z80.AF.b.h = read_mem(z80.BC.w.l);
            break;
        case ld_a_mde:
            z80.AF.b.h = read_mem(z80.DE.w.l);
            break;
        case ld_a_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_a_mword:
        {
            tREGPAIR addr;
            addr.b.l = read_mem(z80.PC.w.l++);
            addr.b.h = read_mem(z80.PC.w.l++);
            z80.AF.b.h = read_mem(addr.w.l);
        }
        break;
        case ld_bc_word:
            z80.BC.b.l = read_mem(z80.PC.w.l++);
            z80.BC.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_b_a:
            z80.BC.b.h = z80.AF.b.h;
            break;
        case ld_b_b:
            break;
        case ld_b_byte:
            z80.BC.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_b_c:
            z80.BC.b.h = z80.BC.b.l;
            break;
        case ld_b_d:
            z80.BC.b.h = z80.DE.b.h;
            break;
        case ld_b_e:
            z80.BC.b.h = z80.DE.b.l;
            break;
        case ld_b_h:
            z80.BC.b.h = z80.IY.b.h;
            break;
        case ld_b_l:
            z80.BC.b.h = z80.IY.b.l;
            break;
        case ld_b_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_c_a:
            z80.BC.b.l = z80.AF.b.h;
            break;
        case ld_c_b:
            z80.BC.b.l = z80.BC.b.h;
            break;
        case ld_c_byte:
            z80.BC.b.l = read_mem(z80.PC.w.l++);
            break;
        case ld_c_c:
            break;
        case ld_c_d:
            z80.BC.b.l = z80.DE.b.h;
            break;
        case ld_c_e:
            z80.BC.b.l = z80.DE.b.l;
            break;
        case ld_c_h:
            z80.BC.b.l = z80.IY.b.h;
            break;
        case ld_c_l:
            z80.BC.b.l = z80.IY.b.l;
            break;
        case ld_c_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_de_word:
            z80.DE.b.l = read_mem(z80.PC.w.l++);
            z80.DE.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_d_a:
            z80.DE.b.h = z80.AF.b.h;
            break;
        case ld_d_b:
            z80.DE.b.h = z80.BC.b.h;
            break;
        case ld_d_byte:
            z80.DE.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_d_c:
            z80.DE.b.h = z80.BC.b.l;
            break;
        case ld_d_d:
            break;
        case ld_d_e:
            z80.DE.b.h = z80.DE.b.l;
            break;
        case ld_d_h:
            z80.DE.b.h = z80.IY.b.h;
            break;
        case ld_d_l:
            z80.DE.b.h = z80.IY.b.l;
            break;
        case ld_d_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_e_a:
            z80.DE.b.l = z80.AF.b.h;
            break;
        case ld_e_b:
            z80.DE.b.l = z80.BC.b.h;
            break;
        case ld_e_byte:
            z80.DE.b.l = read_mem(z80.PC.w.l++);
            break;
        case ld_e_c:
            z80.DE.b.l = z80.BC.b.l;
            break;
        case ld_e_d:
            z80.DE.b.l = z80.DE.b.h;
            break;
        case ld_e_e:
            break;
        case ld_e_h:
            z80.DE.b.l = z80.IY.b.h;
            break;
        case ld_e_l:
            z80.DE.b.l = z80.IY.b.l;
            break;
        case ld_e_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_hl_mword:
            LD16_MEM(IY);
            break;
        case ld_hl_word:
            z80.IY.b.l = read_mem(z80.PC.w.l++);
            z80.IY.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_h_a:
            z80.IY.b.h = z80.AF.b.h;
            break;
        case ld_h_b:
            z80.IY.b.h = z80.BC.b.h;
            break;
        case ld_h_byte:
            z80.IY.b.h = read_mem(z80.PC.w.l++);
            break;
        case ld_h_c:
            z80.IY.b.h = z80.BC.b.l;
            break;
        case ld_h_d:
            z80.IY.b.h = z80.DE.b.h;
            break;
        case ld_h_e:
            z80.IY.b.h = z80.DE.b.l;
            break;
        case ld_h_h:
            break;
        case ld_h_l:
            z80.IY.b.h = z80.IY.b.l;
            break;
        case ld_h_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_l_a:
            z80.IY.b.l = z80.AF.b.h;
            break;
        case ld_l_b:
            z80.IY.b.l = z80.BC.b.h;
            break;
        case ld_l_byte:
            z80.IY.b.l = read_mem(z80.PC.w.l++);
            break;
        case ld_l_c:
            z80.IY.b.l = z80.BC.b.l;
            break;
        case ld_l_d:
            z80.IY.b.l = z80.DE.b.h;
            break;
        case ld_l_e:
            z80.IY.b.l = z80.DE.b.l;
            break;
        case ld_l_h:
            z80.IY.b.l = z80.IY.b.h;
            break;
        case ld_l_l:
            break;
        case ld_l_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
        }
        break;
        case ld_mbc_a:
            write_mem(z80.BC.w.l, z80.AF.b.h);
            break;
        case ld_mde_a:
            write_mem(z80.DE.w.l, z80.AF.b.h);
            break;
        case ld_mhl_a:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
        }
        break;
        case ld_mhl_b:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
        }
        break;
        case ld_mhl_byte:
        {
            signed char o = read_mem(z80.PC.w.l++);
            tUBYTE b = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, b);
        }
        break;
        case ld_mhl_c:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
        }
        break;
        case ld_mhl_d:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
        }
        break;
        case ld_mhl_e:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
        }
        break;
        case ld_mhl_h:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
        }
        break;
        case ld_mhl_l:
        {
            signed char o = read_mem(z80.PC.w.l++);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
        }
        break;
        case ld_mword_a:
        {
            tREGPAIR addr;
            addr.b.l = read_mem(z80.PC.w.l++);
            addr.b.h = read_mem(z80.PC.w.l++);
            write_mem(addr.w.l, z80.AF.b.h);
        }
        break;
        case ld_mword_hl:
            LDMEM_16(IY);
            break;
        case ld_pc_hl:
            z80.PC.w.l = z80.IY.w.l;
            break;
        case ld_sp_hl:
            z80.SP.w.l = z80.IY.w.l;
            iWSAdjust++;
            break;
        case ld_sp_word:
            z80.SP.b.l = read_mem(z80.PC.w.l++);
            z80.SP.b.h = read_mem(z80.PC.w.l++);
            break;
        case nop:
            break;
        case or_a:
            OR(z80.AF.b.h);
            break;
        case or_b:
            OR(z80.BC.b.h);
            break;
        case or_byte:
            OR(read_mem(z80.PC.w.l++));
            break;
        case or_c:
            OR(z80.BC.b.l);
            break;
        case or_d:
            OR(z80.DE.b.h);
            break;
        case or_e:
            OR(z80.DE.b.l);
            break;
        case or_h:
            OR(z80.IY.b.h);
            break;
        case or_l:
            OR(z80.IY.b.l);
            break;
        case or_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            OR(read_mem(z80.IY.w.l + o));
        }
        break;
        case outa:
        {
            tREGPAIR p;
            p.b.l = read_mem(z80.PC.w.l++);
            p.b.h = z80.AF.b.h;
            z80_OUT_handler(p, z80.AF.b.h);
        }
        break;
        case pfx_cb:
            z80_pfx_fdcb();
            break;
        case pfx_dd:
            z80_pfx_dd();
            break;
        case pfx_ed:
            z80_pfx_ed();
            break;
        case pfx_fd:
            z80_pfx_fd();
            break;
        case pop_af:
            POP(AF);
            break;
        case pop_bc:
            POP(BC);
            break;
        case pop_de:
            POP(DE);
            break;
        case pop_hl:
            POP(IY);
            break;
        case push_af:
            PUSH(AF);
            break;
        case push_bc:
            PUSH(BC);
            break;
        case push_de:
            PUSH(DE);
            break;
        case push_hl:
            PUSH(IY);
            break;
        case ret:
            RET;
            break;
        case ret_c:
            if (z80.AF.b.l & Cflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_m:
            if (z80.AF.b.l & Sflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_nc:
            if (!(z80.AF.b.l & Cflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_nz:
            if (!(z80.AF.b.l & Zflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_p:
            if (!(z80.AF.b.l & Sflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_pe:
            if (z80.AF.b.l & Pflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_po:
            if (!(z80.AF.b.l & Pflag))
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case ret_z:
            if (z80.AF.b.l & Zflag)
            {
                iCycleCount += cc_ex[bOpCode];
                RET
            }
            else
            {
                iWSAdjust++;
            };
            break;
        case rla:
            RLA;
            break;
        case rlca:
            RLCA;
            break;
        case rra:
            RRA;
            break;
        case rrca:
            RRCA;
            break;
        case rst00:
            RST(0x0000);
            break;
        case rst08:
            RST(0x0008);
            break;
        case rst10:
            RST(0x0010);
            break;
        case rst18:
            RST(0x0018);
            break;
        case rst20:
            RST(0x0020);
            break;
        case rst28:
            RST(0x0028);
            break;
        case rst30:
            RST(0x0030);
            break;
        case rst38:
            RST(0x0038);
            break;
        case sbc_a:
            SBC(z80.AF.b.h);
            break;
        case sbc_b:
            SBC(z80.BC.b.h);
            break;
        case sbc_byte:
            SBC(read_mem(z80.PC.w.l++));
            break;
        case sbc_c:
            SBC(z80.BC.b.l);
            break;
        case sbc_d:
            SBC(z80.DE.b.h);
            break;
        case sbc_e:
            SBC(z80.DE.b.l);
            break;
        case sbc_h:
            SBC(z80.IY.b.h);
            break;
        case sbc_l:
            SBC(z80.IY.b.l);
            break;
        case sbc_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            SBC(read_mem(z80.IY.w.l + o));
        }
        break;
        case scf:
            z80.AF.b.l = (z80.AF.b.l & (Sflag | Zflag | Pflag)) | Cflag |
                         (z80.AF.b.h & Xflags);
            break;
        case sub_a:
            SUB(z80.AF.b.h);
            break;
        case sub_b:
            SUB(z80.BC.b.h);
            break;
        case sub_byte:
            SUB(read_mem(z80.PC.w.l++));
            break;
        case sub_c:
            SUB(z80.BC.b.l);
            break;
        case sub_d:
            SUB(z80.DE.b.h);
            break;
        case sub_e:
            SUB(z80.DE.b.l);
            break;
        case sub_h:
            SUB(z80.IY.b.h);
            break;
        case sub_l:
            SUB(z80.IY.b.l);
            break;
        case sub_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            SUB(read_mem(z80.IY.w.l + o));
        }
        break;
        case xor_a:
            XOR(z80.AF.b.h);
            break;
        case xor_b:
            XOR(z80.BC.b.h);
            break;
        case xor_byte:
            XOR(read_mem(z80.PC.w.l++));
            break;
        case xor_c:
            XOR(z80.BC.b.l);
            break;
        case xor_d:
            XOR(z80.DE.b.h);
            break;
        case xor_e:
            XOR(z80.DE.b.l);
            break;
        case xor_h:
            XOR(z80.IY.b.h);
            break;
        case xor_l:
            XOR(z80.IY.b.l);
            break;
        case xor_mhl:
        {
            signed char o = read_mem(z80.PC.w.l++);
            XOR(read_mem(z80.IY.w.l + o));
        }
        break;
    }
}

void Z80::z80_pfx_fdcb()
{
    signed char o;
    register tUBYTE bOpCode;

    o = read_mem(z80.PC.w.l++); // offset
    bOpCode = read_mem(z80.PC.w.l++);
    iCycleCount += cc_xycb[bOpCode];
    switch (bOpCode)
    {
        case bit0_a:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_b:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_c:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_d:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_e:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_h:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_l:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit0_mhl:
            BIT_XY(0, read_mem(z80.IY.w.l + o));
            break;
        case bit1_a:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_b:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_c:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_d:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_e:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_h:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_l:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit1_mhl:
            BIT_XY(1, read_mem(z80.IY.w.l + o));
            break;
        case bit2_a:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_b:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_c:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_d:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_e:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_h:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_l:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit2_mhl:
            BIT_XY(2, read_mem(z80.IY.w.l + o));
            break;
        case bit3_a:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_b:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_c:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_d:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_e:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_h:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_l:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit3_mhl:
            BIT_XY(3, read_mem(z80.IY.w.l + o));
            break;
        case bit4_a:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_b:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_c:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_d:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_e:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_h:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_l:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit4_mhl:
            BIT_XY(4, read_mem(z80.IY.w.l + o));
            break;
        case bit5_a:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_b:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_c:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_d:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_e:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_h:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_l:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit5_mhl:
            BIT_XY(5, read_mem(z80.IY.w.l + o));
            break;
        case bit6_a:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_b:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_c:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_d:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_e:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_h:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_l:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit6_mhl:
            BIT_XY(6, read_mem(z80.IY.w.l + o));
            break;
        case bit7_a:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_b:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_c:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_d:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_e:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_h:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_l:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case bit7_mhl:
            BIT_XY(7, read_mem(z80.IY.w.l + o));
            break;
        case res0_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(0, z80.AF.b.h));
            break;
        case res0_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(0, z80.BC.b.h));
            break;
        case res0_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(0, z80.BC.b.l));
            break;
        case res0_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(0, z80.DE.b.h));
            break;
        case res0_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(0, z80.DE.b.l));
            break;
        case res0_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(0, z80.HL.b.h));
            break;
        case res0_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(0, z80.HL.b.l));
            break;
        case res0_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(0, b));
        }
        break;
        case res1_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(1, z80.AF.b.h));
            break;
        case res1_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(1, z80.BC.b.h));
            break;
        case res1_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(1, z80.BC.b.l));
            break;
        case res1_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(1, z80.DE.b.h));
            break;
        case res1_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(1, z80.DE.b.l));
            break;
        case res1_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(1, z80.HL.b.h));
            break;
        case res1_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(1, z80.HL.b.l));
            break;
        case res1_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(1, b));
        }
        break;
        case res2_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(2, z80.AF.b.h));
            break;
        case res2_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(2, z80.BC.b.h));
            break;
        case res2_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(2, z80.BC.b.l));
            break;
        case res2_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(2, z80.DE.b.h));
            break;
        case res2_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(2, z80.DE.b.l));
            break;
        case res2_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(2, z80.HL.b.h));
            break;
        case res2_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(2, z80.HL.b.l));
            break;
        case res2_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(2, b));
        }
        break;
        case res3_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(3, z80.AF.b.h));
            break;
        case res3_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(3, z80.BC.b.h));
            break;
        case res3_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(3, z80.BC.b.l));
            break;
        case res3_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(3, z80.DE.b.h));
            break;
        case res3_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(3, z80.DE.b.l));
            break;
        case res3_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(3, z80.HL.b.h));
            break;
        case res3_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(3, z80.HL.b.l));
            break;
        case res3_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(3, b));
        }
        break;
        case res4_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(4, z80.AF.b.h));
            break;
        case res4_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(4, z80.BC.b.h));
            break;
        case res4_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(4, z80.BC.b.l));
            break;
        case res4_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(4, z80.DE.b.h));
            break;
        case res4_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(4, z80.DE.b.l));
            break;
        case res4_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(4, z80.HL.b.h));
            break;
        case res4_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(4, z80.HL.b.l));
            break;
        case res4_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(4, b));
        }
        break;
        case res5_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(5, z80.AF.b.h));
            break;
        case res5_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(5, z80.BC.b.h));
            break;
        case res5_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(5, z80.BC.b.l));
            break;
        case res5_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(5, z80.DE.b.h));
            break;
        case res5_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(5, z80.DE.b.l));
            break;
        case res5_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(5, z80.HL.b.h));
            break;
        case res5_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(5, z80.HL.b.l));
            break;
        case res5_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(5, b));
        }
        break;
        case res6_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(6, z80.AF.b.h));
            break;
        case res6_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(6, z80.BC.b.h));
            break;
        case res6_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(6, z80.BC.b.l));
            break;
        case res6_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(6, z80.DE.b.h));
            break;
        case res6_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(6, z80.DE.b.l));
            break;
        case res6_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(6, z80.HL.b.h));
            break;
        case res6_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(6, z80.HL.b.l));
            break;
        case res6_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(6, b));
        }
        break;
        case res7_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = RES(7, z80.AF.b.h));
            break;
        case res7_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = RES(7, z80.BC.b.h));
            break;
        case res7_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = RES(7, z80.BC.b.l));
            break;
        case res7_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = RES(7, z80.DE.b.h));
            break;
        case res7_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = RES(7, z80.DE.b.l));
            break;
        case res7_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = RES(7, z80.HL.b.h));
            break;
        case res7_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = RES(7, z80.HL.b.l));
            break;
        case res7_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RES(7, b));
        }
        break;
        case rlc_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = RLC(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case rlc_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = RLC(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case rlc_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = RLC(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case rlc_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = RLC(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case rlc_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = RLC(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case rlc_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = RLC(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case rlc_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = RLC(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case rlc_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RLC(b));
        }
        break;
        case rl_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = RL(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case rl_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = RL(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case rl_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = RL(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case rl_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = RL(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case rl_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = RL(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case rl_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = RL(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case rl_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = RL(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case rl_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RL(b));
        }
        break;
        case rrc_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = RRC(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case rrc_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = RRC(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case rrc_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = RRC(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case rrc_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = RRC(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case rrc_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = RRC(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case rrc_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = RRC(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case rrc_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = RRC(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case rrc_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RRC(b));
        }
        break;
        case rr_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = RR(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case rr_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = RR(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case rr_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = RR(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case rr_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = RR(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case rr_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = RR(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case rr_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = RR(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case rr_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = RR(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case rr_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, RR(b));
        }
        break;
        case set0_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(0, z80.AF.b.h));
            break;
        case set0_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(0, z80.BC.b.h));
            break;
        case set0_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(0, z80.BC.b.l));
            break;
        case set0_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(0, z80.DE.b.h));
            break;
        case set0_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(0, z80.DE.b.l));
            break;
        case set0_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(0, z80.HL.b.h));
            break;
        case set0_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(0, z80.HL.b.l));
            break;
        case set0_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(0, b));
        }
        break;
        case set1_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(1, z80.AF.b.h));
            break;
        case set1_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(1, z80.BC.b.h));
            break;
        case set1_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(1, z80.BC.b.l));
            break;
        case set1_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(1, z80.DE.b.h));
            break;
        case set1_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(1, z80.DE.b.l));
            break;
        case set1_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(1, z80.HL.b.h));
            break;
        case set1_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(1, z80.HL.b.l));
            break;
        case set1_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(1, b));
        }
        break;
        case set2_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(2, z80.AF.b.h));
            break;
        case set2_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(2, z80.BC.b.h));
            break;
        case set2_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(2, z80.BC.b.l));
            break;
        case set2_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(2, z80.DE.b.h));
            break;
        case set2_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(2, z80.DE.b.l));
            break;
        case set2_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(2, z80.HL.b.h));
            break;
        case set2_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(2, z80.HL.b.l));
            break;
        case set2_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(2, b));
        }
        break;
        case set3_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(3, z80.AF.b.h));
            break;
        case set3_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(3, z80.BC.b.h));
            break;
        case set3_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(3, z80.BC.b.l));
            break;
        case set3_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(3, z80.DE.b.h));
            break;
        case set3_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(3, z80.DE.b.l));
            break;
        case set3_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(3, z80.HL.b.h));
            break;
        case set3_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(3, z80.HL.b.l));
            break;
        case set3_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(3, b));
        }
        break;
        case set4_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(4, z80.AF.b.h));
            break;
        case set4_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(4, z80.BC.b.h));
            break;
        case set4_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(4, z80.BC.b.l));
            break;
        case set4_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(4, z80.DE.b.h));
            break;
        case set4_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(4, z80.DE.b.l));
            break;
        case set4_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(4, z80.HL.b.h));
            break;
        case set4_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(4, z80.HL.b.l));
            break;
        case set4_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(4, b));
        }
        break;
        case set5_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(5, z80.AF.b.h));
            break;
        case set5_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(5, z80.BC.b.h));
            break;
        case set5_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(5, z80.BC.b.l));
            break;
        case set5_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(5, z80.DE.b.h));
            break;
        case set5_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(5, z80.DE.b.l));
            break;
        case set5_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(5, z80.HL.b.h));
            break;
        case set5_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(5, z80.HL.b.l));
            break;
        case set5_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(5, b));
        }
        break;
        case set6_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(6, z80.AF.b.h));
            break;
        case set6_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(6, z80.BC.b.h));
            break;
        case set6_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(6, z80.BC.b.l));
            break;
        case set6_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(6, z80.DE.b.h));
            break;
        case set6_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(6, z80.DE.b.l));
            break;
        case set6_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(6, z80.HL.b.h));
            break;
        case set6_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(6, z80.HL.b.l));
            break;
        case set6_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(6, b));
        }
        break;
        case set7_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.AF.b.h = SET(7, z80.AF.b.h));
            break;
        case set7_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.h = SET(7, z80.BC.b.h));
            break;
        case set7_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.BC.b.l = SET(7, z80.BC.b.l));
            break;
        case set7_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.h = SET(7, z80.DE.b.h));
            break;
        case set7_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.DE.b.l = SET(7, z80.DE.b.l));
            break;
        case set7_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.h = SET(7, z80.HL.b.h));
            break;
        case set7_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, z80.HL.b.l = SET(7, z80.HL.b.l));
            break;
        case set7_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SET(7, b));
        }
        break;
        case sla_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = SLA(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case sla_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = SLA(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case sla_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = SLA(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case sla_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = SLA(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case sla_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = SLA(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case sla_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = SLA(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case sla_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = SLA(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case sla_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SLA(b));
        }
        break;
        case sll_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = SLL(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case sll_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = SLL(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case sll_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = SLL(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case sll_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = SLL(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case sll_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = SLL(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case sll_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = SLL(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case sll_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = SLL(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case sll_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SLL(b));
        }
        break;
        case sra_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = SRA(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case sra_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = SRA(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case sra_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = SRA(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case sra_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = SRA(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case sra_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = SRA(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case sra_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = SRA(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case sra_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = SRA(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case sra_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SRA(b));
        }
        break;
        case srl_a:
            z80.AF.b.h = read_mem(z80.IY.w.l + o);
            z80.AF.b.h = SRL(z80.AF.b.h);
            write_mem(z80.IY.w.l + o, z80.AF.b.h);
            break;
        case srl_b:
            z80.BC.b.h = read_mem(z80.IY.w.l + o);
            z80.BC.b.h = SRL(z80.BC.b.h);
            write_mem(z80.IY.w.l + o, z80.BC.b.h);
            break;
        case srl_c:
            z80.BC.b.l = read_mem(z80.IY.w.l + o);
            z80.BC.b.l = SRL(z80.BC.b.l);
            write_mem(z80.IY.w.l + o, z80.BC.b.l);
            break;
        case srl_d:
            z80.DE.b.h = read_mem(z80.IY.w.l + o);
            z80.DE.b.h = SRL(z80.DE.b.h);
            write_mem(z80.IY.w.l + o, z80.DE.b.h);
            break;
        case srl_e:
            z80.DE.b.l = read_mem(z80.IY.w.l + o);
            z80.DE.b.l = SRL(z80.DE.b.l);
            write_mem(z80.IY.w.l + o, z80.DE.b.l);
            break;
        case srl_h:
            z80.HL.b.h = read_mem(z80.IY.w.l + o);
            z80.HL.b.h = SRL(z80.HL.b.h);
            write_mem(z80.IY.w.l + o, z80.HL.b.h);
            break;
        case srl_l:
            z80.HL.b.l = read_mem(z80.IY.w.l + o);
            z80.HL.b.l = SRL(z80.HL.b.l);
            write_mem(z80.IY.w.l + o, z80.HL.b.l);
            break;
        case srl_mhl:
        {
            tUBYTE b = read_mem(z80.IY.w.l + o);
            write_mem(z80.IY.w.l + o, SRL(b));
        }
        break;
    }
}
