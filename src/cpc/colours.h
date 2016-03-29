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
#ifndef COLOURS_H
#define COLOURS_H

#include "types.h"

/** @author Fred Klaus */
class Colours
{

public:
	Colours();
	~Colours() {}

	void init();
	
	void setIntensity(uint intensity) {mIntensity = intensity; init();}
	void setDepth    (uint depth)     {mDepth     = depth;     init();}
	void setMonitor  (uint monitor)   {mMonitor   = monitor;   init();}

	UDWORD get(int num) {return mTable[num];}

private:
	static double mColour[32][3];
	static double mGreen [32];
	UDWORD        mTable [32];
	
	uint mDepth;
	uint mIntensity;
	uint mMonitor;
};

#endif
