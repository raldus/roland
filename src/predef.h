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
#ifndef PREDEF_H
#define PREDEF_H


#include <config.h>



#include <iostream>
#include <iomanip>
using std::cerr;
using std::cout;
using std::setw;
using std::setfill;
using std::left;
using std::right;
using std::boolalpha;
#define EOUT(a,b,c) cerr << setw(12) << setfill(' ') << left << a << "(E) " << setw(40) << setfill('.') << left << b << c << "\n";

#ifdef DEBUG
	//#if DEBUG == 1
		#define IOUT(a,b,c) cout << setw(12) << setfill(' ') << left << a << setw(22) << setfill('.') << left << b  << c << "\n";
		#define DOUT(a,b,c)
	//#endif
		/*
	#if DEBUG == 2
		#define IOUT(a,b,c) cout << setw(12) << setfill(' ') << left << a << setw(22) << setfill('.') << left << b << c << "\n";
		#define DOUT(a,b,c) cout << setw(12) << setfill(' ') << left << a << setw(22) << setfill('.') << left << b << c << "\n";
	#endif

	#define PREOUT \
		void preout() { \
		cout << setw(12) << "Compiler" << __VERSION__<< "\n"; \
		cout << setw(12) << "GnuC" << __GNUC__ << __GNUC_MINOR__ << __GNUC_PATCHLEVEL__ << "\n"; \
		cout << setw(12) << "Buildtime" << __DATE__ << __TIME__ << "\n\n"; \
		} \
		preout(); */
#else
	#define IOUT(a,b,c)
	#define DOUT(a,b,c)
	#define PREOUT
#endif

#define CPC_SCR_WIDTH 1024         // max width
#define CPC_SCR_HEIGHT 312         // max height
#define CPC_VISIBLE_SCR_WIDTH  384 // visible width : 4+40+4 * 8
#define CPC_VISIBLE_SCR_HEIGHT 270 // visible height: 5+25+4 * 8  @todo original is 272


#endif //PREDEF_H
