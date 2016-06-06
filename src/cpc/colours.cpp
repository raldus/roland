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
#include "colours.h"

namespace cpcx
{

    constexpr float  Colours::mColour[32][3];
    constexpr double Colours::mGreen[32];

    void Colours::init()
    {
        if (mMonitor == 0)
        {
            switch (mDepth)
            {
                case 32:
                case 24:
                    for (int i = 0; i < 32; i++)
                    {
                        tUDWORD red =
                            (tUDWORD)(mColour[i][0] * (mIntensity / 10.0) * 255);
                        if (red > 255)
                        { // limit to the maximum
                            red = 255;
                        }
                        tUDWORD green =
                            (tUDWORD)(mColour[i][1] * (mIntensity / 10.0) * 255);
                        if (green > 255)
                        {
                            green = 255;
                        }
                        tUDWORD blue =
                            (tUDWORD)(mColour[i][2] * (mIntensity / 10.0) * 255);
                        if (blue > 255)
                        {
                            blue = 255;
                        }
                        mTable[i] = blue | (green << 8) | (red << 16);
                    }
                    break;

                case 16:
                    for (int i = 0; i < 32; i++)
                    {
                        tUDWORD red =
                            (tUDWORD)(mColour[i][0] * (mIntensity / 10.0) * 31);
                        if (red > 31)
                            red = 31; // limit to the maximum

                        tUDWORD green =
                            (tUDWORD)(mColour[i][1] * (mIntensity / 10.0) * 63);
                        if (green > 63)
                            green = 63;

                        tUDWORD blue =
                            (tUDWORD)(mColour[i][2] * (mIntensity / 10.0) * 31);
                        if (blue > 31)
                            blue = 31;

                        tUDWORD colour = blue | (green << 5) | (red << 11);

                        mTable[i] = colour | (colour << 16);
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            switch (mDepth)
            {
                case 32:
                case 24:
                    for (int i = 0; i < 32; i++)
                    {
                        tUDWORD red = (tUDWORD)(0 * (mIntensity / 10.0) * 255);
                        if (red > 255)
                        { // limit to the maximum
                            red = 255;
                        }
                        tUDWORD green =
                            (tUDWORD)(mGreen[i] * (mIntensity / 10.0) * 255);
                        if (green > 255)
                        {
                            green = 255;
                        }
                        tUDWORD blue = (tUDWORD)(0 * (mIntensity / 10.0) * 255);
                        if (blue > 255)
                        {
                            blue = 255;
                        }
                        mTable[i] = blue | (green << 8) | (red << 16);
                    }
                    break;

                case 16:
                    for (int i = 0; i < 32; i++)
                    {
                        tUDWORD red = (tUDWORD)(0 * (mIntensity / 10.0) * 31);
                        if (red > 31)
                            red = 31; // limit to the maximum

                        tUDWORD green =
                            (tUDWORD)(mGreen[i] * (mIntensity / 10.0) * 63);
                        if (green > 63)
                            green = 63;

                        tUDWORD blue = (tUDWORD)(0 * (mIntensity / 10.0) * 31);
                        if (blue > 31)
                            blue = 31;

                        tUDWORD colour = blue | (green << 5) | (red << 11);

                        mTable[i] = colour | (colour << 16);
                    }
                    break;

                default:
                    break;
            }
        }
    }

} // cpc
