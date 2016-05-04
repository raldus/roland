/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus                                      *
 *   development@fkweb.de                                                  *
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
#include "list.h"

namespace sdltk
{

    List::List(Gui * gui)
    {
        mPosH = 0;
        mGui  = gui;
        mEnabled = true;
        mWantEvents = true;
        mMotion = 0;
        mSpeed  = 5;
    }

    List::~List()
    {
    }
    
    bool List::onKeyboard(SDL_KeyboardEvent * event)
    {
        if (event->type == SDL_KEYDOWN)
        {
            if ((int) event->keysym.sym == SDLK_UP)
            {
                mMotion = 1;
                return true;
            }
            if ((int) event->keysym.sym == SDLK_DOWN)
            {
                mMotion = 2;
                return true;
            }
        }
        else if (event->type == SDL_KEYUP)
        {
            if ((int) event->keysym.sym == SDLK_UP   && mMotion == 1) mMotion = 0;
            if ((int) event->keysym.sym == SDLK_DOWN && mMotion == 2) mMotion = 0;
        }
            
        return false;
    }
    
    void List::add(ListItem * item) 
    {
        mGui->add(item);
        item->setParent(this);
        item->setOrigin(mRect.x(), mRect.y() + mPosH);
        item->setPos(mRect.x(), mRect.y() + mPosH);
        item->setBorder(true);
        mRect.setWidth(item->width());
        mList.push_back(item);
        mPosH += item->height() + 1;
    }
    
    void List::reposition(Sint16 val)
    {
        Rect tmp = mRect;
        for (mIt = mList.begin(); mIt != mList.end(); ++mIt)
        {
            if (!(*mIt)->hasMouseGrab()) (*mIt)->reset();
            tmp.setHeight(mRect.height()+(*mIt)->height());
            tmp.setY(mRect.y()-(*mIt)->height());
            (*mIt)->setPos((*mIt)->x(), (*mIt)->y() + val);
            (*mIt)->setOrigin((*mIt)->x(), (*mIt)->y());
            if (!tmp.inside((*mIt)->x(), (*mIt)->y())) (*mIt)->setEnabled(false);
            else (*mIt)->setEnabled(true);
        }
    }
    
    void List::draw()
    {
        if (!mEnabled) return;
        
        mCanvas->setClipRect(&mRect);
        mCanvas->begin();
        
        mCanvas->setColor(mColor);
        mCanvas->rect(mRect);
        
        if (mMotion == 1) reposition(-mSpeed);
        else if (mMotion == 2) reposition(mSpeed);

        for (mIt = mList.begin(); mIt != mList.end(); ++mIt)
        {
            if ((*mIt)->enabled())
            {
                if (!(*mIt)->hasMouseGrab()) (*mIt)->reset();
                (*mIt)->draw();
            }
        }
        
        mCanvas->end();
        mCanvas->clearClipRect();
    }
    
} //namespace sdltk
