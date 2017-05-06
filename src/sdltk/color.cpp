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
#include "color.h"

namespace sdltk
{

    Color::Color()
    {
        set(0, 0, 0, 255);
    }

    void Color::set(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        mR = r;
        mG = g;
        mB = b;
        mA = a;
    }

    Color Color::operator+(const Color& color) const
    {
        Color result;

        result.setR(mR + color.r());
        result.setG(mG + color.g());
        result.setB(mB + color.b());
        result.setA(255);

        result.setR(result.r());
        result.setG(result.g());
        result.setB(result.b());

        return result;
    }

    Color Color::operator-(const Color& color) const
    {
        Color result;

        result.setR(mR - color.r());
        result.setG(mG - color.g());
        result.setB(mB - color.b());
        result.setA(255);

        result.setR(result.r());
        result.setG(result.g());
        result.setB(result.b());

        return result;
    }

    Color Color::operator*(float value) const
    {
        Color result;
        result.setR((int)(mR * value));
        result.setG((int)(mG * value));
        result.setB((int)(mB * value));
        result.setA(mA);

        result.setR(result.r());
        result.setG(result.g());
        result.setB(result.b());

        return result;
    }

} // sdltk
