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
#ifndef RGBUTTON_H
#define RGBUTTON_H

#include "label.h"
#include "rect.h"
#include "point.h"
#include "color.h"
#include <string>

namespace sdltk
{
	using namespace sdltk;
	using std::string;

	/** @author Fred Klaus development@fkweb.de */
	class Button : public Label
	{
	public:
		Button();
		virtual ~Button();
		
		virtual void draw();
		
		virtual void onMouseMotion (SDL_MouseMotionEvent * event);
		virtual void onMouseButton (SDL_MouseButtonEvent * event);
		
		void setHighliteColor(const Color & color) {mHighlightColor = color;}
		
	private:
		Color mHighlightColor;
		bool  mDown;
		
	};

} //namespace sdltk

#endif //RGBUTTON_H