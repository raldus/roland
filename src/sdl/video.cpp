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
#include "video.h"

namespace sdltk
{

    bool  Video::mFullscreen = false;
    bool  Video::mDoubling   = true;
    bool  Video::mFilter     = true;
    uint  Video::mDesktopWidth  = 0;
    uint  Video::mDesktopHeight = 0;

    Video::Video(Cpc* cpc)
    {
        const SDL_VideoInfo* vinfo = SDL_GetVideoInfo();

        mDesktopWidth  = vinfo->current_w;
        mDesktopHeight = vinfo->current_h;

        mCpc     = cpc;
        mBuffer  = 0;
        mScreen  = 0;

        mCanvas.x    = 0;
        mCanvas.y    = 0;
        mCanvas.w    = 0;
        mCanvas.h    = 0;

        IOUT("[Video]", "desktopwidth",  mDesktopWidth);
        IOUT("[Video]", "desktopheight", mDesktopHeight);

    }
    
    void Video::setIcon(const string & icon)
    {
        SDL_Surface * surface = SDL_LoadBMP(icon.c_str());
        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 255, 0 ,255));
        SDL_WM_SetIcon(surface, 0);
    }

} //sdltk
