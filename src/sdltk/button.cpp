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
#include "button.h"

namespace sdltk
{

    Button::Button() : Label()
    {
        mDown = false;
        mHighlightColor.set(mColor.r() + 10, mColor.g() + 10, mColor.b(), 200);
    }

    void Button::onMouseMotion(SDL_MouseMotionEvent * event)
    {
        //String tmp;
        //tmp  = "x:"  + std::to_string(event->x);
        //tmp += "/y:" + std::to_string(event->y);
        //IOUT("[sdltk::Button]", "Info ", tmp);

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
            Color tmp;
            tmp = mBorderColor1;
            mBorderColor1 = mBorderColor2;
            mBorderColor2 = tmp;
            mDown = false;
        }

        moveInit(event);
    }

    void Button::draw()
    {
        mCanvas->begin();

        if (!mDown || !mMouseOver)
        {
            mTextOffset.set(0, 0);
        }

        drawBorder();
        if (mMouseOver) drawBackground(mHighlightColor);
        else drawBackground(mColor);
        drawText();

        mCanvas->end();
    }

} // sdltk
