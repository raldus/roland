/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus   *
 *   frednet@web.de   *
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
#ifndef PREFS_H
#define PREFS_H

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <string>

#define PREFCOUNT 22

using std::string;

/** @author Fred Klaus */
class Prefs
{

public:
    
    enum PrefsArray {paKey=0, paDefault=1};
    
    Prefs(bool autowrite=true, bool writealways=true);
    ~Prefs();

    bool read ();
    bool write();

    bool set(const string & key, const string & value);
    bool set(const string & key, int value);
    bool set(const string & key, bool value);

    const string & getStr (const string & key) const;
    string         getPath(const string & key) const;
    int            getNum (const string & key) const;
    bool           getBool(const string & key) const;

private:
    #ifdef _WIN32
        static const char delim() {return '\\';}
    #else
        static const char delim() {return '/';}
    #endif

    string mFilename;
    string mNothing;
    string mStr[PREFCOUNT];
    static string text[PREFCOUNT][2];

    bool mAutoWrite;
    bool mWriteAlways;
    bool mRead;
};

#endif //PREFS_H
