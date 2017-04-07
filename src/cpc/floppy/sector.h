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
#ifndef DISKSECTOR_H
#define DISKSECTOR_H

#include "compspec.h"
#include "types.h"

/** @author Fred Klaus */
class Sector final
{
public:
    Sector();
    ~Sector() ROLAND_DEFAULT

    tUBYTE* CHRN()       {return mCHRN;}
    tUBYTE CHRN(int num) {return mCHRN[num];}
    tUBYTE* flags()      {return mFlags;}
    tUBYTE flag(int num) {return mFlags[num];}
    uint size() {return mSize;}
    tUBYTE* data()       {return mData;}

    void setData(tUBYTE* data)      {mData=data;}
    void setFlag(int idx, int val) {mFlags[idx]=val;}
    void setSize(uint num) {mSize=num;}

private:
    tUBYTE mCHRN[4];     // the CHRN for this sector
    tUBYTE mFlags[4];    // ST1 and ST2 - reflects any possible error conditions
    uint mSize; // sector size in bytes
    tUBYTE* mData;       // pointer to sector data

};

#endif
