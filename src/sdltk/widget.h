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
#ifndef SDLTK_WIDGET_H
#define SDLTK_WIDGET_H

#include "canvas.h"
#include "SDL.h"


namespace sdltk
{

    class Color;
    class Gui;
    class Rect;
    class Size;
    class Point;

    //! Baseclass for all Widgets @author Fred Klaus development@fkweb.de
    class Widget
    {

    public:
        //! Constructor with parent Widget
        Widget(Canvas * const canvas);
        //! Constructor with parent Widget
        Widget(Widget * const parent = nullptr);
        //! Standarddestructor
        virtual ~Widget() = default;

        //! Draw thes widget
        virtual void draw() = 0;
        //! Catch MouseMotion
        virtual void onMouseMotion (SDL_MouseMotionEvent * event) {}
        //! Catch MouseButton
        virtual void onMouseButton (SDL_MouseButtonEvent * event) {}
        //! Catch Keyboard
        virtual bool onKeyboard    (SDL_KeyboardEvent * event) {return false;}
        //! Catch UserEvents
        virtual bool onUser        (SDL_UserEvent * event)     {return false;}

        //! Sets the Position
        virtual void setPos(int x, int y)
            {mRect.setX(x), mRect.setY(y);}
        //! Sets the Size
        virtual void setSize(int width, int height)
            {mRect.setWidth(width), mRect.setHeight(height);}

        //! Sets the Canvas to draw on
        virtual void setCanvas(Canvas * const canvas);

        //! Sets the parent Widget
        void  setParent (Widget * const parent) {mParent  = parent;}
        //! Returns the parent
        Widget * const parent() const {return mParent;}

        //! Sets the Color of the Widget by Color
        void setColor(const Color & color)  {mColor = color;}
        //! Sets the Color of the Widget by RGB(A)
        void setColor(int r, int g, int b, int a=255) {mColor.set(r,g,b,a);}
        //! Returns the Color of the Widget
        Color color() const {return mColor;}

        //! Set Widget enabled
        void setEnabled   (bool val) {mEnabled    = val;}
        //! Sets whether to catch Events
        void setWantEvents(bool val) {mWantEvents = val;}
        //! Sets whehther to catch MouseOver
        void setMouseOver (bool val) {mMouseOver  = val;}

        //! Returns whether the Widget is enabled
        bool enabled()      const {return mEnabled;}
        //! Returns whether the Widget wants Events
        bool wantEvents()   const {return mWantEvents;}
        //! Returns whether the Widget receives MouseOver
        bool mouseOver()    const {return mMouseOver;}
        //! Returns whether the Widget has MouseGrab
        bool hasMouseGrab() const {return mMouseGrab;}

        //! Returns the x position of the upper left corner
        Sint16 x()      const {return mRect.x();}
        //! Returns the y position of the upper left corner
        Sint16 y()      const {return mRect.y();}
        //! Returns the position of the upper left corner
        Point  pos()    const {return mRect.pos();}
        //! Returns the width of the Widget
        Uint16 width()  const {return mRect.width();}
        //! Returns the height of the Widget
        Uint16 height() const {return mRect.height();}
        //! Returns the Size of the Widget
        Size   size()   const {return mRect.size();}

        Point relativePos(Sint16 x, Sint16 y) const
                        {return Point(x - mRect.x(), y - mRect.y());}
        void  relativePos(Sint16 x, Sint16 y, Point & target) const
                        {return target.set(x - mRect.x(), y - mRect.y());}

        const Rect & rect()       const {return mRect;}
        const Rect & canvasrect() const {return mCanvasRect;}

    protected:
        Canvas * mCanvas;
        Widget * mParent;

        Rect  mRect;
        Rect  mCanvasRect;
        Color mColor;

        bool mMouseOver;
        bool mMouseGrab;
        bool mEnabled;
        bool mWantEvents;

    };

} // sdltk

#endif // SDLTK_VIDEOGL_H



