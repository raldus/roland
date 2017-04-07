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
#ifndef PSG_H
#define PSG_H

#include "compspec.h"
#include "types.h"

namespace cpcx
{

    class Sound;

    /** @author Fred Klaus */
    class Psg final
    {

    public:
        Psg() {init();}
        ~Psg() ROLAND_DEFAULT

        void init();

        tUBYTE control()           const {return mControl;}
        tUBYTE selected()          const {return mSelected;}
        tUBYTE registerAY(int num) const {return mRegisterAY[num];}
        tUBYTE registerAY()        const {return mRegisterAY[mSelected];}

        tUBYTE tonALo()     const {return mRegisterAY[0];}
        tUBYTE tonAHi()     const {return mRegisterAY[1];}
        tUBYTE tonBLo()     const {return mRegisterAY[2];}
        tUBYTE tonBHi()     const {return mRegisterAY[3];}
        tUBYTE tonCLo()     const {return mRegisterAY[4];}
        tUBYTE tonCHi()     const {return mRegisterAY[5];}
        tUBYTE noise()      const {return mRegisterAY[6];}
        tUBYTE mixer()      const {return mRegisterAY[7];}
        tUBYTE amplitudeA() const {return mRegisterAY[8];}
        tUBYTE amplitudeB() const {return mRegisterAY[9];}
        tUBYTE amplitudeC() const {return mRegisterAY[10];}
        tUBYTE envelopeLo() const {return mRegisterAY[11];}
        tUBYTE envelopeHi() const {return mRegisterAY[12];}
        tUBYTE envType()    const {return mRegisterAY[13];}
        tUBYTE portA()      const {return mRegisterAY[14];}
        tUBYTE portB()      const {return mRegisterAY[15];}

        tWORD tonALoW()     const {return *(tWORD*) &mRegisterAY[0];}
        tWORD tonAHiW()     const {return *(tWORD*) &mRegisterAY[1];}
        tWORD tonBLoW()     const {return *(tWORD*) &mRegisterAY[2];}
        tWORD tonBHiW()     const {return *(tWORD*) &mRegisterAY[3];}
        tWORD tonCLoW()     const {return *(tWORD*) &mRegisterAY[4];}
        tWORD tonCHiW()     const {return *(tWORD*) &mRegisterAY[5];}
        tWORD envelopeLoW() const {return *(tWORD*) &mRegisterAY[11];}

        int  amplitudeEnv() const {return mAmplitudeEnv;}
        bool firstPeriod()  const {return mFirstPeriod;}

        void setControl (tUBYTE val)            {mControl  = val;}
        void setSelected(tUBYTE val)            {mSelected = val;}
        void setRegisterAY(int num, tUBYTE val) {mRegisterAY[num] = val;}

        void setTonALo(tUBYTE value)     {mRegisterAY[0]  = value;}
        void setTonAHi(tUBYTE value)     {mRegisterAY[1]  = value;}
        void setTonBLo(tUBYTE value)     {mRegisterAY[2]  = value;}
        void setTonBHi(tUBYTE value)     {mRegisterAY[3]  = value;}
        void setTonCLo(tUBYTE value)     {mRegisterAY[4]  = value;}
        void setTonCHi(tUBYTE value)     {mRegisterAY[5]  = value;}
        void setNoise(tUBYTE value)      {mRegisterAY[6]  = value;}
        void setMixer(tUBYTE value)      {mRegisterAY[7]  = value;}
        void setAmplitudeA(tUBYTE value) {mRegisterAY[8]  = value;}
        void setAmplitudeB(tUBYTE value) {mRegisterAY[9]  = value;}
        void setAmplitudeC(tUBYTE value) {mRegisterAY[10] = value;}
        void setEnvelopeLo(tUBYTE value) {mRegisterAY[11] = value;}
        void setEnvelopeHi(tUBYTE value) {mRegisterAY[12] = value;}
        void setEnvType(tUBYTE value)    {mRegisterAY[13] = value;}
        void setPortA(tUBYTE value)      {mRegisterAY[14] = value;}
        void setPortB(tUBYTE value)      {mRegisterAY[15] = value;}

        void setAmplitudeEnv(int value)  {mAmplitudeEnv = value;}
        void setFirstPeriod (bool value) {mFirstPeriod  = value;}

    private:

        tUBYTE mControl;
        tUBYTE mSelected;

        tUBYTE mRegisterAY[16];

        int   mAmplitudeEnv;
        bool  mFirstPeriod;

    };

} // cpc

#endif
