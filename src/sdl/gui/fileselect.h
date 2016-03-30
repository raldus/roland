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
#ifndef FILESELECT_H
#define FILESELECT_H

#include <string>

#include "font.h"
#include "directory.h"
#include "SDL.h"

/** @author Fred Klaus */
class FileSelect
{
public:
    FileSelect(SDL_Surface* scrn, const string & dir, const string & last, const string & prefix="File: ");
    ~FileSelect();

    void openDir(const string &  dir);
    void closeDir();

    const string & filename();

    bool loop();

private:
    Directory*   mDir;
    Directory::iterator mDirIt;
    SDL_Surface* mScrn;

    SDL_Rect rect;

    Font mFont;

    string mDirname;
    string mPrefix;
    FileName mFilename;
    FileName mLast;

    bool running;
    bool abort;

    void clear();
    void display();
    void events();

};

#endif
