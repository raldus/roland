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

double Colours::mColour[32][3] = {
    {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5},
    {0.0, 0.0, 0.5}, {1.0, 0.0, 0.5}, {0.0, 0.5, 0.5}, {1.0, 0.5, 0.5},
    {1.0, 0.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0},
    {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 0.5, 0.0}, {1.0, 0.5, 1.0},
    {0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
    {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 1.0},
    {0.5, 0.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 1.0, 0.0}, {0.5, 1.0, 1.0},
    {0.5, 0.0, 0.0}, {0.5, 0.0, 1.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 1.0}};

double Colours::mGreen[32] = {
    0.5647, 0.5647, 0.7529, 0.9412, 0.1882, 0.3765, 0.4706, 0.6588,
    0.3765, 0.9412, 0.9098, 0.9725, 0.3451, 0.4078, 0.6275, 0.6902,
    0.1882, 0.7529, 0.7216, 0.7843, 0.1569, 0.2196, 0.4392, 0.5020,
    0.2824, 0.8471, 0.8157, 0.8784, 0.2510, 0.3137, 0.5333, 0.5961};

Colours::Colours()
{
    mDepth = 16;
    mIntensity = 10;
    mMonitor = 0;
}

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
                    UDWORD red =
                        (UDWORD)(mColour[i][0] * (mIntensity / 10.0) * 255);
                    if (red > 255)
                    { // limit to the maximum
                        red = 255;
                    }
                    UDWORD green =
                        (UDWORD)(mColour[i][1] * (mIntensity / 10.0) * 255);
                    if (green > 255)
                    {
                        green = 255;
                    }
                    UDWORD blue =
                        (UDWORD)(mColour[i][2] * (mIntensity / 10.0) * 255);
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
                    UDWORD red =
                        (UDWORD)(mColour[i][0] * (mIntensity / 10.0) * 31);
                    if (red > 31)
                        red = 31; // limit to the maximum

                    UDWORD green =
                        (UDWORD)(mColour[i][1] * (mIntensity / 10.0) * 63);
                    if (green > 63)
                        green = 63;

                    UDWORD blue =
                        (UDWORD)(mColour[i][2] * (mIntensity / 10.0) * 31);
                    if (blue > 31)
                        blue = 31;

                    UDWORD colour = blue | (green << 5) | (red << 11);

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
                    UDWORD red = (UDWORD)(0 * (mIntensity / 10.0) * 255);
                    if (red > 255)
                    { // limit to the maximum
                        red = 255;
                    }
                    UDWORD green =
                        (UDWORD)(mGreen[i] * (mIntensity / 10.0) * 255);
                    if (green > 255)
                    {
                        green = 255;
                    }
                    UDWORD blue = (UDWORD)(0 * (mIntensity / 10.0) * 255);
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
                    UDWORD red = (UDWORD)(0 * (mIntensity / 10.0) * 31);
                    if (red > 31)
                        red = 31; // limit to the maximum

                    UDWORD green =
                        (UDWORD)(mGreen[i] * (mIntensity / 10.0) * 63);
                    if (green > 63)
                        green = 63;

                    UDWORD blue = (UDWORD)(0 * (mIntensity / 10.0) * 31);
                    if (blue > 31)
                        blue = 31;

                    UDWORD colour = blue | (green << 5) | (red << 11);

                    mTable[i] = colour | (colour << 16);
                }
                break;

            default:
                break;
        }
    }
}
