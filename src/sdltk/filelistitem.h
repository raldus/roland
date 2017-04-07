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
#ifndef SDLTK_FILE_LIST_ITEM_H
#define SDLTK_FILE_LIST_ITEM_H

#include "listitem.h"
#include "filename.h"

namespace sdltk
{

    //! Represents a file list entry
    class FileListItem : public ListItem, public FileName
    {

    public:
        FileListItem() ROLAND_DELETE
        //! Constructs an Item based on FileName
        FileListItem(const FileName & filename);
        //! Standarddestructor
        virtual ~FileListItem() ROLAND_DEFAULT

        void setFileName(const FileName & filename) {*this = filename;}

        const FileName & filename() {return *this;}

    };

} // sdltk

#endif // SDLTK_FILE_LIST_ITEM_H
