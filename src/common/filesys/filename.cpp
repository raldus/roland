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
#include "filename.h"

#include <cstring>

using std::string;

bool FileName::mCaseSensitive = false;

bool FileName::operator<(const FileName &fn)
{
    if (mCaseSensitive)
        return (this->compare(fn) < 0);
    else
        return (strcasecmp(this->c_str(), fn.c_str()) < 0);
}

bool FileName::operator==(const FileName &fn)
{
    if (mCaseSensitive)
        return (this->compare(fn) == 0);
    else
        return (strcasecmp(this->c_str(), fn.c_str()) == 0);
}

string FileName::qualified() const
{
    if (at(0) != delim())
        return string("file:") + delim() + delim() + *this;
    else
        return string("file:") + delim() + *this;
}

string FileName::base(bool ext) const
{
    if (ext)
        return substr(rfind(delim()) + 1);
    else
        return substr(rfind(delim()) + 1, find('.') - (rfind(delim()) + 1));
}

string FileName::path() const { return substr(0, rfind(delim()) + 1); }

string FileName::relpath() const
{
    if (at(0) == delim())
        return substr(1, rfind(delim()));
    else
        return substr(0, rfind(delim()) + 1);
}

string FileName::ext(bool dot) const
{
    string::size_type st = find('.');
    if (st == string::npos)
        return string(0);
    if (dot)
        return substr(st, size() - st);
    else
        return substr(st + 1, size() - st);
}

void FileName::setBase(const string &basename, bool ext)
{
    string::size_type st = rfind(delim()) + 1;
    if (st == string::npos && ext)
    {
        append(basename);
        return;
    }
    replace(st, size() - st, basename);
}

void FileName::setExt(const string &extension)
{
    string::size_type st = find('.');
    if (st == string::npos)
    {
        append("." + extension);
        return;
    }
    if (extension.at(0) != '.')
        st++;
    replace(st, size() - st, extension);
}

void FileName::setPath(const string &path)
{
    if (find(delim()) == string::npos) // no delimiter found means no path.
    {
        if (path.at(path.size() - 1) != delim())
            insert(0, 1, delim());
        insert(0, path);
    }
    else
    {
        string::size_type s = 1;
        if (path.at(path.size() - 1) != delim())
            s = 0;
        replace(0, rfind(delim()) + s, path);
    }
}
