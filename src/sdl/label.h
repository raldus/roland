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
#ifndef SDLTK_LABEL_H
#define SDLTK_LABEL_H

#include "widget.h"
#include <string>

namespace sdltk
{

    class Image;
    class Point;
    class Rect;

    //! \author Fred Klaus development@fkweb.de
    class Label : public Widget
    {

    public:
        Label(Widget * parent = nullptr);
        virtual ~Label();

        virtual void draw();

        virtual void setPos(int x, int y) {mRect.setX(x), mRect.setY(y); setBorder(mBorder);}

        virtual void onMouseMotion (SDL_MouseMotionEvent * event);
        virtual void onMouseButton (SDL_MouseButtonEvent * event);

        void setImage(const std::string & fname, bool autosize=true, bool bg=false);
        void setBorder(bool val);
        void setBackground(bool val)      {mBackground = val;}
        void setText(const std::string & text) {mText = text;}

    protected:
        void drawBorder();
        void drawBackground();
        void drawBackground(const Color & color);
        void drawText();

        void moveInit(SDL_MouseButtonEvent * event);
        void move(SDL_MouseMotionEvent * event);


        bool mBorder;
        bool mBackground;


        Image * mImage;

        std::string mText;
        Point mTextPos;
        Point mTextOffset;

        Point mRelativeMousePos;

        Point mLeftTop;
        Point mRightTop;
        Point mRightBottom;
        Point mLeftBottom;

        Color mBorderColor1;
        Color mBorderColor2;

    };

} // sdltk

#endif // SDLTK_LABEL_H
