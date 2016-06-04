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
#include "gui.h"

namespace sdltk
{

    Gui::Gui(Video * video) : mEnabled(false), mHasFocus(0), mVideo(video)
    {
        SDL_EnableUNICODE(1);
    }

    Gui::~Gui()
    {
        for (auto it : *this) delete it;
    }

    void Gui::add(Widget * widget)
    {
        widget->setCanvas(mVideo->getCanvas());
        push_back(widget);
        mHasFocus = widget;
    }

    bool Gui::checkEvent(SDL_Event * event)
    {
        if (!mEnabled) return false;

        SDL_Event ev2;
        int i = SDL_PeepEvents(&ev2, 1, SDL_PEEKEVENT, SDL_MOUSEMOTIONMASK);
        if ((i > 0) && (ev2.type == event->type)) return true;

        bool ret = false;

        if (((event->type == SDL_KEYUP) || (event->type == SDL_KEYDOWN)) && mHasFocus)
        {
            return mHasFocus->onKeyboard(&event->key);
        }

        for (auto it : *this)
        {
            if (it->wantEvents())
            {
                switch(event->type)
                {
                    case SDL_MOUSEMOTION:
                        if (it->hasMouseGrab() || it->rect().inside(event->motion.x, event->motion.y))
                        {
                            it->setMouseOver(true);
                            it->onMouseMotion(&event->motion);
                            ret = true;
                        }
                        else it->setMouseOver(false);
                        break;

                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEBUTTONDOWN:
                        if (it->rect().inside(event->button.x, event->button.y))
                        {
                            it->onMouseButton(&event->button);
                            ret = true;
                        }
                        break;

                    case SDL_USEREVENT:
                        it->onUser(&event->user);
                        ret = true;
                        break;

                    default:
                        it->setMouseOver(false);
                        break;
                }
            }
        }
        return ret;
    }

    void Gui::update()
    {
        if (!mEnabled) return;

        for (auto it : *this)
        {
            if ((it->parent() == nullptr) && it->enabled()) it->draw();
        }
    }

} //namespace sdltk



