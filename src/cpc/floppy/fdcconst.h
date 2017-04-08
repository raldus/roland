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
#ifndef CPC_FDCCONST_H
#define CPC_FDCCONST_H

// FDC constants
#define DSK_BPTMAX      8192
#define DSK_TRACKMAX    102   // max amount that fits in a DSK header
#define DSK_SIDEMAX     2
#define DSK_SECTORMAX   29    // max amount that fits in a track header

#define FDC_TO_CPU      0
#define CPU_TO_FDC      1

#define CMD_PHASE       0
#define EXEC_PHASE      1
#define RESULT_PHASE    2

#define SKIP_flag       1     // skip sectors with DDAM/DAM
#define SEEKDRVA_flag   2     // seek operation has finished for drive A
#define SEEKDRVB_flag   4     // seek operation has finished for drive B
#define RNDDE_flag      8     // simulate random DE sectors
#define OVERRUN_flag    16    // data transfer timed out
#define SCAN_flag       32    // one of the three scan commands is active
#define SCANFAILED_flag 64    // memory and sector data does not match
#define STATUSDRVA_flag 128   // status change of drive A
#define STATUSDRVB_flag 256   // status change of drive B

// more

#define CMD_CODE  0
#define CMD_UNIT  1
#define CMD_C     2
#define CMD_H     3
#define CMD_R     4
#define CMD_N     5
#define CMD_EOT   6
#define CMD_GPL   7
#define CMD_DTL   8
#define CMD_STP   8

#define RES_ST0   0
#define RES_ST1   1
#define RES_ST2   2
#define RES_C     3
#define RES_H     4
#define RES_R     5
#define RES_N     6

#define OVERRUN_TIMEOUT 128*4
#define INITIAL_TIMEOUT OVERRUN_TIMEOUT*4
#define MAX_CMD_COUNT 15


// #define ERR_INPUT_INIT           1
// #define ERR_VIDEO_INIT           2
// #define ERR_VIDEO_SET_MODE       3
// #define ERR_VIDEO_SURFACE        4
// #define ERR_VIDEO_PALETTE        5
// #define ERR_VIDEO_COLOUR_DEPTH   6
// #define ERR_AUDIO_INIT           7
// #define ERR_AUDIO_RATE           8
#define ERR_OUT_OF_MEMORY        9
// #define ERR_CPC_ROM_MISSING      10
// #define ERR_NOT_A_CPC_ROM        11
// #define ERR_ROM_NOT_FOUND        12
#define ERR_FILE_NOT_FOUND       13
#define ERR_FILE_BAD_ZIP         14
#define ERR_FILE_EMPTY_ZIP       15
#define ERR_FILE_UNZIP_FAILED    16
// #define ERR_SNA_INVALID          17
// #define ERR_SNA_SIZE             18
// #define ERR_SNA_CPC_TYPE         19
// #define ERR_SNA_WRITE            20
#define ERR_DSK_INVALID          21
#define ERR_DSK_SIDES            22
#define ERR_DSK_SECTORS          23
#define ERR_DSK_WRITE            24
#define MSG_DSK_ALTERED          25
// #define ERR_TAP_INVALID          26
// #define ERR_TAP_UNSUPPORTED      27
// #define ERR_TAP_BAD_VOC          28
// #define ERR_PRINTER              29
// #define ERR_BAD_MF2_ROM          30
// #define ERR_SDUMP                31


#endif
