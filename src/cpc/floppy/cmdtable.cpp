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
#include "cmdtable.h"

// #include "fdc.h"
//
// CmdTable::CmdTable()
// {
//
//  entry[0].set (0x03, 3, 0, FDC_TO_CPU, Fdc::specify); // specify
//  entry[1].set (0x04, 2, 1, FDC_TO_CPU, Fdc::drvstat); // sense device status
//  entry[2].set (0x07, 2, 0, FDC_TO_CPU, Fdc::recalib); // recalibrate
//  entry[3].set (0x08, 1, 2, FDC_TO_CPU, Fdc::intstat); // sense interrupt
//  status
//  entry[4].set (0x0f, 3, 0, FDC_TO_CPU, Fdc::seek);    // seek
//  entry[5].set (0x42, 9, 7, FDC_TO_CPU, Fdc::readtrk); // read diagnostic
//  entry[6].set (0x45, 9, 7, CPU_TO_FDC, Fdc::write);   // write data
//  entry[7].set (0x46, 9, 7, FDC_TO_CPU, Fdc::read);    // read data
//  entry[8].set (0x49, 9, 7, CPU_TO_FDC, Fdc::write);   // write deleted data
//  entry[9].set (0x4a, 2, 7, FDC_TO_CPU, Fdc::readID);  // read id
//  entry[10].set(0x4c, 9, 7, FDC_TO_CPU, Fdc::read);    // read deleted data
//  entry[11].set(0x4d, 6, 7, CPU_TO_FDC, Fdc::writeID); // write id
//  entry[12].set(0x51, 9, 7, CPU_TO_FDC, Fdc::scan);    // scan equal
//  entry[13].set(0x59, 9, 7, CPU_TO_FDC, Fdc::scan);    // scan low or equal
//  entry[14].set(0x5d, 9, 7, CPU_TO_FDC, Fdc::scan);    // scan high or equal
// }
