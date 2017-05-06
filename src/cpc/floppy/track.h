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
#ifndef CPC_DISKTRACK_H
#define CPC_DISKTRACK_H

#include "types.h"
#include "sector.h"
#include "fdcconst.h"

#include <cstring>

namespace cpcx
{
    //! A track of a disk
    class Track ROLAND_FINAL
    {
    public:
        Track() {std::memset(this, 0, sizeof(*this));}
        ~Track() ROLAND_DEFAULT

        tUBYTE*  data()    const {return mData;}
        uint     sectors() const {return mSectors;}
        uint     size()    const {return mSize;}
        Sector & sector(int num) {return mSector[num];}

        void setSectors(uint sectors)  {mSectors = sectors;}
        void setSize(uint size)        {mSize = size;}
        void setData(tUBYTE* data)     {mData = data;}
        void set(tUBYTE num, int size) {memset(mData, num, size);}

        void clear() {delete [] mData; mData = nullptr;}

    private:
        uint    mSectors;               //! sector count for this track
        uint    mSize;                  //! track size in bytes
        tUBYTE* mData;                  //! pointer to track data
        Sector  mSector[DSK_SECTORMAX]; //! array of sector information structures
    };
} // namespace cpcx

#endif // CPC_DISKTRACK_H
