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
#ifndef FILENAME_H
#define FILENAME_H

#include <string>

using std::string;

/// [std::string] extension for filenames
/** This class is derived from std::string and is nothing more or less
	than that, with additional functions to extract parts of a filename
	in a cross-safe manner under consideration of the different delimiters
	and the Drive/Root thing of Linux and Win32. It is also possible to sort
	FileNames caseinsensitive (the default) in STL containers. Also, no slicing
	can occure by passing pointers of FileNames, because no additional variables
	are added at all.
	Just use it as an ordinary std::string.
	@todo Drive/Root not yet considered.
	\n\n<B>Example:</B>\n @include "filename.cpp"
	@author Fred Klaus */
class FileName : public string
{

public:
	FileName() : string("") {}
	FileName(const string & filename) : string(filename) {}
	~FileName() {}

	/** for case sensitive or insensitive sorting */
	bool operator<(const FileName & fn);
	/** for case sensitive or insensitive equalization */
	bool operator==(const FileName & fn);

	//FileName(const FileName & filename) : string(filename) {}
	//const FileName & operator=(const FileName & filename) {*this=filename; return *this;}

	string qualified() const; /**< @todo not yet implemented */
	string base(bool ext=true) const; /**< @bug with ext=false may couse bad name */
	string path() const;
	string relpath() const;
	string ext(bool dot=true) const;

	/** @todo used function -localtime- not threadsafe but -localtime_r- not included in MinGW !!
		@todo implement errorhandling */
	void set(const string & filename) {*this=filename;}
	void setBase(const string & basename, bool ext=true); /**< @todo ext=false not yet implemented @see base */
	void setExt(const string & extension);
	void setPath(const string & path);

	bool isValid();    /**< @todo implement me */
	bool isRelative(); /**< @todo implement me */

	/** Sets wether the operator< should work case sensitive. Switches a static
		private variable and is therefore NOT THREADSAFE but in most cases riskless.
		Asynchronus setting this vally may cause mixed sorted Filenames. */
	static void setCaseSensitiveCompare(bool csc) {mCaseSensitive = csc;}

private:
#ifdef _WIN32
	static const char delim() {return '\\';}
#else
	static const char delim() {return '/';}
#endif

	static bool mCaseSensitive;

};


#endif
