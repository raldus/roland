/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus                                      *
 *   frednet@web.de                                                        *
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
#include "fileselect.h"
#include "filename.h"


#include <cstring>


using std::string;

FileSelect::FileSelect(SDL_Surface* scrn, const string & dir, const string & last, const string & prefix)
{
    mDir     = 0;
    mScrn    = scrn;
    mDirname = dir;
    mLast    = last;
    mPrefix  = prefix;

    running  = true;
    abort    = false;

    rect.x = 12;
    rect.y = 12;
    rect.w = mScrn->w-24;
    rect.h = 18;

    openDir(dir);
    mDir->sort();
}

FileSelect::~FileSelect()
{
    closeDir();
    clear();
}

void FileSelect::openDir(const string & dir)
{
    if (mDir) closeDir();
    mDir=new Directory(dir, true);
    if (mDir->empty())
    {
        mDirIt=mDir->begin();
        return;
    }

    mDir->sort();

    for(mDirIt=mDir->begin(); mDirIt < mDir->end(); mDirIt++)
    {
        FileName fn = *mDirIt;
        fn.setPath(mDirname);
        if (fn == mLast) return;
    }

    mDirIt=mDir->begin();
}

void FileSelect::closeDir()
{
    if (mDir)
    {
        delete mDir;
        mDir=0;
    }
}

const string & FileSelect::filename()
{
    mFilename = *mDirIt;
    mFilename.setPath(mDirname);
    return mFilename;
}

bool FileSelect::loop()
{
    while(running)
    {
        events();
    }
    clear();
    if (abort) return 1;
    else return 0;
}
void FileSelect::clear()
{
    //SDL_FillRect(mScrn, &rect, 0);

    for (int iy=0; iy<rect.h; iy++)
    {
        for (int ix=0; ix<rect.w*mScrn->format->BytesPerPixel; ix++)
        {
            *(((char*)mScrn->pixels)
                + ((mScrn->pitch * (rect.y+iy)) )
                + ((rect.x*mScrn->format->BytesPerPixel)+ix)) = 0;
        }
    }
    if(SDL_MUSTLOCK(mScrn)) SDL_UnlockSurface(mScrn);
    SDL_Flip(mScrn);
    if(SDL_MUSTLOCK(mScrn))
        while (SDL_LockSurface(mScrn))
        {
            SDL_Delay(20);
        }
}

void FileSelect::display()
{
    for (int iy=0; iy<rect.h; iy++)
    {
        for (int ix=0; ix<rect.w*mScrn->format->BytesPerPixel; ix++)
        {
            *(((char*)mScrn->pixels)
                + ((mScrn->pitch * (rect.y+iy)) )
                + ((rect.x*mScrn->format->BytesPerPixel)+ix)) = 0;
        }
    }

    string msg=mPrefix+*mDirIt;

    mFont.write(mScrn, rect.x+2, rect.y+2, msg.c_str());

    if(SDL_MUSTLOCK(mScrn)) SDL_UnlockSurface(mScrn);
    SDL_Flip(mScrn);
    if(SDL_MUSTLOCK(mScrn))
        while (SDL_LockSurface(mScrn))
        {
            SDL_Delay(20);
        }
}

void FileSelect::events()
{
    SDL_Event event;
    long ksym;
    //SDLMod mod;

    // Check key events
    while(SDL_PollEvent(&event))
    {
        ksym = event.key.keysym.sym;
        //mod  = event.key.keysym.mod;

        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(ksym)
                {
                    case SDLK_UP:
                        if (mDirIt < mDir->end()-1) mDirIt++;
                        break;

                    case SDLK_DOWN:
                        if (mDirIt > mDir->begin()) mDirIt--;
                        break;

                    case SDLK_LEFT:
                        if (mDirIt > mDir->begin()) mDirIt--;
                        break;

                    case SDLK_RIGHT:
                        if (mDirIt < mDir->end()-1) mDirIt++;
                        break;

                    case SDLK_PAGEUP:
                        for(int i=0; i<10; i++)
                            if (mDirIt < mDir->end()-1) mDirIt++;
                        break;

                    case SDLK_PAGEDOWN:
                        for(int i=0; i<10; i++)
                            if (mDirIt > mDir->begin()) mDirIt--;
                        break;

                    case SDLK_HOME:
                        mDirIt=mDir->begin();
                        break;

                    case SDLK_END:
                        mDirIt=mDir->end()-1;
                        break;

                    case SDLK_ESCAPE:
                        break;

                    case SDLK_RETURN:
                        //running=false;
                        break;

                    default:
                    {
                        char kc[2] ={0, 0};
                        char tmp[2]={0, 0};
                        if ((ksym >= SDLK_a) && (ksym <= SDLK_z))
                        {
                            kc[0]=(char) ksym;
                            for(Directory::iterator dirIt=mDir->begin(); dirIt < mDir->end(); dirIt++)
                            {
                                tmp[0]=(*dirIt).at(0);
                                if (strcasecmp(tmp, kc) == 0)
                                {
                                    mDirIt=dirIt;
                                    break;
                                }
                            }
                        }
                        else if ((ksym >= SDLK_0) && (ksym <= SDLK_9))
                        {
                            kc[0]=(char) ksym;
                            for(Directory::iterator dirIt=mDir->begin(); dirIt < mDir->end(); dirIt++)
                            {
                                tmp[0]=(*dirIt).at(0);
                                if (strcasecmp(tmp, kc) == 0)
                                {
                                    mDirIt=dirIt;
                                    break;
                                }
                            }
                        }
                    }
                        break;
                }

            case SDL_KEYUP:
                switch(ksym)
                {
                    case SDLK_UP:
                        break;

                    case SDLK_DOWN:
                        break;

                    case SDLK_LEFT:
                        break;

                    case SDLK_RIGHT:
                        break;

                    case SDLK_ESCAPE:
                        running=false;
                        abort=true;
                        break;

                    case SDLK_RETURN:
                        running=false;
                        break;
                }

            default:
                break;
        }
        display();
    }
}





