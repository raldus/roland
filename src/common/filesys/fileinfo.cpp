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
#include "fileinfo.h"


FileInfo::FileInfo(const string & filename)
{
    mValid=read(filename, false);
}

bool FileInfo::read(const string & fname, bool followlink)
{
    #ifdef _WIN32
        return mValid = stat(fname.c_str(), &mStat) ? false : true;
    #else
        if (followlink) return mValid=stat(fname.c_str(), &mStat) ? false : true;
        else return mValid = lstat(fname.c_str(), &mStat) ? false : true;
    #endif

}

