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

//! @brief covers SDL-functions
namespace sdltk
{

    //! Baseclass for all SDL Video drivers @author Fred Klaus development@fkweb.de
    class Video
    {
    public:
        //! Constructs the video interface for the \param Cpc
        Video(Cpc* cpc);
        virtual ~Video() {};

        //! device dependant init for inheritance
        virtual int  init() = 0;
        //! inits all @param width = width @param: height = height
        virtual int  init(uint width, uint height, uint depth, bool fullscreen, unsigned char scale) = 0;

        virtual Canvas * getCanvas()  = 0;

        //! Things that needs to be done before update
        virtual void setup() {};
        //! Updates the Screen (flip or something)
        virtual void update()  = 0;
        //! Free all resources
        virtual void quit()    = 0;
        //! Locks the screen-surface
        virtual void lock()    {};
        //! Unlocks the screen-surface
        virtual void unlock()  {};
        //! Things that needs to be done before update

        //! Returns the bits per pixel (8 to 32)
        uint bpp()   {return mBuffer->format->BitsPerPixel;}
        //! Returns the bytes per pixel inbetween (1 to 4)
        uint depth() {return mBuffer->format->BytesPerPixel;}
        //! Returns the size of the video surface
        Size size()  {return Size(mScreen->w, mScreen->h);}

        //! Set to fullscreen or not
        void setFullscreen(bool value) {mFullscreen = value; init();}
        //! Set line doubling on or off
        void setDoubling  (bool value) {mDoubling   = value;}
        //! En-/disable bilinear texture filter
        void setFilter    (bool value) {mFilter     = value; init();}
        //! Toggles fullscreen
        void toggleFullscreen() {mFullscreen = (mFullscreen ? false : true); init();}
        //! Toggle line doubling
        void toggleDoubling  () {mDoubling   = (mDoubling   ? false : true); init();}
        //! Toggle bilinear texture filter
        void toggleFilter    () {mFilter     = (mFilter     ? false : true); init();}

        //! Returns the start of the backbuffer
        uint* bufferStart()   {return mBufferStart;}
        //! Returns the end of the backbuffer
        uint* bufferEnd()     {return mBufferEnd;}

        //! Returns the SDL_Surface of the backbuffer
        SDL_Surface* buffer() {return mBuffer;}
        //! Returns the SDL_Surface of the screen
        SDL_Surface* screen() {return mScreen;}

        //! Sets an image as application icon
        static void setIcon   (const string & icon);
        //! Sets a caption for the application
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
