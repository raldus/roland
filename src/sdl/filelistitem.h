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
#ifndef FILELISTITEM_H
#define FILELISTITEM_H

#include "listitem.h"
#include "point.h"
#include "filename.h"
#include "directory.h"

namespace sdltk
{

    /** @author Fred Klaus development@fkweb.de */
    class FileListItem : public ListItem
    {

    public:
        FileListItem(const FileName & filename);
        virtual ~FileListItem() = default;

        void setFileName(FileName filename) {mFileName = filename;}

    private:
        FileName mFileName;
    };

} //namespace sdltk

#endif //FILELISTITEM_H
