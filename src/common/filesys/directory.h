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
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "filename.h"

#include <vector>

using std::vector;

/// [std::vector] for directory entrys
/** This is a vector to retrieve, store and sort FileName types.
    No slicing can occure by passing pointers of Directorys, because no additional
    variables are added at all.
    @todo excludeflag for hidden files !!
    @todo maybe use a set rather than a vector, to sort entries
        this is needed in FileName :
        bool operator<(const ... & right) const {return mTestOffset > right.mTestOffset;}
    \n\n<B>Example:</B>\n @include "directory.cpp"
    @author Fred Klaus */
class Directory : public vector<FileName>
{

public:
    Directory() : vector<FileName>() {}
    Directory(const string & path, bool subdir=false, bool owndir=false);
    ~Directory() {}

    void scan(const string & path, bool subdir=false, bool owndir=false);

    /** Sorts this vector by filename.
        If you dont need to change the default behaviour (case insensitive), use this.
        @todo implement dirsfirst flag */
    void sort();
    /** @overload
        Is NOT REALLY THREADSAFE because of using a static function in FileName.
        @param casesensitive = Sets whether sorting is case sensitive or not.
        @see <B>static</B> stk::FileName::setCaseSensitiveCompare */
    void sort(bool casesensitive);
};

#endif
