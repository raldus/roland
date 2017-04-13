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
#ifndef PREFS_H
#define PREFS_H

#include "config.h"
#include <string>
#include <array>
#include <utility>

#define PREFCOUNT 23

/** @author Fred Klaus */
class Prefs
{

  public:
    enum PrefsArray {paKey = 0, paValue = 1};

    Prefs(bool autowrite = true, bool writealways = true);
    ~Prefs();

    bool read();
    bool write();

    bool set(const std::string & key, const std::string & value);
    bool set(const std::string & key, int  value);
    bool set(const std::string & key, bool value);

    std::string getStr (const std::string & key) const;
    std::string getPath(const std::string & key) const;
    int         getNum (const std::string & key) const;
    bool        getBool(const std::string & key) const;

#ifdef _WIN32
    static const char delim() {return '\\';}
#else
    static char delim() {return '/';}
#endif

  private:

    bool mAutoWrite;
    bool mWriteAlways;
    bool mRead;

    std::string mFilename;
    std::string mNothing;
    std::array<std::pair<std::string, std::string>, PREFCOUNT> mPrefs;
    };

#endif // PREFS_H
