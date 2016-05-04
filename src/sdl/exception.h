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
#ifndef SDLGUIEXCEPTION_H
#define SDLGUIEXCEPTION_H

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <string>
#include <cerrno>
#include "SDL.h"
#include "SDL_opengl.h"

#define SDLGUIERROR(error,details) sdltk::Exception ((details), __FILE__, __LINE__)
#define SDLGUICERROR(eno,error,details) sdltk::Exception (eno, (details), __FILE__, __LINE__)
#define EXCLF __FILE__, __LINE__
#define EXCSDL(error) sdltk::ExcSDL((error), SDL_GetError(), EXCLF);
#define EXCGL(error) sdltk::ExcGL((error), oglGetError(), EXCLF);

namespace sdltk
{
    using std::string;

    /// Handles all Exceptions
    /** All Exceptions are collected in this class.
        exception.h contains the SDLGUIERROR Macro:
        @code SDLGUIERROR(error,details)@endcode This is a short for
        @code sdltk::Exception ((details), __FILE__, __LINE__) @endcode
        @author Fred Klaus */
    class Exception
    {

    public:
        Exception(const char* details=0, const char* file=0, int line=0);
        Exception(const char* message, const char* details=0, const char* file=0, int line=0, bool eno=false);
        Exception(const char* section, const char* message, const char* details=0, const char* file=0, int line=0);
        Exception(const char* section, const char* message, GLenum glerr, const char* file=0, int line=0);
        virtual ~Exception() {}

        void init(bool eno, const char* details, const char* file, int line);

        int         getLine()    const {return mLine;}
        const char* getFile()    const {return mFile.c_str();}
        const char* getMessage() const {return mMsg.c_str();}
        const char* getDetails() const {return mDetails.c_str();}

        virtual void printError();

    protected:
        int    mError;
        int    mLine;
        string mSection;
        string mMsg;
        string mFile;
        string mDetails;
        GLenum mGLErr;

    private:
        void init(bool eno, const char* message, const char* details, const char* file, int line);
    };
    
    
    class ExcSDL : public Exception
    {
    public:
        ExcSDL(const char* message=0, const char* details=0, const char* file=0, int line=0)
                : Exception("[SDL]", message, details, file, line) {}

        ~ExcSDL() {}
    };
    
    
    class ExcGL : public Exception
    {
    public:
        ExcGL(const char* message=0, GLenum glerr=0, const char* file=0, int line=0)
                : Exception("[OpenGL]", message, glerr, file, line) {}

        ~ExcGL() {}
        
        GLenum getEnum() const {return mGLErr;}
    };

    /*
    class ExcMemory : public Exception
    {
    public:
        ExcMemory(const char* details=0, const char* file=0, int line=0, bool eno=false)
                : Exception("Out of Memory", details, file, line) {}

        ~ExcMemory() {}
    };

    
    class ExcIO : public Exception
    {
    public:
        ExcIO(const char* details=0, const char* file=0, int line=0, bool eno=false)
                : Exception("IO-Error", details, file, line) {}

        ~ExcIO() {}
    };
    */

} //namespace sdltk

#endif
