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
#ifndef SDLTK_FILE_LIST_H
#define SDLTK_FILE_LIST_H

#include "list.h"
#include "filelistitem.h"
#include "filename.h"
#include "directory.h"

namespace sdltk
{

    class Gui;

    //! A filelist to select a file.
    class FileList : public List
    {

    public:
        //! Some initialization
        FileList(Gui * gui, const FileName & dirname, char letter = 0);
        //! Deafault destructor
        ~FileList() ROLAND_DEFAULT

        void init(const FileName & dirname, char letter = 0);

        bool onKeyboard(SDL_KeyboardEvent * event);

        //! Returns a pointer to the selected Item
        FileListItem * selected() const {return (FileListItem*) (*mSelected);}

    private:
        Directory mDirectory;

    };

} // sdltk

#endif //SDLTK_FILE_LIST_H
