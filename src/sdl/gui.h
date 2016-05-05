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
#ifndef GUI_H
#define GUI_H

#include "widget.h"
#include "video.h"
#include "SDL.h"

#include <vector>


/** @brief the SDL based GUI */
namespace sdltk
{

    using std::vector;

    /** @author Fred Klaus development@fkweb.de */
    class Gui
    {
    public:
        Gui(Video * video);
        ~Gui();

        void add(Widget * widget);

        /** returns true if event is accepted */
        bool checkEvent(SDL_Event * event);
        void update();

        bool enabled()                {return mEnabled;}
        void setEnabled(bool enabled) {mEnabled = enabled;}
        void toggleEnabled()          {mEnabled = !mEnabled;}

        void setFocus(Widget * focus) {mHasFocus = focus;}

    protected:
        vector<Widget*> mWidgets;
        vector<Widget*>::iterator mIt;
        bool mEnabled;
        Widget * mHasFocus; // receives the incoming events

        Video * mVideo;
    };

} // sdltk

#endif // GUI_H
