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
#ifndef SDLTK_LIST_ITEM_H
#define SDLTK_LIST_ITEM_H

#include "button.h"

namespace sdltk
{

    class Point;

    //! @author Fred Klaus development@fkweb.de
    class ListItem : public Button
    {

    public:
        ListItem();
        virtual ~ListItem() = default;

        void setOrigin(Sint16 x, Sint16 y) {mOrigin.set(x, y);}
        void reset() {mRect.setPos(mOrigin.x(), mOrigin.y());}

    protected:
        Point mOrigin;

    };

} // sdltk

#endif // SDLTK_LIST_ITEM_H
