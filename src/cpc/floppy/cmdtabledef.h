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
#ifndef CMDTABLEDEF_H
#define CMDTABLEDEF_H

class Fdc;

/** @author Fred Klaus */
class CmdTableDef
{
public:
	CmdTableDef()  {}
	~CmdTableDef() {}

	typedef void (Fdc::*CmdHandler)(void);

	void set(int cmd, int cmdlen, int reslen, int cmddir, CmdHandler cmdhandler)
		{mCmd=cmd; mCmdLength=cmdlen; mResLength=reslen; mCmdDirection=cmddir; mCmdHandler=cmdhandler;}

	int cmd()          {return mCmd;}
	int cmdLength()    {return mCmdLength;}
	int resLength()    {return mResLength;}
	int cmdDirection() {return mCmdDirection;}
	CmdHandler cmdHandler() {return mCmdHandler;}

private:
	int mCmd;
	int mCmdLength;
	int mResLength;
	int mCmdDirection;
	CmdHandler mCmdHandler;

};

#endif
