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
#ifndef SDLTK_IMAGE_H
#define SDLTK_IMAGE_H

#include "exception.h"
#include "pixel.h"
#include "rect.h"
#include "SDL.h"
#include "SDL_image.h"
#include "glfuncs.h"
#include <string>

namespace sdltk
{

    class Color;
    class Point;
    class Size;

    //! \brief Image to display either on an OpenGL- or plain SDL-Surface
    class Image
    {

    public:
        //! Constructor. Does some init.
        Image(bool autoconvert = true);
        Image(const String & fname, bool autoconvert = true);
        ~Image();

        //! Clears the Image and frees all resources
        void clear();
        //! loads an Image into a SDL_Surface @param fname the filename to lo:ad
        void load(const String & fname, bool autoconvert, uchar alpha=255);
        void load(const String & fname, uchar alpha=255);
        //! converts an Image to Screenformat
        void convert();
        //! has alphachannel? \return true if there is an alphachannel
        bool hasAlpha()    const {return mHasAlpha;}
        //! has colorkey? RGB(255, 0, 255) @return true if there is a colorkey
        bool hasColorKey() const {return mHasColorKey;}
        //! \return the width
        Uint16 width () const {return mSize.width();}
        //! \return the height
        Uint16 height() const {return mSize.height();}
        //! \return the Size
        const Size & size()   const {return mSize;}

        //! sets whether or not autoconvert to screenformat
        //! \param autoconvert = should be converted automatically?
        void setAutoConvert(bool autoconvert) {mAutoConvert=autoconvert;}

        void setAlpha(uchar alpha=255) {SDL_SetAlpha(mSurface, SDL_SRCALPHA, alpha);}
        void setColorKey(Color color) {mColorKey = color;}

        Color  pixel(const Point & pos) {return Pixel::get(mSurface, pos.x(), pos.y());}
        SDL_Surface * surface() {return mSurface;}
        void * pixels() {return mSurface->pixels;}

        GLuint texNum()     const {return mTexNum;}
        bool   hasTexture() const {return (mTexNum ? true : false);}

        GLuint textureWidth()  const {return mTextureWidth;}
        GLuint textureHeight() const {return mTextureHeight;}

    protected:
        //! checks the surface for Alpha
        void checkAlpha();
        //! checks the surface for ColorKey
        void checkColorKey();

        void genTexture();
        void calcTexSize();
        void clearTexture();
        void clearSurface();

    private:
        SDL_Surface * mSurface;

        Color mColorKey;

        bool mHasAlpha;
        bool mHasColorKey;
        bool mIsConverted;

        bool mAutoConvert;

        Size mSize;

        GLuint mTextureWidth;
        GLuint mTextureHeight;
        GLuint mTexNum;
    };

} // sdltk

#endif // SDLTK_IMAGE_H
