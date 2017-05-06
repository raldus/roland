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
#include "events.h"
#include "def.h"

namespace sdltk
{

    void List::init()
    {
        mSelected   = begin();
        mEnabled    = true;
        mWantEvents = true;
        mMotion     = 0;
        mSpeed      = 5;

/*      Label * label = new Label(this);
        label->setColor(128, 128, 128, 144);
        label->setPos(100, 100);
        label->setText("Fileselect");
        label->setSize(mGui->video()->size().width(), 55);
        label->setEnabled(true);
        label->setBackground(true);
        mGui->add(label);
*/
        mPosH       = 0;
    }

    void List::onMouseButton(SDL_MouseButtonEvent * event)
    {
        if (!mEnabled || empty()) return;

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
        if (!mEnabled || empty()) return false;
        bool ret = false;
        mClock.init();

        if (event->type == SDL_KEYDOWN)
        {
            switch(static_cast<int>(event->keysym.sym))
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
            switch(static_cast<int>(event->keysym.sym))
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

    bool List::onUser(SDL_UserEvent * event) // Selection ... TODO Change to infinite list
    {
        if (!mEnabled || empty()) return false;
        if (event->code == UserEvent::ListItemClicked)
        {
            auto selected = find(begin(), end(), (ListItem*) event->data1);
            if (selected != end())
            {
                (*mSelected)->setColor(mTmpColor);
                mSelected = selected;
                mTmpColor = (*mSelected)->color();
            }

            IOUT("UserEvent", "ListItemClicked", "received");
            return true;
        }
        if (event->code == UserEvent::ListItemDoubleClicked)
        {
            auto selected = find(begin(), end(), (ListItem*) event->data1);
            if (selected != end())
            {
                mSelected = selected;
            }


            IOUT("UserEvent", "ListItemDoubleClicked", "received");
            return true;
        }
        return false;
    }

    void List::add(ListItem * item)
    {
        if (std::list<ListItem*>::size() > 3200) return; // TODO implement infinite list
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
        if (!mEnabled || empty()) return;
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
        if (!mEnabled || empty()) return;

        if (mMotion == 1) reposition(mSpeed);
        else
        if (mMotion == 2) reposition(-mSpeed);

        (*mSelected)->setColor(164, 148, 128, 164);

        mCanvas->setClipRect(&mRect);
        mCanvas->begin();

        mCanvas->setColor(mColor);
        mCanvas->rect(mRect);

        for (auto item : *this)
        {
            //EOUT("[sdltk::List]", "Item.pos.y", item->pos().y());
            //EOUT("[sdltk::List]", "List.height", height());
            //if ((item->pos().y() < 0) || (item->pos().y() > height())) item->setEnabled(false);
            //if (item->pos().y() < 0) continue;
            if (item->pos().y() > height() + (item->height()*2)) break;
            if (item->enabled())
            {
                if (!item->hasMouseGrab()) item->reset();
                item->draw();
            }
        }
        mCanvas->end();
        mCanvas->clearClipRect();
    }

} // sdltk
