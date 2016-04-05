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

    const std::string & getStr (const std::string & key) const;
    std::string         getPath(const std::string & key) const;
    int                 getNum (const std::string & key) const;
    bool                getBool(const std::string & key) const;

  private:
#ifdef _WIN32
    static const char delim() {return '\\';}
#else
    static const char delim() {return '/';}
#endif

    bool mAutoWrite;
    bool mWriteAlways;
    bool mRead;

    std::string mFilename;
    std::string mNothing;
    std::string mPrefs[PREFCOUNT][2] =
    {
        {"cpctype", "2"},        {"cpcspeed", "4"},    {"cpcrom", "~/"},
        {"amsdos", "~/"},        {"romdir", "~/"},     {"diskdir", "~/"},
        {"tapedir", "~/"},       {"snapdir", "~/"},    {"diska", ""},
        {"diskb", ""},           {"ramsize", "128"},   {"showfps", "true"},
        {"fullscreen", "false"}, {"fullwidth", "640"}, {"fullheight", "480"},
        {"winwidth", "640"},     {"winheight", "480"}, {"monitor", "0"},
        {"border", "true"},      {"intensity", "10"},  {"doublescan", "true"},
        {"jumpers", "58"}};
    };

#endif // PREFS_H
