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
#include "videogl.h"

#include "glfuncs.h"

namespace sdltk
{

    volatile bool VideoGL::mIsLoaded=false;

    VideoGL::VideoGL(Cpc * cpc) : Video(cpc)
    {
        mCpcScale  = 1;
        mCpcWidth  = CPC_VISIBLE_SCR_WIDTH  * mCpcScale;
        mCpcHeight = CPC_VISIBLE_SCR_HEIGHT * mCpcScale; // @todo doublescan ??? -- change this

        mTexWidth  = 1024;
        mTexHeight = 512;
        mTexnum    = 0;

        mScreen=0;
    }

    VideoGL::~VideoGL()
    {
        quit();
    }

    void VideoGL::quit()
    {
        if (mBuffer)
        {
            SDL_FreeSurface(mBuffer);
            mBuffer=0;
            IOUT("[SDLVidGL]", "backbuffer", "destroyed");
        }
        if (mScreen)
        {
            SDL_FreeSurface(mScreen);
            mScreen=0;
            IOUT("[SDLVidGL]", "screenbuffer", "destroyed");
        }
        if (mTexnum)
        {
            if (oglIsTexture(mTexnum))
            {
                oglDeleteTextures(1, &mTexnum);
                mTexnum = 0;
                IOUT("[SDLVidGL]", "texture", "destroyed");
            }
        }
        IOUT("[SDLVidGL]", "video", "quit");
    }

    int VideoGL::init()
    {
        return init(0, 0, 0, mFullscreen, mCpcScale);
    }

    int VideoGL::init(uint width, uint height, uint depth, bool fullscreen, unsigned char scale)
    {
#ifdef _WIN32
        const char *gl_library = "OpenGL32.DLL";
#else
        const char *gl_library = "libGL.so.1";
#endif

        if (!mIsLoaded)
        {
            SDL_ClearError();
            if (SDL_GL_LoadLibrary(gl_library) != 0)
            {
                EOUT("[SDLVidGL]", "could not load OpenGL library", "abort");
                mIsLoaded = false;
                return -1;
            }
            else
            {
                mIsLoaded = true;
                init_glfuncs();
                IOUT("[SDLVidGL]", gl_library, "loaded");
            }
        }


        quit();

        mFullscreen = fullscreen;
        mCpcScale   = 2; // scale only width
        //scale=1;

        mCpcWidth  = CPC_VISIBLE_SCR_WIDTH; // * scale;
        mCpcHeight = CPC_VISIBLE_SCR_HEIGHT; // @todo doublescan ??? -- change this
        if (!fullscreen)
        {
            width  = mCpcWidth*2;
            height = mCpcHeight*2;
        }
        else
        {
            if (!width)  width  = mCpcWidth;
            if (!height) height = mCpcHeight; // @todo +1 ?? doublescan ??? -- change this

            width  = 1280;
            height = 1024;
        }


        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        //SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_HWPALETTE|
        uint desiredFlags = SDL_OPENGL|SDL_HWSURFACE | (fullscreen ? SDL_FULLSCREEN : 0);
        /*
        #ifndef _WIN32
            desiredFlags|=SDL_RESIZABLE;
        #endif
        */

        SDL_ClearError();
        mScreen = SDL_SetVideoMode(width, height, depth, desiredFlags);

        if (mScreen == NULL)
        {
            EOUT("[SDLVidGL]",  "couldn't set" << width << "x" << height << "-" << mScreen->format->BitsPerPixel << " video mode: ", SDL_GetError());
            SDL_Quit();
            return -1;
        }
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        //if (fullscreen) SDL_ShowCursor (0); else SDL_ShowCursor (1);

        mCanvas.x = 0;
        mCanvas.y = 0;
        mCanvas.w = mCpcWidth;
        mCanvas.h = mCpcHeight;

        /*
        int major, minor;
        const char *version;
        version = (char *) eglGetString(GL_VERSION);
        if (sscanf(version, "%d.%d", &major, &minor) != 2) {
            fprintf(stderr, "Unable to get OpenGL version\n");
            return NULL;
        }*/

        GLint max_texsize;
        oglGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texsize);
        IOUT("[SDLVidGL]", "max texturesize", max_texsize);
        if (max_texsize < 1024)
        {
            EOUT("[SDLVidGL]", "max texturesize is below 1024", "abort init");
            return -1;
        }

        oglDisable(GL_FOG);
        oglDisable(GL_LIGHTING);
        oglDisable(GL_CULL_FACE);
        oglDisable(GL_DEPTH_TEST);
        oglDisable(GL_BLEND);
        oglDisable(GL_NORMALIZE);
        oglDisable(GL_ALPHA_TEST);
        oglDisable(GL_SCISSOR_TEST);
        oglEnable(GL_TEXTURE_2D);

