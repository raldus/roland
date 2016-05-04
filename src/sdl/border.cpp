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
#include "border.h"

namespace sdltk
{

    Border::Border()
    {
        mColor=0xffffffff;
    }
    
    Border::~Border()
    {}
    
    void Border::setUniSize(uint width, uint height, uint thickness)
    {
        mBorder[bpTop].x = thickness;
        mBorder[bpTop].y = 0;
        mBorder[bpTop].w = width - (2 * thickness);
        mBorder[bpTop].h = thickness;
                
        mBorder[bpBottom].x = thickness;
        mBorder[bpBottom].y = height - thickness;
        mBorder[bpBottom].w = width - (2 * thickness);
        mBorder[bpBottom].h = thickness;
                
        mBorder[bpLeft].x = 0;
        mBorder[bpLeft].y = thickness;
        mBorder[bpLeft].w = thickness;
        mBorder[bpLeft].h = height - (2 * thickness);
                
        mBorder[bpRight].x  = width - thickness;
        mBorder[bpRight].y  = thickness;
        mBorder[bpRight].w  = thickness;
        mBorder[bpRight].h  = height - (2 * thickness);
        
        mBorder[bpLeftTop].x  = 0;
        mBorder[bpLeftTop].y  = 0;
        mBorder[bpLeftTop].w  = thickness;
        mBorder[bpLeftTop].h  = thickness;
        
        mBorder[bpLeftBottom].x  = 0;
        mBorder[bpLeftBottom].y  = height - thickness;
        mBorder[bpLeftBottom].w  = thickness;
        mBorder[bpLeftBottom].h  = thickness;
        
        mBorder[bpRightTop].x  = width - thickness;
        mBorder[bpRightTop].y  = 0;
        mBorder[bpRightTop].w  = thickness;
        mBorder[bpRightTop].h  = thickness;
        
        mBorder[bpRightBottom].x  = width - thickness;
        mBorder[bpRightBottom].y  = height - thickness;
        mBorder[bpRightBottom].w  = thickness;
        mBorder[bpRightBottom].h  = thickness;
    
    }
    
    void Border::paint(SDL_Surface * surface)
    {
        for (uchar i=0; i < 8; i++)
        {
            SDL_FillRect(surface, &mBorder[i], mColor);
        }
    }
    
    void Border::setColor(uint color)
    {
        mColor = color;
    }

}//namespace sdltk

