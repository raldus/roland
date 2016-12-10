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
#include "filelist.h"
#include "filelistitem.h"

namespace sdltk
{

    FileList::FileList(Gui * gui, const FileName & dirname, char letter) : List(gui)
    {
        setPos(50, 50);
        setSize(gui->video()->size().width() - 100, gui->video()->size().height() - 100);
        setEnabled(true);
        init(dirname, letter);
    }

    void FileList::init(const FileName & dirname, char letter)
    {
        mDirectory.scan(dirname, Directory::Options::BothDir, letter);
        if (mDirectory.empty()) return;
        mDirectory.sort();

        clear();
        List::init();
        FileListItem * item;
        for  (auto file : mDirectory)
        {
            item = new FileListItem(file);
            item->setSize(mGui->video()->size().width() - 100, 25);
            item->setBorder(true);
            add(item);
        }
    }

    bool FileList::onKeyboard(SDL_KeyboardEvent * event)
    {
        if (!mEnabled || empty()) return List::onKeyboard(event);

        if (((int) event->keysym.sym >= SDLK_a) && ((int) event->keysym.sym <= SDLK_z))
        {
            init(mDirectory.path(), (int) event->keysym.sym);
            return true;
        }
        else if (((int) event->keysym.sym >= SDLK_0) && ((int) event->keysym.sym <= SDLK_9))
        {
            init(mDirectory.path(), (int) event->keysym.sym);
            return true;
        }
        else if((int) event->keysym.sym == SDLK_SPACE)
        {
            init(mDirectory.path(), 0);
            return true;
        }
        //return true; // blocks event reusing but arrow keys too :(
        return List::onKeyboard(event);
    }

} // sdltk
