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
#ifndef CPC_KEYBOARD_H
#define CPC_KEYBOARD_H

#include "types.h"
#include "cpckeys.h"

/** @author Fred Klaus */
class Keyboard final
{

public:
    Keyboard();
    ~Keyboard() = default;

    void init();

    tUBYTE value()           const {return mLine[mRow];}
    tUBYTE value(tUBYTE row) const {return mLine[row];}
    tUBYTE row()             const {return mRow;}

    void setRow(tUBYTE row)   {mRow = row;}
    void setValue(tUBYTE val) {mLine[mRow] = val;}
    void setValue(tUBYTE row, tUBYTE val) {mLine[row] = val;}

private:
    tUBYTE mRow;
    tUBYTE mLine[16];
};

#endif // CPC_KEYBOARD_H
