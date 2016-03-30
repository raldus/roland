/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus                                      *
 *   frednet@web.de                                                        *
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
#ifndef FILEINFO_H
#define FILEINFO_H

#include <sys/stat.h>
#include <string>

#ifdef _WIN32
    #ifndef nlink_t
        typedef unsigned short int nlink_t;
    #endif
    #ifndef uid_t
        typedef unsigned int uid_t;
    #endif
    #ifndef gid_t
        typedef unsigned int gid_t;
    #endif
    #ifndef blkcnt_t
        typedef unsigned long int blkcnt_t;
    #endif
#endif

using std::string;

/// Represents Fileattributes
/** With this, you can get attributes like size, different dates/times and
    checks whether the file is a regular file, pipe or folder.\n
    This class uses the gnulibc standard types.
    On most architectures (i386) the sizes of the used types treated as follows: @code
    off_t       uint
    mode_t      uint
    ino_t       unsigned long int
    ino64_t     unsigned long long int
    dev_t       int
    nlink_t     unsigned short int
    blkcnt_t    unsigned long int
    blkcnt64_t  unsigned long long int @endcode
    if compiled with _FILE_OFFSET_BITS == 64:
    @code
    off_t       unsigned long int
    ino_t       ino64_t
    blkcnt_t    blkcnt64_t @endcode
    @todo implement mime - size and date does not work
    \n\n<B>Example:</B>\n @include "fileinfo.cpp"
    @author Fred Klaus */
class FileInfo
{

public:
    /** Standardconstructor. The only action is to set validflag to false */
    FileInfo() : mValid(false) {}
    FileInfo(const string & filename);
    ~FileInfo() {};

    off_t   size()   const {return mStat.st_size;}
    time_t  atime()  const {return mStat.st_atime;}
    time_t  mtime()  const {return mStat.st_mtime;}
    time_t  ctime()  const {return mStat.st_ctime;}
    mode_t  mode()   const {return mStat.st_mode;}
    ino_t   inode()  const {return mStat.st_ino;}
    dev_t   device() const {return mStat.st_dev;}
#ifdef _WIN32
    nlink_t nlinks() const {return 0;}
    uid_t   uid()    const {return 0;}
    gid_t   gid()    const {return 0;}
    //unsigned long int ausec() const {return mStat.st_atime_usec;}
    //unsigned long int musec() const {return mStat.st_mtime_usec;}
    //unsigned long int cusec() const {return mStat.st_ctime_usec;}
    blkcnt_t blocks() const {return 0;} /**< in 512 KB. */
    unsigned int optblocksize() const {return 512;} /**< optimal block size for reading of writing. */

    bool isLink() const {return false;}
#else
    nlink_t nlinks() const {return mStat.st_nlink;}
    uid_t   uid()    const {return mStat.st_uid;}
    gid_t   gid()    const {return mStat.st_gid;}
    //unsigned long int ausec() const {return mStat.st_atime_usec;}
    //unsigned long int musec() const {return mStat.st_mtime_usec;}
    //unsigned long int cusec() const {return mStat.st_ctime_usec;}
    blkcnt_t blocks() const {return mStat.st_blocks;} /**< in 512 KB. */
    unsigned int optblocksize() const {return mStat.st_blksize;} /**< optimal block size for reading of writing. */

    bool isLink() const {return S_ISLNK (mStat.st_mode) ? true : false;}
#endif
    bool isDir()  const {return S_ISDIR(mStat.st_mode) ? true : false;}
    bool isFile() const {return S_ISDIR(mStat.st_mode) ? false : true;}

    bool isValid() {return mValid;}

    bool read(const string & fname, bool followlink=false);

private:
    struct stat mStat;
    bool mValid;

};


#endif

