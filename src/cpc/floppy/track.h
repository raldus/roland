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
#ifndef DISKTRACK_H
#define DISKTRACK_H

#include "types.h"
#include "sector.h"
#include "fdcconst.h"

#include <memory>

/** @author Fred Klaus */
class Track
{
public:
	Track() {memset(this, 0, sizeof(*this));}
	~Track() {}

	UBYTE* data()            {return mData;}
	uint sectors()   {return mSectors;}
	uint size()      {return mSize;}
	Sector & sector(int num) {return mSector[num];}

	void setSectors(uint sectors) {mSectors=sectors;}
	void setSize(uint size)       {mSize=size;}
	void setData(UBYTE* data)             {mData=data;}

	void clear()             {delete [] mData; mData=0;}
	void set(UBYTE num, int size)      {memset(mData, num, size);}

private:
	uint mSectors;         // sector count for this track
	uint mSize;            // track size in bytes
	UBYTE* mData;                  // pointer to track data
	Sector mSector[DSK_SECTORMAX]; // array of sector information structures

};

#endif
