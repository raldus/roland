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
#include "list.h"

namespace sdltk
{

    void List::init()
    {
        mPosH       = 0;
        mEnabled    = true;
        mWantEvents = true;
        mMotion     = 0;
        mSpeed      = 5;
        SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    }

    void List::onMouseButton(SDL_MouseButtonEvent * event)
    {
        if (!mEnabled) return;

        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            switch((int) event->button)
            {
                case SDL_BUTTON_WHEELUP:
                    mSpeed  = 24;
                    mMotion = 1;
                    break;

                case SDL_BUTTON_WHEELDOWN:
                    mSpeed  = 24;
                    mMotion = 2;
                    break;
            }
        }
    }

    bool List::onKeyboard(SDL_KeyboardEvent * event)
    {
        if (!mEnabled) return false;
        bool ret = false;
        mClock.init();

        if (event->type == SDL_KEYDOWN)
        {
            switch((int) event->keysym.sym)
            {
                case SDLK_UP:
                    mSpeed  = 16;
                    mMotion = 1;
                    ret = true;
                    (*mSelected)->setColor(mTmpColor);
                    if (mSelected != begin()) mSelected--;
                    mTmpColor = (*mSelected)->color();
                    break;

                case SDLK_PAGEUP:
                    mSpeed  = 32;
                    mMotion = 1;
                    ret = true;
                    break;

                case SDLK_DOWN:
                    mSpeed  = 16;
                    mMotion = 2;
                    ret = true;
                    (*mSelected)->setColor(mTmpColor);
                    if (mSelected != --end()) mSelected++;
                    mTmpColor = (*mSelected)->color();
                    break;

                case SDLK_PAGEDOWN:
                    mSpeed  = 32;
                    mMotion = 2;
                    ret = true;
                    break;

                case SDLK_HOME:
                    init();
                    ret = true;
                    break;
            }
        }
        else if (event->type == SDL_KEYUP)
        {
            switch((int) event->keysym.sym)
            {
                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_PAGEUP:
                case SDLK_PAGEDOWN:
                    mMotion = 0;
                    ret = true;
                    break;
            }
        }
        return ret;
    }

    void List::add(ListItem * item)
    {
        mGui->add(item);

        item->setParent(this);
        item->setOrigin(x(), y() + mPosH);
        item->setPos   (x(), y() + mPosH);
        item->setBorder(true);
        push_back(item);

        mPosH += item->height() + 1;
        mRect.setWidth(item->width() + 2); // autowidth

        mSelected = begin();
        mTmpColor = (*mSelected)->color();

    }

    void List::reposition(Sint16 val)
    {
        mClock.init();

        if (front()->y() >= y() && mMotion == 1)
        {
            return;
        }

        if ( back()->y() - back()->height() <= height() && mMotion == 2)
        {
            return;
        }

        val = val + mClock.elapsed() * (mMotion-2);
        Rect tmp = mRect;

        for (auto item : *this)
        {
            //if (!item->hasMouseGrab()) item->reset();

            tmp.setHeight(height() + item->height()); //because one scrolls out
            tmp.setY(y() - item->height());

            item->setPos(item->x(), item->y() + val);
            item->setOrigin(item->x(), item->y());
            item->setEnabled(tmp.inside(item->x(), item->y()));

            // move selection, keep inside
            if (!item->enabled() && item == (*mSelected))
            {
                if (mMotion == 1 && mSelected != begin())
                {
                    (*mSelected)->setColor(mTmpColor);
                    while (!(*mSelected)->enabled() && mSelected != begin())
                    {
                        mSelected--;
                    }
                    if (mSelected != begin()) mSelected--;
                    mTmpColor = (*mSelected)->color();
                }
                else if (mMotion == 2 && mSelected != --end())
                {
                    (*mSelected)->setColor(mTmpColor);
                    while (!(*mSelected)->enabled() && mSelected != --end())
                    {
                        mSelected++;
                    }
                    if (mSelected != --end()) mSelected++;
                    mTmpColor = (*mSelected)->color();
                }
            }
        }
        if (mSpeed > 0) mSpeed--;
    }

    void List::draw()
    {
        if (!mEnabled) return;

        if (mMotion == 1) reposition(mSpeed);
        else if (mMotion == 2) reposition(-mSpeed);

        (*mSelected)->setColor(164, 148, 128, 164);

        mCanvas->setClipRect(&mRect);
        mCanvas->begin();

        mCanvas->setColor(mColor);
        mCanvas->rect(mRect);

        for (auto item : *this)
        {
            if (item->enabled())
            {
                if (!item->hasMouseGrab()) item->reset();
                item->draw();
            }
        }
        mCanvas->end();
        mCanvas->clearClipRect();
    }

} //namespace sdltk
