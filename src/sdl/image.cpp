/***************************************************************************
 *   Copyright (C) 2005-2013 by Fred Klaus <development@fkweb.de>          *
 *                                                                         *
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
#include "image.h"

namespace sdltk
{

    Image::Image(bool autoconvert)
    {
        mAutoConvert = autoconvert;

        mSurface = 0;
        mTexNum  = 0;

        clear();
    }

    Image::Image(const string & fname, bool autoconvert)
    {
        mAutoConvert = autoconvert;

        mSurface = 0;
        mTexNum  = 0;

        clear();

        load(fname);
    }

    Image::~Image()
    {
        clear();
    }

    void Image::clear()
    {
        mColorKey.set(255, 0, 255, 255);

        mHasAlpha    = false;
        mHasColorKey = false;
        mIsConverted = false;

        clearSurface();
        clearTexture();
    }

    void Image::clearSurface()
    {
        if (mSurface)
        {
            SDL_FreeSurface(mSurface);
            mSurface = 0;
            //mSize.set(0, 0);
        }
    }

    void Image::clearTexture()
    {
        if (hasTexture())
        {
            if (oglIsTexture(mTexNum))
            {
                oglDeleteTextures(1, &mTexNum);
                if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not delete texture");
                mTexNum = 0;
                mTextureWidth  = 0;
                mTextureHeight = 0;
            }
        }
        mSize.set(0, 0); // @todo watch this !!!!!!!!!!!!!!1
    }

    void Image::calcTexSize()
    {
        mTextureWidth  = 1;
        mTextureHeight = 1;
        while(mTextureWidth  < mSize.width() ) mTextureWidth  *= 2;
        while(mTextureHeight < mSize.height()) mTextureHeight *= 2;

        GLint max_texsize=0;
        oglGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texsize);
        if (((GLuint) max_texsize < mTextureWidth) || ((GLuint) max_texsize < mTextureHeight))
        {
            throw EXCGL("image exceeds maximum texturesize");
        }
    }

    void Image::checkAlpha()
    {
        mHasAlpha = false;
        Color color;
        for (Uint16 x=0; x < mSize.width(); ++x)
        {
            for (Uint16 y=0; y < mSize.height(); ++y)
            {

                color = Pixel::get(mSurface, x, y);

                if (color.a() != 255)
                {
                    mHasAlpha = true;
                    break;
                }
            }
        }
    }

    void Image::checkColorKey()
    {
        mHasColorKey = false;
        Color color;
        for (Uint16 x=0; x < mSize.width(); ++x)
        {
            for (Uint16 y=0; y < mSize.height(); ++y)
            {

                color = Pixel::get(mSurface, x, y);

                if (color == mColorKey)
                {
                    SDL_SetColorKey(mSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mSurface->format, mColorKey.r(), mColorKey.g(), mColorKey.b()));
                    mHasColorKey = true;
                    break;
                }
            }
        }
    }

    void Image::load(const string & fname, bool autoconvert)
    {
        mAutoConvert = autoconvert;
        mSurface = IMG_Load(fname.c_str());
        if (!mSurface) throw EXCSDL("could not load image");
        mSize.set(mSurface);
        if (mAutoConvert) convert();
    }

    void Image::load(const string & fname)
    {
        load(fname, mAutoConvert);
    }

    void Image::convert()
    {
        if (mIsConverted) return;

        checkAlpha();
        checkColorKey();

        SDL_Surface * surface = SDL_GetVideoSurface();
        if (!surface) throw EXCSDL("could not get videosurface");
        if (surface->flags & SDL_OPENGL)
        {
            calcTexSize();
            if (mHasAlpha || mHasColorKey)
            {
                SDL_Surface * tmp = SDL_DisplayFormatAlpha(mSurface);
                if (!tmp) throw EXCSDL("could not convert to screenformat");
                clearSurface();
                mSurface = tmp;
            }
            else
            {
                SDL_Surface * ref = SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, 24, 0,0,0,0);
                if (!ref) throw EXCSDL("could not convert to screenformat");
                SDL_Surface * tmp = SDL_ConvertSurface(mSurface, ref->format, SDL_SWSURFACE);
                if (!tmp) throw EXCSDL("could not convert to screenformat");
                clearSurface();
                mSurface = tmp;
            }
            mSize.set(mSurface);
            calcTexSize();
            genTexture();
        }
        else
        {
            SDL_Surface * tmp;
            if (mHasAlpha) tmp = SDL_DisplayFormatAlpha(mSurface);
            else           tmp = SDL_DisplayFormat(mSurface);
            if (!tmp) throw EXCSDL("could not convert to screenformat");
            clearSurface();
            mSurface = tmp;
            mSize.set(mSurface);
        }
        mIsConverted = true;
    }

    void Image::genTexture()
    {
        if (hasTexture()) return;

        if (mHasColorKey) // && !mHasAlpha
        {
            Color color;
            for (Uint16 x=0; x < mSize.width(); ++x)
            {
                for (Uint16 y=0; y < mSize.height(); ++y)
                {

                    color = Pixel::get(mSurface, x, y);
                    if ((color.r() == 255) && (color.b() == 255))
                    {
                        color.setA(0);
                        Pixel::put(mSurface, x, y, color);
                    }
                }
            }
            mHasAlpha    = true;
            mHasColorKey = false;
        }

        oglGenTextures(1, &mTexNum);
        if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not create texture-id");
        oglBindTexture(GL_TEXTURE_2D, mTexNum);
        if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not bind texture");
        oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR:  GL_NEAREST
        oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not set texparameter");

        if (mHasAlpha)
        {
            oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
            if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not create texture");
            oglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mSize.width(), mSize.height(), GL_BGRA, GL_UNSIGNED_BYTE, mSurface->pixels);
            if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not create subtexture");
        }
        else
        {
            oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mTextureWidth, mTextureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);
            if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not create texture");
            oglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mSize.width(), mSize.height(), GL_BGR, GL_UNSIGNED_BYTE, mSurface->pixels);
            if (oglGetError() != GL_NO_ERROR) throw EXCSDL("could not create subtexture");
        }

        clearSurface(); // @todo auto ???
    }

} //namespace sdltk
