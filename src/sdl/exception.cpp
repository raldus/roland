/***************************************************************************
 *   Copyright (C) 2005-2013 by Fred Klaus <development@fkweb.de>          *
 *                                                                         *
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
#include "exception.h"

#include <iostream>

namespace sdltk
{
    
    Exception::Exception(const char* details, const char* file, int line)
            : mLine(line)
    {
        init(false, details, file, line);
    }

    Exception::Exception(const char* message, const char* details, const char* file, int line, bool eno)
    {
        mMsg = message;
        init(eno, details, file, line);
    }
    
    Exception::Exception(const char* section, const char* message, const char* details, const char* file, int line)
    {
        mSection = section;
        mMsg = message;
        init(false, details, file, line);
    }
    
    Exception::Exception(const char* section, const char* message, GLenum glerr, const char* file, int line)
    {
        mSection = section;
        mMsg = message;
        mGLErr = glerr;
        string details;
        
        switch (glerr)
        {
            case GL_INVALID_ENUM:
                details = "An unacceptable value is specified for an enumerated argument.";
                break;
            
            case GL_INVALID_VALUE:
                details = "A numeric argument is out of range.";
                break;
                
            case GL_INVALID_OPERATION:
                details = "The specified operation is not allowed in the current state.";
                break;
                
            case GL_STACK_OVERFLOW:
                details = "This command would cause a stack overflow";
                break;
                
            case GL_STACK_UNDERFLOW:
                details = "This command would cause a stack underflow";
                break;
                
            case GL_OUT_OF_MEMORY:
                details = "There is not enough memory left to execute the command.";
                break;
                
            case GL_TABLE_TOO_LARGE:
                details = "The specified table exceeds the implementation's maximum supported table size.";
                break;
                
            default:
                details = "An unknown OpenGL error has occured.";
                break;
        };

        init(false, details.c_str(), file, line);
    }

    void Exception::init(bool eno, const char* details, const char* file, int line)
    {
        if (details) mDetails=details;
        else mDetails="";
        
        mLine=line;

        if (file) mFile=file;
        else mFile="unknown file";

        int idx = mFile.rfind('/', mFile.size());
        if (idx) mFile.erase(0, idx+1);

        #ifdef DEBUG
            printError();
        #endif
    }

    void Exception::printError()
    {
        if (mMsg.empty())     mMsg = "Undefined";
        if (mSection.empty()) mSection ="[Undef]";

        std::cerr << mSection << " " << mFile << ": " << mLine << std::endl;
        std::cerr << mSection << " " << mMsg << std::endl;

        if (!mDetails.empty()) std::cerr << mSection << " " << mDetails;
        
        std::cerr << std::endl;

    }

} //namespace sdltk
