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
#ifndef PSG_H
#define PSG_H

#include "types.h"

class Sound;

/** @author Fred Klaus */
class Psg
{

public:
    Psg();
    ~Psg() {}

    void init();

    UBYTE control()  {return mControl;}
    UBYTE selected() {return mSelected;}
    UBYTE registerAY(int num) const {return mRegisterAY[num];}
    UBYTE registerAY()        const {return mRegisterAY[mSelected];}

    UBYTE tonALo()     const {return mRegisterAY[0];}
    UBYTE tonAHi()     const {return mRegisterAY[1];}
    UBYTE tonBLo()     const {return mRegisterAY[2];}
    UBYTE tonBHi()     const {return mRegisterAY[3];}
    UBYTE tonCLo()     const {return mRegisterAY[4];}
    UBYTE tonCHi()     const {return mRegisterAY[5];}
    UBYTE noise()      const {return mRegisterAY[6];}
    UBYTE mixer()      const {return mRegisterAY[7];}
    UBYTE amplitudeA() const {return mRegisterAY[8];}
    UBYTE amplitudeB() const {return mRegisterAY[9];}
    UBYTE amplitudeC() const {return mRegisterAY[10];}
    UBYTE envelopeLo() const {return mRegisterAY[11];}
    UBYTE envelopeHi() const {return mRegisterAY[12];}
    UBYTE envType()    const {return mRegisterAY[13];}
    UBYTE portA()      const {return mRegisterAY[14];}
    UBYTE portB()      const {return mRegisterAY[15];}

    WORD tonALoW()     const {return *(WORD*) &mRegisterAY[0];}
    WORD tonAHiW()     const {return *(WORD*) &mRegisterAY[1];}
    WORD tonBLoW()     const {return *(WORD*) &mRegisterAY[2];}
    WORD tonBHiW()     const {return *(WORD*) &mRegisterAY[3];}
    WORD tonCLoW()     const {return *(WORD*) &mRegisterAY[4];}
    WORD tonCHiW()     const {return *(WORD*) &mRegisterAY[5];}
    WORD envelopeLoW() const {return *(WORD*) &mRegisterAY[11];}

    int  amplitudeEnv() const {return mAmplitudeEnv;}
    bool firstPeriod()  const {return mFirstPeriod;}

    void setControl (UBYTE val)            {mControl = val;}
    void setSelected(UBYTE val)            {mSelected = val;}
    void setRegisterAY(int num, UBYTE val) {mRegisterAY[num]=val;}

    void setTonALo(UBYTE value)     {mRegisterAY[0]=value;}
    void setTonAHi(UBYTE value)     {mRegisterAY[1]=value;}
    void setTonBLo(UBYTE value)     {mRegisterAY[2]=value;}
    void setTonBHi(UBYTE value)     {mRegisterAY[3]=value;}
    void setTonCLo(UBYTE value)     {mRegisterAY[4]=value;}
    void setTonCHi(UBYTE value)     {mRegisterAY[5]=value;}
    void setNoise(UBYTE value)      {mRegisterAY[6]=value;}
    void setMixer(UBYTE value)      {mRegisterAY[7]=value;}
    void setAmplitudeA(UBYTE value) {mRegisterAY[8]=value;}
    void setAmplitudeB(UBYTE value) {mRegisterAY[9]=value;}
    void setAmplitudeC(UBYTE value) {mRegisterAY[10]=value;}
    void setEnvelopeLo(UBYTE value) {mRegisterAY[11]=value;}
    void setEnvelopeHi(UBYTE value) {mRegisterAY[12]=value;}
    void setEnvType(UBYTE value)    {mRegisterAY[13]=value;}
    void setPortA(UBYTE value)      {mRegisterAY[14]=value;}
    void setPortB(UBYTE value)      {mRegisterAY[15]=value;}

    void setAmplitudeEnv(int value)  {mAmplitudeEnv=value;}
    void setFirstPeriod (bool value) {mFirstPeriod =value;}

private:

    UBYTE mControl;
    UBYTE mSelected;

    UBYTE mRegisterAY[16];

    int   mAmplitudeEnv;
    bool  mFirstPeriod;

};

#endif
