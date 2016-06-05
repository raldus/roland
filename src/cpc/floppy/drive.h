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
#ifndef DISKDRIVE_H
#define DISKDRIVE_H

#include "track.h"
#include "fdcconst.h"

/** @author Fred Klaus */
class Drive final
{
public:
    Drive();
    ~Drive() = default;

    void init();

    uint currentSector()     {return mCurrentSector;}
    uint tracks()            {return mTracks;}
    uint writeProtected()    {return mWriteProtected;}
    uint currentTrack()      {return mCurrentTrack;}
    uint currentSide()       {return mCurrentSide;}
    uint sides()             {return mSides;}
    uint flipped()           {return mFlipped;}
    uint randomDEs()         {return mRandomDEs;}
    Track & track(int trck, int sde) {return mTrack[trck][sde];}

    void setAltered(uint num)       {mAltered=num;}
    void setCurrentSector(uint num) {mCurrentSector=num;}
    void setCurrentTrack(uint num)  {mCurrentTrack=num;}
    void setCurrentSide(uint num)   {mCurrentSide=num;}
    void setSides(uint num)         {mSides=num;}
    void setRandomDEs(uint num)     {mRandomDEs=num;}
    void setTracks(uint num)        {mTracks=num;}

private:
    uint mTracks;                    // total number of tracks
    uint mCurrentTrack;              // location of drive head
    uint mSides;                     // total number of sides
    uint mCurrentSide;               // side being accessed
    uint mCurrentSector;             // sector being accessed
    uint mAltered;                   // has the image been modified?
    uint mWriteProtected;            // is the image write protected?
    uint mRandomDEs;                 // sectors with Data Errors return random data?
    uint mFlipped;                   // reverse the side to access?
    Track mTrack[DSK_TRACKMAX][DSK_SIDEMAX]; // array of track information structures

};

#endif
