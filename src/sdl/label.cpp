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
#include "label.h"

namespace sdltk
{

    Label::Label() : Widget()
    {
        mImage = 0;
        mText.clear();
        mRect.set(0, 0, 0, 0);
        mRelativeMousePos.set(0, 0);
        mTextOffset.set(0, 0);
        mBackground = true;
        mBorder = false;
        mWantEvents = true;
    }

    Label::~Label()
    {
        if (mImage) delete mImage;
    }
    
    void Label::onMouseMotion(SDL_MouseMotionEvent * event)
    {
        move(event);
    }
    
    void Label::onMouseButton(SDL_MouseButtonEvent * event)
    {
        moveInit(event);
    }
    
    void Label::moveInit(SDL_MouseButtonEvent * event)
    {
        if ((event->type == SDL_MOUSEBUTTONDOWN) && (event->button == SDL_BUTTON_MIDDLE))
        {
            relativePos(event->x, event->y, mRelativeMousePos);
            mMouseGrab = true;
        }
        
        if ((event->type == SDL_MOUSEBUTTONUP) && (event->button == SDL_BUTTON_MIDDLE))
        {
            mRelativeMousePos.set(0, 0);
            mMouseGrab = false;
        }
    }
    
    void Label::move(SDL_MouseMotionEvent * event)
    {
        if (event->state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        {
            mRect.setX(event->x - mRelativeMousePos.x());
            mRect.setY(event->y - mRelativeMousePos.y());
            setBorder(mBorder);
        }
    }
    
    void Label::setBorder(bool val)
    {
        mBorder = val;
        
        if (mBorder)
        {
            mCanvas.setX(mRect.x() + 1);
            mCanvas.setY(mRect.y() + 1);
            mCanvas.setWidth( mRect.width()  - 2);
            mCanvas.setHeight(mRect.height() - 2);
            
            mLeftTop.set(mRect.pos());
            mRightTop.set(mRect.x() + mRect.width(), mRect.y());
            mRightBottom.set(mRect.x() + mRect.width(), mRect.y() + mRect.height() );
            mLeftBottom.set(mRect.x(), mRect.y() + mRect.height() );
            
            mBorderColor1.set(150, 150, 150, 230);
            mBorderColor2.set(20, 20, 20, 230);
        }
        else mCanvas = mRect;
    }
    
    void Label::setImage(const string & fname, bool autosize, bool bg)
    {
        mBackground = bg;
        
        mImage = new Image(fname);
        if (autosize) 
        {
            mRect.setSize(mImage->size());
            setBorder(false);
        }
    }
    
    void Label::drawBorder()
    {
        if (mBorder)
        {
            mDraw->setColor(mBorderColor2);
            mDraw->line(mLeftBottom, mRightBottom);
            mDraw->line(mRightTop, mRightBottom);
            mDraw->setColor(mBorderColor1);
            mDraw->line(mLeftTop, mRightTop);
            mDraw->line(mLeftTop, mLeftBottom);
        }
    }
    
    void Label::drawBackground()
    {
        if (mBackground)
        {
            mDraw->setColor(mColor);
            mDraw->fill(mCanvas);
        }
        if (mImage) mDraw->image(mImage, mRect.pos());
    }
    
    void Label::drawBackground(const Color & color)
    {
        if (mBackground)
        {
            mDraw->setColor(color);
            mDraw->fill(mCanvas);
        }
        if (mImage) mDraw->image(mImage, mRect.pos());
    }
    
    void Label::drawText()
    {
        if (!mText.empty())
        {
            mTextPos = mCanvas.pos();
            mTextPos.setX(mCanvas.x() + ((mCanvas.width()  - mDraw->textSize(mText).width()) / 2 ) + mTextOffset.x());
            mTextPos.setY(mCanvas.y() + ((mCanvas.height() - mDraw->textHeight()) / 2) + 1 + mTextOffset.y());
            mDraw->write(mTextPos, mText);
        }
    }
    
    void Label::draw()
    {
        mDraw->begin();
        
        drawBorder();
        drawBackground();
        drawText();
        
        mDraw->end();
    }

} //namespace sdltk
