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
#include "prefs.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <cstdlib>

#include <limits.h>

#ifdef _WIN32
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

using std::string;
using std::ofstream;
using std::ifstream;

string Prefs::text[][2] = {
    {"cpctype", "2"},        {"cpcspeed", "4"},    {"cpcrom", "~/"},
    {"amsdos", "~/"},        {"romdir", "~/"},     {"diskdir", "~/"},
    {"tapedir", "~/"},       {"snapdir", "~/"},    {"diska", ""},
    {"diskb", ""},           {"ramsize", "128"},   {"showfps", "true"},
    {"fullscreen", "false"}, {"fullwidth", "640"}, {"fullheight", "480"},
    {"winwidth", "640"},     {"winheight", "480"}, {"monitor", "0"},
    {"border", "true"},      {"intensity", "10"},  {"doublescan", "true"},
    {"jumpers", "58"}};

Prefs::Prefs(bool autowrite, bool writealways)
{
    mAutoWrite = autowrite;
    mWriteAlways = writealways;

#ifdef _WIN32
    char buf[NAME_MAX + 1];
    mFilename = getcwd(buf, NAME_MAX);
    mFilename += delim();
    mFilename += PACKAGE_NAME;
    mFilename += ".cfg";
#else
    mFilename = getenv("HOME");
    mFilename += delim();
    mFilename += ".";
    mFilename += PACKAGE_NAME;
#endif

    mNothing = "";

    read();
}

Prefs::~Prefs()
{
    if (mAutoWrite)
        write();
}

bool Prefs::read()
{
    DOUT("\nReading Preferences...\n");

    ifstream in(mFilename.c_str());
    if (!in)
        return false;

    string tmp;
    while (!in.eof())
    {
        std::getline(in, tmp);
        if (in.eof())
            break;
        for (int i = 0; i < PREFCOUNT; i++)
        {
            // mStr[i].clear();
            if (tmp.substr(0, tmp.find('=')) == text[i][paKey])
            {
                mStr[i] = tmp.substr(tmp.rfind('=') + 1);
                break;
            }
        }
    }
    in.close();

    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (mStr[i].empty())
            mStr[i] = text[i][paDefault];
        DOUT(text[i][paKey] << ": " << mStr[i] << "\n");
    }

    return true;
}

bool Prefs::write()
{
    DOUT("\nWriting Preferences...\n");

    ofstream out(mFilename.c_str());
    if (!out)
        return false;
    for (int i = 0; i < PREFCOUNT; i++)
    {
        out << text[i][paKey] << "=" << mStr[i] << "\n";
        DOUT(text[i][paKey] << ": " << mStr[i] << "\n");
    }
    out.close();
    return true;
}

bool Prefs::set(const string &key, const string &value)
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == text[i][paKey])
        {
            mStr[i] = value;
            if (mWriteAlways)
                write();
            return true;
        }
    }
    return false;
}

bool Prefs::set(const string &key, int value)
{
    int d, n;
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == text[i][paKey])
        {
            mStr[i] = fcvt(value, 0, &d, &n);
            if (mWriteAlways)
                write();
            return true;
        }
    }
    return false;
}

bool Prefs::set(const string &key, bool value)
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == text[i][paKey])
        {
            if (value == getBool(key))
                return true; // @todo change this for all members to "is
                             // changed"
            mStr[i] = value ? "yes" : "no";
            if (mWriteAlways)
                write();
            return true;
        }
    }
    return false;
}

const string &Prefs::getStr(const string &key) const
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == text[i][paKey])
        {
            return mStr[i];
        }
    }
    return mNothing;
}

string Prefs::getPath(const string &key) const
{
    string text = getStr(key);
    if (text.empty())
        return mNothing;

    string tmp = text;

    if ((text.find_first_of('.') == 0) && (text.find_first_of(delim()) == 1))
    {
        string tmp;
        char buf[NAME_MAX + 1];
        tmp = getcwd(buf, NAME_MAX);
        tmp += delim();
        tmp += text.substr(2);
        return tmp;
    }
    else
        return tmp;
}

int Prefs::getNum(const string &key) const
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == text[i][paKey])
        {
            if (mStr[i].empty())
                return 0;
            return atoi(mStr[i].c_str());
        }
    }
    return 0;
}

bool Prefs::getBool(const string &key) const
{
    for (int i = 0; i < PREFCOUNT; i++)
    {
        if (key == text[i][paKey])
        {
            if (mStr[i].empty())
                return false;
            if ((mStr[i] == "yes") || (mStr[i] == "true") ||
                (mStr[i] == "on") || (mStr[i] == "1"))
                return true;
            else
                return false;
        }
    }
    return 0;
}
