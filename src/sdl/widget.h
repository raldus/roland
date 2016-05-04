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
#ifndef WIDGET_H
#define WIDGET_H

#include "draw.h"
#include "color.h"
#include "rect.h"
#include "SDL.h"


namespace sdltk
{
    
    class Gui;
    class Size;
    class Point;

    /** Baseclass for all Widgets @author Fred Klaus development@fkweb.de */
    class Widget
    {
    public:
        Widget();
        virtual ~Widget();
        
        virtual void draw() = 0;
        virtual void onMouseMotion (SDL_MouseMotionEvent * event) {}
        virtual void onMouseButton (SDL_MouseButtonEvent * event) {}
        virtual bool onKeyboard    (SDL_KeyboardEvent * event)    {return false;}
        
        virtual void setPos(int x, int y)            {mRect.setX(x), mRect.setY(y);}
        virtual void setSize(int width, int height)  {mRect.setWidth(width), mRect.setHeight(height);}
        
        
        virtual void setDraw(Draw * draw);
        
        
        void setParent (Widget * parent) {mParent  = parent;}
        Widget * parent() {return mParent;}
        
        
        void setColor(Color color)                    {mColor = color;}
        void setColor(int r, int g, int b, int a=255) {mColor.set(r,g,b,a);}
        
        void setEnabled   (bool val) {mEnabled    = val;}
        void setWantEvents(bool val) {mWantEvents = val;}
        void setMouseOver (bool val) {mMouseOver  = val;}
        
        bool enabled()      {return mEnabled;}
        bool wantEvents()   {return mWantEvents;}
        bool mouseOver()    {return mMouseOver;}
        bool hasMouseGrab() {return mMouseGrab;}
        
        Sint16 x()      {return mRect.x();}
        Sint16 y()      {return mRect.y();}
        Point  pos()    {return mRect.pos();}
        Uint16 width()  {return mRect.width();}
        Uint16 height() {return mRect.height();}
        Size   size()   {return mRect.size();}
        
        Point relativePos(Sint16 x, Sint16 y) {return Point(x - mRect.x(), y - mRect.y());}
        void  relativePos(Sint16 x, Sint16 y, Point & target) {return target.set(x - mRect.x(), y - mRect.y());}
        
        const Rect & rect()   {return mRect;}
        const Rect & canvas() {return mCanvas;}
        
    protected:
        Draw  * mDraw;
        
        Widget * mParent;
        
        Rect  mRect;
        Rect  mCanvas;
        Color mColor;
        
        bool mMouseOver;
        bool mMouseGrab;
        bool mEnabled;
        bool mWantEvents;

    };

} //namespace sdltk

#endif
