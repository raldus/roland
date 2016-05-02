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
#include "videostd.h"

namespace sdltk
{

    VideoStd::VideoStd(Cpc * cpc) : Video(cpc)
    {
        mCpcScale  = 2;
        mCpcWidth  = CPC_VISIBLE_SCR_WIDTH  * mCpcScale;
        mCpcHeight = CPC_VISIBLE_SCR_HEIGHT * mCpcScale + 1; ///@todo doublescan ??? -- change this
    }

    VideoStd::~VideoStd()
    {
        quit();
    }

    void VideoStd::quit()
    {
        // dont do that following ???:
        if (mBuffer)
        {
            SDL_FreeSurface(mBuffer);
            mBuffer = 0;
            IOUT("[SDLVidStd]", "backbuffer", "destroyed");
        }
        IOUT("[SDLVidStd]", "video", "quit");

        mScreen=0;
    }

    int VideoStd::init()
    {
        return init(0, 0, 0, mFullscreen, mCpcScale);
    }

    int VideoStd::init(uint width, uint height, uint depth, bool fullscreen, unsigned char scale)
    {
        quit();
        mFullscreen = fullscreen;
        mCpcScale   = scale;

        mCpcWidth  = CPC_VISIBLE_SCR_WIDTH  * scale;
        mCpcHeight = CPC_VISIBLE_SCR_HEIGHT * scale + 1; ///@todo doublescan ??? -- change this
        if (!fullscreen)
        {
            width  = mCpcWidth;
            height = mCpcHeight;
        }
        else
        {
            if (!width)  width  = 800; //mCpcWidth;
            if (!height) height = 600; //mCpcHeight; ///@todo +1 ?? doublescan ??? -- change this
        }

        uint desiredFlags = SDL_SWSURFACE|SDL_ANYFORMAT| (fullscreen ? SDL_FULLSCREEN : 0);
        //SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_HWPALETTE|


        mBuffer = SDL_SetVideoMode(width, height, depth, desiredFlags);
        if (mBuffer == NULL)
        {
            EOUT("[SDLVidStd]",  "Couldn't set" << width << "x" << height << "-" << mBuffer->format->BitsPerPixel << " video mode: ", SDL_GetError());
            SDL_Quit();
            return 1;
        }

        SDL_FillRect(mBuffer, 0, SDL_MapRGBA(mBuffer->format, 0, 0, 0, 255)); // clear the Buffer

        char buf[16];
        memset(buf, 0, 16);
        SDL_VideoDriverName(buf, 15);
        string driver=buf;

        uint hp = (width  - mCpcWidth)  / 2;
        uint vp = (height - mCpcHeight) / 2;

        mCanvas.x = hp;
        mCanvas.y = vp;
        mCanvas.w = mCpcWidth;
        mCanvas.h = mCpcHeight;

        if (mBuffer->format->BitsPerPixel == 16) mCpc->vdu().setBpp(Vdu::Bpp16);
        if (mBuffer->format->BitsPerPixel == 24) mCpc->vdu().setBpp(Vdu::Bpp24);
        if (mBuffer->format->BitsPerPixel == 32) mCpc->vdu().setBpp(Vdu::Bpp32);

        mBufferStart = calcBufferStart(); // @todo maybe better to calculate everytime -> flipping ???
        mBufferEnd   = calcBufferEnd();

        IOUT("[SDLVidStd]", "driver", driver)
        IOUT("[SDLVidStd]", "width", width)
        IOUT("[SDLVidStd]", "height", height)
        IOUT("[SDLVidStd]", "bpp", (int) mBuffer->format->BitsPerPixel)
        IOUT("[SDLVidStd]", "SDL_SWSURFACE", boolalpha << (bool) (mBuffer->flags & SDL_SWSURFACE))
        IOUT("[SDLVidStd]", "SDL_HWSURFACE", boolalpha << (bool) (mBuffer->flags & SDL_HWSURFACE))
        IOUT("[SDLVidStd]", "SDL_DOUBLEBUF", boolalpha << (bool) (mBuffer->flags & SDL_DOUBLEBUF))
        IOUT("[SDLVidStd]", "SDL_HWPALETTE", boolalpha << (bool) (mBuffer->flags & SDL_HWPALETTE))
        IOUT("[SDLVidStd]", "startadr", calcBufferStart());
        IOUT("[SDLVidStd]", "endadr",   calcBufferEnd());
        IOUT("[SDLVidStd]", "init", "done");

        mCpc->colours().setDepth(mBuffer->format->BitsPerPixel);
        // BUG: mCpc->vdu().setScale(mCpcScale);
        // BUG: mCpc->vdu().setLineDoubling(mDoubling);
        mCpc->vdu().setScrLineOffset((mBuffer->pitch/4)*mCpcScale);
        mCpc->vdu().setScrBase(mBufferStart);
        mCpc->vdu().setScrEnd (mBufferEnd);

        mScreen = mBuffer;

        mDraw.setSurface(mScreen);

        return 0;
    }

    void VideoStd::update()
    {
        SDL_Flip(mBuffer);
    }

    uint* VideoStd::calcBufferStart()
    {
        if (mBuffer->format->BitsPerPixel == 16)
        {
            return (uint*) (unsigned short int*) mBuffer->pixels+((mBuffer->pitch / 2)*mCanvas.y)+mCanvas.x;
        }
        else
        {
            return  (uint*) mBuffer->pixels+((mBuffer->pitch / 4)*(mCanvas.y))+mCanvas.x;
        }
    }

    uint* VideoStd::calcBufferEnd()
    {
        if (mBuffer->format->BitsPerPixel == 16)
        {
            return (uint*) (unsigned short int*) mBuffer->pixels+((mBuffer->pitch / 2) * (mBuffer->h-1));
        }
        else
        {
            return (uint*) mBuffer->pixels+((mBuffer->pitch/4) * (mBuffer->h-1));
        }
    }


} //sdltk



