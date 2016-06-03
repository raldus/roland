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
#ifndef SDLTK_IMAGE_FONT_H
#define SDLTK_IMAGE_FONT_H

#include "image.h"
#include <string>

namespace sdltk
{

    class Rect;

    //! \brief a font created from an Image
    //! \author Fred Klaus development@fkweb.de
    class ImageFont
    {

    public:
        //! Standardconstructor
        ImageFont() : mSpacing(0) {};
        //! Constructor, calls ImageFont::load to load an Image
        //! \param fname = the filename of an Image @param glyphs = the available glyphs in the Image
        ImageFont(const String & fname, const String & glyphs);
        //! Standarddestructor
        ~ImageFont() = default;

        //! Check whether the Image is valid and sets up the glyphs and their Rect class
        void init();
        //! Frees all used resources
        void clear();

        //! loads an Image and calls ImageFont::init to set up the glyphs
        //! \param fname = the filename of an Image @param glyphs = the available glyphs in the Image
        void load(const String & fname, const String & glyphs);

        inline const Rect & glyph(Uint8 chr) const;

        const Image & image() {return mImage;}

        void setSpacing(Uint16 spacing);
        Uint16 spacing() const {return mSpacing;}

    private:
        Image mImage;
        String mGlyphs;
        Rect mGlyphRect[256]; // @todo  maximum Glyphs !?!
        Uint16 mSpacing;

    };


    inline const Rect & ImageFont::glyph(Uint8 chr) const
    {
        String::size_type i = mGlyphs.find(chr);
        if (i == String::npos) i = 0;
        return mGlyphRect[i];
    }

} // sdltk

#endif // SDLTK_IMAGE_FONT_H
