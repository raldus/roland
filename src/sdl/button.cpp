/***************************************************************************
 *   Copyright (C) 2005-2013 by Fred Klaus <development@fkweb.de>          *
 *                                                                         *
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
#include "button.h"

namespace sdltk
{

	Button::Button() : Label()
	{
		mDown = false;
		mHighlightColor.set(mColor.r()+30, mColor.g()+30, mColor.b(), 225);
	}

	Button::~Button()
	{
		if (mImage) delete mImage;
	}
	
	void Button::onMouseMotion(SDL_MouseMotionEvent * event)
	{
		string tmp;
		tmp  = "x:"  + mDraw->numberToText(event->x);
		tmp += "/y:" + mDraw->numberToText(event->y);
		setText(tmp);
		
		move(event);
	}
	
	void Button::onMouseButton(SDL_MouseButtonEvent * event)
	{
		if ((event->type == SDL_MOUSEBUTTONDOWN) && (event->button == SDL_BUTTON_LEFT))
		{
			Color tmp;
			tmp = mBorderColor1;
			mBorderColor1 = mBorderColor2;
			mBorderColor2 = tmp;
			mTextOffset.set(1, 1);
			mDown = true;
		}
		
		if ((event->type == SDL_MOUSEBUTTONUP) && (event->button == SDL_BUTTON_LEFT))
		{
			mDown = false;
		}
		
		moveInit(event);
	}
	
	void Button::draw()
	{
		mDraw->begin();
		
		if (!mDown || !mMouseOver)
		{
			mTextOffset.set(0, 0);
			setBorder(mBorder);
		}
		
		drawBorder();
		if (mMouseOver) drawBackground(mHighlightColor);
		else drawBackground(mColor);
		drawText();
		
		mDraw->end();
	}

} //namespace sdltk
