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
#include "gui.h"

namespace sdltk
{

    Gui::Gui(Video * video)
    {
        SDL_EnableUNICODE(1);
        mEnabled  = false;
        mHasFocus = 0;
        mVideo    = video;
    }
    
    Gui::~Gui()
    {
        for (mIt = mWidgets.begin(); mIt < mWidgets.end(); mIt++)
        {
            delete (*mIt);
        }
    }
    
    void Gui::add(Widget * widget)
    {
        widget->setDraw(mVideo->getDraw());
        mWidgets.push_back(widget);
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
        
        for (mIt = mWidgets.begin(); mIt < mWidgets.end(); mIt++)
        {
            if ((*mIt)->wantEvents())
            {
                switch(event->type)
                {
                    case SDL_MOUSEMOTION:
                        if ((*mIt)->hasMouseGrab() || (*mIt)->rect().inside(event->motion.x, event->motion.y))
                        {
                            (*mIt)->setMouseOver(true);
                            (*mIt)->onMouseMotion(&event->motion);
                            ret = true;
                        }
                        else (*mIt)->setMouseOver(false);
                        break;
                        
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEBUTTONDOWN:
                        if ((*mIt)->rect().inside(event->button.x, event->button.y))
                        {
                            (*mIt)->onMouseButton(&event->button);
                            ret = true;
                        }
                        break;
                        
                    default:
                        (*mIt)->setMouseOver(false);
                        break;
                }
            }
        }
        return ret;
    }
    
    void Gui::update()
    {
        if (!mEnabled) return;
        
        for (mIt = mWidgets.begin(); mIt < mWidgets.end(); mIt++)
        {
            if (((*mIt)->parent() == 0) && (*mIt)->enabled()) (*mIt)->draw();
        }
    }

} //namespace sdltk



