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
#ifndef KEYTRANS_H
#define KEYTRANS_H

#include "types.h"
#include "SDL.h"

namespace sdltk
{

    //! This class provides Keyboard translaation between CPC/EN/DE/SDL/WIN/LINUX
    class KeyTrans
    {

    public:
        KeyTrans();
        ~KeyTrans() {}

        struct JoyAlloc
        {
            tUBYTE joy;
            tUBYTE orig;
            tUWORD key;
        };
        struct SeqPair
        {
            tUBYTE keyval;
            bool  down;
        };

        enum Language {German, English};

        void init(Language lang=German);

        tUBYTE get(SDL_Event & event);

        bool toggleJoystick();
        bool joystickEnabled() {return mJoyEnabled;}

        const SeqPair & sequenceVal();
        bool hasSequence();

        void sequenceCatRun();

    private:
        SeqPair mSequence[64];
        uint mSeqIndex;

        static tUBYTE mTable[320];

        JoyAlloc mJoyAlloc[6];
        bool     mJoyEnabled;
    };

} // sdltk

#endif // KEYTRANS_H