        oglGenTextures(1, &mTexnum);
        oglBindTexture(GL_TEXTURE_2D, mTexnum);
        oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilter ? GL_LINEAR : GL_NEAREST); //GL_LINEAR:  GL_NEAREST
        oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilter ? GL_LINEAR : GL_NEAREST);
        oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mTexWidth, mTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        oglViewport(0, 0, width, height);
        oglMatrixMode(GL_PROJECTION);
        oglLoadIdentity();
        oglOrtho(0, width, height, 0, -1.0f, 1.0f);

        oglMatrixMode(GL_MODELVIEW);
        oglLoadIdentity();

        mBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, CPC_VISIBLE_SCR_WIDTH*mCpcScale, CPC_VISIBLE_SCR_HEIGHT, 24, 0,0,0,0);
        if (!mBuffer)
        {
            EOUT("[SDLVidGL]",  "couldn't create BufferSurface", "abort init");
            return -1;
        }


        //*** Video @todo 24bit OK ?
        //lock(); // <-not used with SWSurface
        char buf[16];
        memset(buf, 0, 16);
        SDL_VideoDriverName(buf, 15);
        string driver=buf;


        if (mBuffer->format->BitsPerPixel == 16) mCpc->vdu().setBpp(Vdu::Bpp16);
        if (mBuffer->format->BitsPerPixel == 24) mCpc->vdu().setBpp(Vdu::Bpp24);
        if (mBuffer->format->BitsPerPixel == 32) mCpc->vdu().setBpp(Vdu::Bpp32);

        mBufferStart = calcScreenStart(); // @todo maybe better to calculate everytime -> flipping ???
        mBufferEnd   = calcScreenEnd();

        int v=0;
        IOUT("[SDLVidGL]", "driver", driver);
        IOUT("[SDLVidGL]", "width", width);
        IOUT("[SDLVidGL]", "height", height);
        IOUT("[SDLVidGL]", "Screen bpp", (int) mScreen->format->BitsPerPixel);
        IOUT("[SDLVidGL]", "Buffer bpp", (int) mBuffer->format->BitsPerPixel);
        SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &v);
        IOUT("[SDLVidGL]", "SDL_GL_DOUBLEBUFFER", boolalpha << (bool) v);
        SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &v);
        IOUT("[SDLVidGL]", "SDL_GL_DEPTH_SIZE", v);
        IOUT("[SDLVidGL]", "SDL_OPENGL", boolalpha << (bool) (mScreen->flags & SDL_OPENGL));
        IOUT("[SDLVidGL]", "startadr", calcScreenStart());
        IOUT("[SDLVidGL]", "endadr",   calcScreenEnd());
        IOUT("[SDLVidGL]", "init", "done");

        mCpc->colours().setDepth(mBuffer->format->BitsPerPixel);
        // BUG: mCpc->vdu().setScale(mCpcScale);
        // BUG: mCpc->vdu().setLineDoubling(false);
        mCpc->vdu().setScrLineOffset((mBuffer->pitch/4)*1); //*mCpcScale
        mCpc->vdu().setScrBase(mBufferStart);
        mCpc->vdu().setScrEnd (mBufferEnd);

        mDraw.setSurface(mScreen);

        //unlock(); // <-not used with SWSurface
        return 0;
    }

    void VideoGL::setup()
    {
        oglDisable(GL_BLEND);
        oglEnable(GL_TEXTURE_2D);

        oglBindTexture(GL_TEXTURE_2D, mTexnum);
        oglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mBuffer->w, mBuffer->h, GL_BGR, GL_UNSIGNED_BYTE, mBuffer->pixels);
        //oglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mGui->widget(0)->buffer()->w, mGui->widget(0)->buffer()->h, GL_BGRA, GL_UNSIGNED_BYTE, mGui->widget(0)->buffer()->pixels);
        oglBegin(GL_QUADS);
        oglColor4ub(255,255,255,255);

        oglTexCoord2f(0.f, 0.f);
        oglVertex2i(0, 0);

        oglTexCoord2f(0.f, (float)(mBuffer->h)/mTexHeight);
        oglVertex2i(0, mScreen->h);

        oglTexCoord2f((float)(mBuffer->w)/mTexWidth, (float)(mBuffer->h)/mTexHeight);

        oglVertex2i(mScreen->w, mScreen->h);

        oglTexCoord2f((float)(mBuffer->w)/mTexWidth, 0.f);
        oglVertex2i(mScreen->w, 0);
        oglEnd();

        //drawGui();
        //SDL_GL_SwapBuffers();

        //mBufferStart = calcScreenStart(); // @todo maybe better to calculate everytime -> flipping ???
        //mBufferEnd   = calcScreenEnd();
        //mCpc->vdu().setScrBase(mBufferStart);
        //mCpc->vdu().setScrEnd (mBufferEnd);
    }

    uint* VideoGL::calcScreenStart()
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

    uint* VideoGL::calcScreenEnd()
    {
        if (mBuffer->format->BitsPerPixel == 16)
        {
            return (uint*) (unsigned short int*) mBuffer->pixels+((mBuffer->pitch / 2) * (mBuffer->h));
        }
        else
        {
            return (uint*) mBuffer->pixels+((mBuffer->pitch / 4) * (mBuffer->h));
        }
    }


} //sdltk
