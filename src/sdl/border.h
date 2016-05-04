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
#ifndef BORDER_H
#define BORDER_H

#include "predef.h"
#include "types.h"

#include "SDL.h"

namespace sdltk
{

/** @author Fred Klaus development@fkweb.de */
class Border
{
public:
	enum BorderPos 
	{
		bpTop          = 0,
		bpBottom       = 1,
		bpLeft         = 2,
		bpRight        = 3,
		bpLeftTop      = 4,
		bpLeftBottom   = 5,
		bpRightTop     = 6,
		bpRightBottom  = 7,
	};
	
	void setColor(uint color);
	void setUniSize(uint width, uint height, uint thickness);
	void paint(SDL_Surface * surf);
	
	Border();
	~Border();
	
	
protected:
	SDL_Rect mBorder[8];
	
	uint mColor;

};

} //sdltk

#endif
