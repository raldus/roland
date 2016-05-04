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
#ifndef SDLGUIIMAGEFONT_H
#define SDLGUIIMAGEFONT_H

#include <string>
#include "rect.h"
#include "image.h"
#include "color.h"
#include "point.h"

namespace sdltk
{

    /** @brief a font created from an Image
        @author Fred Klaus development@fkweb.de */
    class ImageFont
    {
    public:
        /** Standardconstructor */
        ImageFont();
        /** Constructor, calls ImageFont::load to load an Image
            @param fname = the filename of an Image @param glyphs = the available glyphs in the Image */
        ImageFont(const string & fname, const string & glyphs);
        /** Standarddestructor */
        ~ImageFont();
        
        /** Check whether the Image is valid and sets up the glyphs and their Rect class */
        void init();
        /** Frees all used resources */
        void clear();
        
        /** loads an Image and calls ImageFont::init to set up the glyphs
            @param fname = the filename of an Image @param glyphs = the available glyphs in the Image */
        void load(const string & fname, const string & glyphs);
        
        inline const Rect & glyph(Uint8 chr) const;
        
        const Image & image() {return mImage;}
        
        void setSpacing(Uint16 spacing);
        Uint16 spacing() const {return mSpacing;}
        
    private:
        Image mImage;
        string mGlyphs;
        Rect mGlyphRect[256]; // @todo  maximum Glyphs !?!
        Uint16 mSpacing;

    };
    
    
    inline const Rect & ImageFont::glyph(Uint8 chr) const
    {
        string::size_type i = mGlyphs.find(chr);
        if (i == string::npos) i = 0;
        return mGlyphRect[i];
    }

} //namespace sdltk

#endif
