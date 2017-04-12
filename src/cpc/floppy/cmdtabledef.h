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
#ifndef CPC_CMDTABLEDEF_H
#define CPC_CMDTABLEDEF_H

#include "compspec.h"

namespace cpcx
{
    class Fdc;

    //! Comand table definitions
    class CmdTableDef final
    {
    public:
        CmdTableDef()  ROLAND_DEFAULT
        ~CmdTableDef() ROLAND_DEFAULT

        typedef void (Fdc::*CmdHandler)(void);

        void set(int cmd, int cmdlen, int reslen, int cmddir, CmdHandler cmdhandler)
            {mCmd=cmd; mCmdLength=cmdlen; mResLength=reslen; mCmdDirection=cmddir; mCmdHandler=cmdhandler;}

        int cmd()          const {return mCmd;}
        int cmdLength()    const {return mCmdLength;}
        int resLength()    const {return mResLength;}
        int cmdDirection() const {return mCmdDirection;}
        CmdHandler cmdHandler() const {return mCmdHandler;}

    private:
        int mCmd;
        int mCmdLength;
        int mResLength;
        int mCmdDirection;
        CmdHandler mCmdHandler;
    };
} // namespace cpcx

#endif //CPC_CMDTABLEDEF_H
