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
#include "imagefont.h"

namespace sdltk
{

    ImageFont::ImageFont()
    {
        mSpacing = 0;
        //mImage.setAutoConvert(false);
    }
    
    ImageFont::ImageFont(const string & fname, const string & glyphs)
    {
        load(fname, glyphs);
    }

    ImageFont::~ImageFont()
    {}
    
    void ImageFont::init() // @todo linearfont only !!!!
    {
        Point point(0, 0);
        Color sepcolor = mImage.pixel(point);
        
        Uint8 numglyph=0;
        for (Uint16 x=0; x < mImage.width(); x++)
        {
            point.setX(x);
            if (mImage.pixel(point) == sepcolor) continue;
            
            mGlyphRect[numglyph].setPos(point);
            while (mImage.pixel(point) != sepcolor) point.setX(++x);
            mGlyphRect[numglyph].setWidth(x-mGlyphRect[numglyph].x()-1);
            
            mGlyphRect[numglyph].setHeight(mImage.height());
            
            numglyph++;
        }
    }
    
    void ImageFont::clear()
    {
        mImage.clear();
    }

    void ImageFont::load(const string & fname, const string & glyphs)
    {
        clear();
        mImage.load(fname, false);
        mGlyphs = glyphs;
        init();
        mImage.convert(); // IMPORTANT: first init(), then convert()
    }

} //namespace sdltk






