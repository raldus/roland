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
#ifndef SDLVIDEO_H
#define SDLVIDEO_H

#include "predef.h"

#include "canvas.h"
#include "cpc.h"
#include "SDL.h"

#include <string>

/** @brief covers SDL-functions */
namespace sdltk
{

    //! Baseclass for all SDL Video drivers @author Fred Klaus development@fkweb.de
    class Video
    {
    public:
        Video(Cpc* cpc);
        virtual ~Video() {};

        /** device dependant init for inheritance */
        virtual int  init() = 0;
        /** inits all @param width = width @param height = height */
        virtual int  init(uint width, uint height, uint depth, bool fullscreen, unsigned char scale) = 0;

        virtual Canvas * getCanvas()  = 0;

        /** updates the Screen (flip or something) */
        virtual void update()  = 0;
        /** free all resources */
        virtual void quit()    = 0;
        /** locks the screen-surface */
        virtual void lock()    {};
        /** unlocks the screen-surface */
        virtual void unlock()  {};

        virtual void setup() {};

        uint bpp()   {return mBuffer->format->BitsPerPixel;}
        uint depth() {return mBuffer->format->BytesPerPixel;}

        void setFullscreen(bool value) {mFullscreen = value; init();}
        void setDoubling  (bool value) {mDoubling   = value;}
        void setFilter    (bool value) {mFilter     = value; init();}

        void toggleFullscreen() {mFullscreen = (mFullscreen ? false : true); init();}
        void toggleDoubling  () {mDoubling   = (mDoubling   ? false : true); init();}
        void toggleFilter    () {mFilter     = (mFilter     ? false : true); init();}

        uint* bufferStart()   {return mBufferStart;}
        uint* bufferEnd()     {return mBufferEnd;}

        SDL_Surface* buffer() {return mBuffer;}
        SDL_Surface* screen() {return mScreen;}

        static void setIcon   (const string & icon);
        static void setCaption(const string & caption) {SDL_WM_SetCaption(caption.c_str(), caption.c_str());}

    protected:
        Cpc* mCpc;

        static unsigned int mDesktopWidth;
        static unsigned int mDesktopHeight;

        static bool mFullscreen;
        static bool mDoubling;
        static bool mFilter;

        unsigned int  mCpcWidth;
        unsigned int  mCpcHeight;
        unsigned char mCpcScale;

        unsigned int * mBufferStart;
        unsigned int * mBufferEnd;

        SDL_Surface * mBuffer;
        SDL_Surface * mScreen;
        SDL_Rect      mCanvasRect;

    };

} //sdltk

#endif
