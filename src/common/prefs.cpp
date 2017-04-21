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
#include "prefs.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef NAME_MAX
#define NAME_MAX 1024
#endif

#ifdef DEBUG
#define DOUT(a) std::cerr << a;
#else
#define DOUT(a)
#endif

Prefs::Prefs(bool autowrite, bool writealways)
    : mAutoWrite(autowrite), mWriteAlways(writealways), mRead(false)
{

#if defined(_WIN32) || defined(_WIN64)
    char buf[NAME_MAX + 1];
    mFilename  = _getcwd(buf, NAME_MAX);
    mFilename += delim();
    mFilename += PACKAGE_NAME;
    mFilename += ".cfg";
#else
    mFilename  = getenv("HOME");
    mFilename += delim();
    mFilename += ".";
    mFilename += PACKAGE_NAME;
#endif

    mNothing = "";

    // keys and default values
    mPrefs =
    {
        {
            {"cpctype",    "2"},
            {"cpcspeed",   "4"},
            {"cpcrom",     CPCROM},
            {"amsdos",     AMSROM},
            {"romdir",     ROMDIR},
            {"diskdir",    DISKDIR},
            {"tapedir",    "~/.roland"},
            {"snapdir",    "~/.roland"},
            {"datadir",    DATADIR},
            {"diska",      ""},
            {"diskb",      ""},
            {"ramsize",    "128"},
            {"showfps",    "true"},
            {"fullscreen", "false"},
            {"fullwidth",  "640"},
            {"fullheight", "480"},
            {"winwidth",   "640"},
            {"winheight",  "480"},
            {"monitor",    "0"},
            {"border",     "true"},
            {"intensity",  "10"},
            {"doublescan", "true"},
            {"jumpers",    "58"}
        }
    };

    read();
}

Prefs::~Prefs()
{
    if (mAutoWrite) write();
}

bool Prefs::read()
{
    DOUT("\nReading Preferences...\n");

    std::ifstream in(mFilename.c_str());
    if (!in)
    {
        write();
        in.open(mFilename.c_str());
        if (!in) return false;
    }

    std::string tmp;
    while (!in.eof())
    {
        std::getline(in, tmp);
        if (in.eof()) break;
        for (int i = 0; i < PREFCOUNT; i++)
        {
            if (tmp.substr(0, tmp.find('=')) == mPrefs[i].first)
            {
                std::string tmpval(tmp.substr(tmp.rfind('=') + 1));
                if (!tmpval.empty()) mPrefs[i].second = tmpval;
                break;
            }
        }
    }
    in.close();

    for (int i = 0; i < PREFCOUNT; i++)
    {
        DOUT(mPrefs[i].first<< ": " << mPrefs[i].second<< "\n");
    }

    return true;
}

bool Prefs::write()
{
    DOUT("\nWriting Preferences...\n");

    std::ofstream out(mFilename.c_str());
    if (!out) return false;
    for (int i = 0; i < PREFCOUNT; i++)
    {
        out << mPrefs[i].first<< "=" << mPrefs[i].second<< "\n";
        DOUT(mPrefs[i].first<< ": " << mPrefs[i].second<< "\n");
    }
    out.close();
    return true;
}

bool Prefs::set(const std::string & key, const std::string &value)
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == mPrefs[i].first)
        {
            mPrefs[i].second= value;
            if (mWriteAlways) write();
            return true;
        }
    }
    return false;
}

bool Prefs::set(const std::string & key, int value)
{
    int d, n;
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == mPrefs[i].first)
        {
            mPrefs[i].second= fcvt(value, 0, &d, &n);
            if (mWriteAlways) write();
            return true;
        }
    }
    return false;
}

bool Prefs::set(const std::string & key, bool value)
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == mPrefs[i].first)
        {
            if (value == getBool(key)) return true; // @todo change this for all members to "is
                             // changed"
            mPrefs[i].second= value ? "yes" : "no";
            if (mWriteAlways) write();
            return true;
        }
    }
    return false;
}

std::string Prefs::getStr(const std::string & key) const
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == mPrefs[i].first) return mPrefs[i].second;
    }
    return mNothing;
}

std::string Prefs::getPath(const std::string & key) const
{
    std::string text = getStr(key);
    if (text.empty()) return mNothing;

    std::string tmp = text;

    if ((text.find_first_of('.') == 0) && (text.find_first_of(delim()) == 1))
    {
        std::string tmp;
#if defined(_WIN32) || defined(_WIN64)
    	char buf[NAME_MAX + 1];
    	tmp = _getcwd(buf, NAME_MAX);
#else
    	tmp = getenv("HOME");
#endif
        tmp += delim();
        tmp += text.substr(2);
        return tmp;
    }
    else
        return tmp;
}

int Prefs::getNum(const std::string & key) const
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == mPrefs[i].first)
        {
            if (mPrefs[i].second.empty()) return 0;
            return atoi(mPrefs[i].second.c_str());
        }
    }
    return 0;
}

bool Prefs::getBool(const std::string & key) const
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == mPrefs[i].first)
        {
            if (mPrefs[i].second.empty()) return false;
            if ((mPrefs[i].second== "yes") || (mPrefs[i].second== "true") ||
                (mPrefs[i].second== "on")  || (mPrefs[i].second== "1"))
                return true;
            else
                return false;
        }
    }
    return 0;
}
