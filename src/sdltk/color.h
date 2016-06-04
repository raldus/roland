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
#ifndef SDLTK_COLOR_H
#define SDLTK_COLOR_H

#include "SDL.h"

namespace sdltk
{

    //! \author Fred Klaus development@fkweb.de @bug operators does not work correctly @todo operators*/
    class Color
    {

    public:
        Color();
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255) {mR = r; mG = g; mB = b; mA = a;}
        ~Color() = default;

        Color(const Color & c) {mR = c.r(); mG = c.g(); mB = c.b(); mA = c.a();}

        bool operator== (const Color& color) const
            {return   mR == color.r() && mG == color.g() && mB == color.b() && mA == color.a();}
        bool operator!= (const Color& color) const
            {return !(mR == color.r() && mG == color.g() && mB == color.b() && mA == color.a());}

        Color operator+ (const Color& color) const;
        Color operator- (const Color& color) const;
        Color operator* (float value) const;

        void set(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
        void setBlack() {mR = 0;   mG = 0;   mB = 0;   mA = 255;}
        void setWhite() {mR = 255; mG = 255; mB = 255; mA = 255;}
        void setAlpha(Uint8 a) {mA = a;}
        void clearAlpha()      {mA = 255;}
        bool hasAlpha()        {return (mA != 255);}

        Uint8 r() const {return mR;}
        Uint8 g() const {return mG;}
        Uint8 b() const {return mB;}
        Uint8 a() const {return mA;}

        void setR(Uint8 r) {mR = r;}
        void setG(Uint8 g) {mG = g;}
        void setB(Uint8 b) {mB = b;}
        void setA(Uint8 a) {mA = a;}

    protected:
        Uint8 mR;
        Uint8 mG;
        Uint8 mB;
        Uint8 mA;
    };

} // sdltk

#endif // SDLTK_COLOR_H
