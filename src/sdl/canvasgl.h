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
#ifndef SDLGUIDRAWGL_H
#define SDLGUIDRAWGL_H

#include <canvas.h>
#include "glfuncs.h"

namespace sdltk
{

    /** @author Fred Klaus development@fkweb.de */
    class CanvasGL : public Canvas
    {
    public:
        CanvasGL();
        virtual ~CanvasGL();

        virtual void begin();
        virtual void end();

        virtual void point(const Point & pos);
        virtual void rect (const Rect  & rect);
        virtual void fill (const Rect  & rect);
        virtual void line (const Point & pos1, const Point & pos2);
        virtual void image(const Image & image, const Rect &  src,  const Rect  & dest);

        virtual void setColor(const Color & color);
        
        virtual void setClipRect(const Rect & rect);
        virtual void clearClipRect();

    };

} //namespace sdltk

#endif //SDLGUIDRAWGL_H
