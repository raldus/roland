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
#include "drawgl.h"

namespace sdltk
{

    DrawGL::DrawGL() : Draw()
    {}

    DrawGL::~DrawGL()
    {}
    
    void DrawGL::clearClipRect()
    {
        mClipRect.set(0, 0, mSurface->w, mSurface->h);
        oglScissor(0, 0, mSurface->w, mSurface->h);
    }

    void DrawGL::setClipRect(const Rect & rect)
    {
        mClipRect = rect;
        oglScissor(mClipRect.x(), mSurface->h - (mClipRect.y() + mClipRect.height()), mClipRect.width(), mClipRect.height());
    }

    void DrawGL::begin()
    {
        oglPushAttrib
        (
            GL_COLOR_BUFFER_BIT |
            GL_CURRENT_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_ENABLE_BIT |
            GL_FOG_BIT |
            GL_LIGHTING_BIT |
            GL_LINE_BIT |
            GL_POINT_BIT |
            GL_POLYGON_BIT |
            GL_SCISSOR_BIT |
            GL_STENCIL_BUFFER_BIT |
            GL_TEXTURE_BIT |
            GL_TRANSFORM_BIT |
            GL_POINT_BIT |
            GL_LINE_BIT
        );

        oglMatrixMode(GL_MODELVIEW);
        oglPushMatrix();
        oglLoadIdentity();

        oglMatrixMode(GL_TEXTURE);
        oglPushMatrix();
        oglLoadIdentity();

        oglMatrixMode(GL_PROJECTION);
        oglPushMatrix();
        oglLoadIdentity();

        oglOrtho(0.0f, (float) mSurface->w, (float) mSurface->h, 0.0f, -1.0f, 1.0f);

        oglDisable(GL_LIGHTING);
        oglDisable(GL_CULL_FACE);
        oglDisable(GL_DEPTH_TEST);
        oglDisable(GL_TEXTURE_2D);

        oglEnable(GL_SCISSOR_TEST);
        oglPointSize(1.0f);
        oglLineWidth(1.0f);

        oglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        oglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        
        if (mColor.hasAlpha()) oglEnable(GL_BLEND);
        else oglDisable(GL_BLEND);
    }

    void DrawGL::end()
    {       
        oglMatrixMode(GL_MODELVIEW);
        oglPopMatrix();

        oglMatrixMode(GL_TEXTURE);
        oglPopMatrix();

        oglMatrixMode(GL_PROJECTION);
        oglPopMatrix();

        oglPopAttrib();
    }


    void DrawGL::point(const Point & pos)
    {
        oglBegin(GL_POINTS);
        oglVertex2i(pos.x(), pos.y());
        oglEnd();
    }

    void DrawGL::rect(const Rect & rect)
    {
        oglBegin(GL_LINE_LOOP);
        oglVertex2f(rect.x() + 0.5f, rect.y() + 0.5f );
        oglVertex2f(rect.x() + rect.width() - 0.5f, rect.y() + 0.5f);
        oglVertex2f(rect.x() + rect.width() - 0.5f, rect.y() + rect.height() - 0.5f);
        oglVertex2f(rect.x() + 0.5f, rect.y() + rect.height() - 0.5f);
        oglEnd();
    }

    void DrawGL::fill(const Rect & rect)
    {
        oglBegin(GL_QUADS);
        oglVertex2i(rect.x(), rect.y() - 1);
        oglVertex2i(rect.x() + rect.width() + 1, rect.y() - 1);
        oglVertex2i(rect.x() + rect.width() + 1, rect.y() + rect.height());
        oglVertex2i(rect.x(), rect.y() + rect.height());
        oglEnd();
    }

    void DrawGL::line(const Point & pos1, const Point & pos2)
    {
        oglBegin(GL_LINES);
        oglVertex2i(pos1.x(), pos1.y());
        oglVertex2i(pos2.x() + 1, pos2.y()); ///@todo +1 is OK ????
        oglEnd();
    }

    void DrawGL::image(const Image & image, const Rect & src, const Rect & dest)
    {
        // switch only if necessary
        if (image.hasAlpha() && !mColor.hasAlpha()) oglEnable(GL_BLEND);
        if (!image.hasAlpha() && mColor.hasAlpha()) oglDisable(GL_BLEND);
        
        // Find OpenGL texture coordinates
        float texX1 = src.x() / (float) image.textureWidth();
        float texY1 = src.y() / (float) image.textureHeight();
        float texX2 = (src.x() + src.width())  / (float) image.textureWidth();
        float texY2 = (src.y() + src.height()) / (float) image.textureHeight();
        
        oglEnable(GL_TEXTURE_2D);
        oglBindTexture(GL_TEXTURE_2D, image.texNum());

        // Draw a textured quad -- the image
        oglBegin(GL_QUADS);
        oglTexCoord2f(texX1, texY1);
        oglVertex2i(dest.x(), dest.y());

        oglTexCoord2f(texX1, texY2);
        oglVertex2i(dest.x(), dest.y() + dest.height());

        oglTexCoord2f(texX2, texY2);
        oglVertex2i(dest.x() + dest.width(), dest.y() + dest.height());

        oglTexCoord2f(texX2, texY1);
        oglVertex2i(dest.x() + dest.width(), dest.y());
        oglEnd();
        
        // switch only if necessary
        if (image.hasAlpha() && !mColor.hasAlpha()) oglDisable(GL_BLEND);
        else if (mColor.hasAlpha()) oglEnable(GL_BLEND);

        oglDisable(GL_TEXTURE_2D);
    }

    void DrawGL::setColor(const Color & color)
    {
        mColor = color;
        oglColor4ub(mColor.r(), mColor.g(), mColor.b(), mColor.a());
        if (mColor.hasAlpha()) oglEnable(GL_BLEND);
    }
    
    /*
    void DrawGL::write(Point & pos, string & text)
    {
    }*/


} //namespace sdltk
