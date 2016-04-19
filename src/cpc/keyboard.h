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
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "cpckeys.h"

/** @author Fred Klaus */
class Keyboard
{
public:
    Keyboard();
    ~Keyboard();

    void init();

    UBYTE value() {return mLine[mRow];}
    UBYTE value(UBYTE row) {return mLine[row];}
    UBYTE row() {return mRow;}

    void setRow(UBYTE row) {mRow=row;}
    void setValue(UBYTE val) {mLine[mRow]=val;}
    void setValue(UBYTE row, UBYTE val) {mLine[row]=val;}

private:
    UBYTE mRow;
    UBYTE mLine[16];
};

#endif
