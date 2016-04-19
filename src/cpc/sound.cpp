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
#include "sound.h"

#include <cmath>
#include <iostream>

#include "cpc.h"

extern Cpc cpc;

UWORD Sound::mAmplitudesAY[16] = {0,     836,   1212,  1773,  2619,  3875,
                                  5397,  8823,  10392, 16706, 23339, 29292,
                                  36969, 46421, 55195, 65535};

#define MAX_FREQ_ENTRIES 5
DWORD Sound::mFreqTable[MAX_FREQ_ENTRIES] = {11025, 22050, 44100, 48000, 96000};

Sound::Sound(Psg *psg)
{
    mPsg = psg;
    init(psg);
}

void Sound::init(Psg *psg)
{
    mSndEnabled = true;
    mSndPlaybackRate = 1;
    mSndBits = 1;
    mSndStereo = 1;
    mSndVolume = 90;
    mSndDevice = 0;
    mSndBufferSize = 0;
    mSndBufferPtr = 0;
    mCycleCountInit.both = CYCLE_COUNT_INIT;

    mBufferFull = false;

    if (psg)
        mPsg = psg;
    if (mPsg)
        mPsg->init();
    else
        return;

    for (int n = 0; n < 16; n++)
    {
        setAYRegister(
            n, mPsg->registerAY(n)); // init sound emulation with valid values
    }
}

/*
inline void Sound::setMixerRegister(UBYTE value)
{
    mPsg->setMixer(value);
    mTonEnA = value & 1 ? false : true;
    mNoiseEnA = value & 8 ? false : true;
    mTonEnB = value & 2 ? false : true;
    mNoiseEnB = value & 16 ? false : true;
    mTonEnC = value & 4 ? false : true;
    mNoiseEnC = value & 32 ? false : true;
}



inline void Sound::setAmplA(UBYTE value)
{
    mPsg->setAmplitudeA(value);
    mEnvelopeEnA = value & 16 ? false : true;
}



inline void Sound::setAmplB(UBYTE value)
{
    mPsg->setAmplitudeB(value);
    mEnvelopeEnB = value & 16 ? false : true;
}



inline void Sound::setAmplC(UBYTE value)
{
    mPsg->setAmplitudeC(value);
    mEnvelopeEnC = value & 16 ? false : true;
}
*/

void Sound::caseEnvType0_3__9()
{
    if (mPsg->firstPeriod())
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() - 1);
        if (!mPsg->amplitudeEnv())
        {
            mPsg->setFirstPeriod(false);
        }
    }
}

void Sound::caseEnvType4_7__15()
{
    if (mPsg->firstPeriod())
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() + 1);
        if (mPsg->amplitudeEnv() == 32)
        {
            mPsg->setFirstPeriod(false);
            mPsg->setAmplitudeEnv(0);
        }
    }
}

void Sound::caseEnvType8()
{
    mPsg->setAmplitudeEnv((mPsg->amplitudeEnv() - 1) & 31);
}

void Sound::caseEnvType10()
{
    if (mPsg->firstPeriod())
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() - 1);
        if (mPsg->amplitudeEnv() == -1)
        {
            mPsg->setFirstPeriod(false);
            mPsg->setAmplitudeEnv(0);
        }
    }
    else
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() + 1);
        if (mPsg->amplitudeEnv() == 32)
        {
            mPsg->setFirstPeriod(true);
            mPsg->setAmplitudeEnv(31);
        }
    }
}

void Sound::caseEnvType11()
{
    if (mPsg->firstPeriod())
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() - 1);
        if (mPsg->amplitudeEnv() == -1)
        {
            mPsg->setFirstPeriod(false);
            mPsg->setAmplitudeEnv(31);
        }
    }
}

void Sound::caseEnvType12()
{
    mPsg->setAmplitudeEnv((mPsg->amplitudeEnv() + 1) & 31);
}

void Sound::caseEnvType13()
{
    if (mPsg->firstPeriod())
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() + 1);
        if (mPsg->amplitudeEnv() == 32)
        {
            mPsg->setFirstPeriod(false);
            mPsg->setAmplitudeEnv(31);
        }
    }
}

void Sound::caseEnvType14()
{
    if (!mPsg->firstPeriod())
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() - 1);
        if (mPsg->amplitudeEnv() == -1)
        {
            mPsg->setFirstPeriod(true);
            mPsg->setAmplitudeEnv(0);
        }
    }
    else
    {
        mPsg->setAmplitudeEnv(mPsg->amplitudeEnv() + 1);
        if (mPsg->amplitudeEnv() == 32)
        {
            mPsg->setFirstPeriod(false);
            mPsg->setAmplitudeEnv(31);
        }
    }
}

/*
inline void Sound::setEnvelopeRegister(UBYTE value)
{
    mEnvelopeCounter.high = 0;
    mPsg->setFirstPeriod(true);
    if (!(value & 4))
    {
        mPsg->setAmplitudeEnv(32);
    }
    else
    {
        mPsg->setAmplitudeEnv(-1);
    }
    mPsg->setEnvType(value);
    switch (value)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 9:
            mCaseEnvType = &Sound::caseEnvType0_3__9;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 15:
            mCaseEnvType = &Sound::caseEnvType4_7__15;
            break;
        case 8:
            mCaseEnvType = &Sound::caseEnvType8;
            break;
        case 10:
            mCaseEnvType = &Sound::caseEnvType10;
            break;
        case 11:
            mCaseEnvType = &Sound::caseEnvType11;
            break;
        case 12:
            mCaseEnvType = &Sound::caseEnvType12;
            break;
        case 13:
            mCaseEnvType = &Sound::caseEnvType13;
            break;
        case 14:
            mCaseEnvType = &Sound::caseEnvType14;
            break;
    }
}

void Sound::setAYRegister(int num, UBYTE value)
{
    switch(num)
    {
        case 13:
            setEnvelopeRegister(value & 15);
            break;
        case 1:
        case 3:
        case 5:
            mPsg->setRegisterAY(num, value & 15);
            break;
        case 6:
            mPsg->setNoise(value & 31);
            break;
        case 7:
            setMixerRegister(value & 63);
            break;
        case 8:
            setAmplA(value & 31);
            break;
        case 9:
            setAmplB(value & 31);
            break;
        case 10:
            setAmplC(value & 31);
            break;
        case 0:
        case 2:
        case 4:
        case 11:
        case 12:
            mPsg->setRegisterAY(num, value);
            break;
    }
}
*/

inline void Sound::synthesizerLogicQ()
{
    // std::cerr << "synthesizer 3b *************" << std::endl;
    mTonCounterA.high++;
    if (mTonCounterA.high >= mPsg->tonALoW()) ///< @todo check this !!!
    {
        mTonCounterA.high = 0;
        mTonA ^= 1;
    }
    mTonCounterB.high++;
    if (mTonCounterB.high >= mPsg->tonBLoW()) ///< @todo check this !!!
    {
        mTonCounterB.high = 0;
        mTonB ^= 1;
    }
    mTonCounterC.high++;
    if (mTonCounterC.high >= mPsg->tonCLoW()) ///< @todo check this !!!
    {
        mTonCounterC.high = 0;
        mTonC ^= 1;
    }
    mNoiseCounter.high++;
    if ((!(mNoiseCounter.high & 1)) &&
        (mNoiseCounter.high >= (mPsg->noise() << 1)))
    {
        mNoiseCounter.high = 0;
        mNoise.seed = (((((mNoise.seed >> 13) ^ (mNoise.seed >> 16)) & 1) ^ 1) |
                       mNoise.seed << 1) &
                      0x1ffff;
    }
    if (!mEnvelopeCounter.high)
    {
        (this->*mCaseEnvType)();
    }
    mEnvelopeCounter.high++;
    if (mEnvelopeCounter.high >= mPsg->envelopeLoW()) ///< @todo check this !!!
    {
        mEnvelopeCounter.high = 0;
    }
}

inline void Sound::synthesizerMixerQ()
{
    int LevL, LevR, k;

    LevL = mTapeLevel ? mLevelTape : 0; // start with the tape signal
    /*
    if (snd_pp_device)
    {
        LevL += mLevelPP[cpc.printer_port];
    }
    */

    LevR = LevL;
    if (mTonEnA)
    {
        if ((!mEnvelopeEnA) || (mPsg->tonALoW() > 4)) ///< @todo check this !!!
        {
            k = mTonA;
        }
        else
        {
            k = 1;
        }
    }
    else
    {
        k = 1;
    }
    if (mNoiseEnA)
    {
        k &= mNoise.val;
    }
    if (k)
    {
        if (mEnvelopeEnA)
        {
            LevL += mLevelAL[mPsg->amplitudeA() * 2 + 1];
            LevR += mLevelAR[mPsg->amplitudeA() * 2 + 1];
        }
        else
        {
            LevL += mLevelAL[mPsg->amplitudeEnv()];
            LevR += mLevelAR[mPsg->amplitudeEnv()];
        }
    }

    if (mTonEnB)
    {
        if ((!mEnvelopeEnB) || (mPsg->tonBLoW() > 4)) ///< @todo check this !!!
        {
            k = mTonB;
        }
        else
        {
            k = 1;
        }
    }
    else
    {
        k = 1;
    }
    if (mNoiseEnB)
    {
        k &= mNoise.val;
    }
    if (k)
    {
        if (mEnvelopeEnB)
        {
            LevL += mLevelBL[mPsg->amplitudeB() * 2 + 1];
            LevR += mLevelBR[mPsg->amplitudeB() * 2 + 1];
        }
        else
        {
            LevL += mLevelBL[mPsg->amplitudeEnv()];
            LevR += mLevelBR[mPsg->amplitudeEnv()];
        }
    }

    if (mTonEnC)
    {
        if ((!mEnvelopeEnC) || (mPsg->tonCLoW() > 4)) ///< @todo check this !!!
        {
            k = mTonC;
        }
        else
        {
            k = 1;
        }
    }
    else
    {
        k = 1;
    }
    if (mNoiseEnC)
    {
        k &= mNoise.val;
    }
    if (k)
    {
        if (mEnvelopeEnC)
        {
            LevL += mLevelCL[mPsg->amplitudeC() * 2 + 1];
            LevR += mLevelCR[mPsg->amplitudeC() * 2 + 1];
        }
        else
        {
            LevL += mLevelCL[mPsg->amplitudeEnv()];
            LevR += mLevelCR[mPsg->amplitudeEnv()];
        }
    }

    mLeftChan += LevL;
    mRightChan += LevR;
}

void Sound::synthesizerStereo16()
{
    int tickcounter = 0;
    while (mLoopCount.high)
    {
        synthesizerLogicQ();
        synthesizerMixerQ();
        tickcounter++;
        mLoopCount.high--;
    }
    mLoopCount.both += mLoopCountInit;
    REGPAIR val;
    val.w.l = mLeftChan / tickcounter;
    val.w.h = mRightChan / tickcounter;
    *(DWORD *)mSndBufferPtr = val.d; // @todo check this,write to mixing buffer
                                     // ########################
    mSndBufferPtr += 4;
    mLeftChan = 0;
    mRightChan = mLeftChan;
    if (mSndBufferPtr >= mSndBufferEnd)
    {
        mSndBufferPtr = mSndBuffer;
        mBufferFull = true;
    }
}

void Sound::synthesizerStereo8()
{
    int tickcounter = 0;
    while (mLoopCount.high)
    {
        synthesizerLogicQ();
        synthesizerMixerQ();
        tickcounter++;
        mLoopCount.high--;
    }
    mLoopCount.both += mLoopCountInit;
    REGPAIR val;
    val.b.l = 128 + mLeftChan / tickcounter;
    val.b.h = 128 + mRightChan / tickcounter;
    setBufferPtrW(val.w.l); // @todo check this,write to mixing buffer
    mSndBufferPtr += 2;
    mLeftChan = 0;
    mRightChan = mLeftChan;
    if (mSndBufferPtr >= mSndBufferEnd)
    {
        mSndBufferPtr = mSndBuffer;
        mBufferFull = true;
    }
}

inline void Sound::synthesizerMixerQMono()
{
    int Lev, k;

    Lev = mTapeLevel ? mLevelTape : 0; // start with the tape signal
    if (device())
    {
        // @todo fix this: Lev += mLevelPP[cpc.printer_port];
    }

    if (mTonEnA)
    {
        if ((!mEnvelopeEnA) || (mPsg->tonALoW() > 4)) ///< @todo check this !!!
        {
            k = mTonA;
        }
        else
        {
            k = 1;
        }
    }
    else
    {
        k = 1;
    }
    if (mNoiseEnA)
    {
        k &= mNoise.val;
    }
    if (k)
    {
        if (mEnvelopeEnA)
        {
            Lev += mLevelAL[mPsg->amplitudeA() * 2 + 1];
        }
        else
        {
            Lev += mLevelAL[mPsg->amplitudeEnv()];
        }
    }

    if (mTonEnB)
    {
        if ((!mEnvelopeEnB) || (mPsg->tonBLoW() > 4)) ///< @todo check this !!!
        {
            k = mTonB;
        }
        else
        {
            k = 1;
        }
    }
    else
    {
        k = 1;
    }
    if (mNoiseEnB)
    {
        k &= mNoise.val;
    }
    if (k)
    {
        if (mEnvelopeEnB)
        {
            Lev += mLevelBL[mPsg->amplitudeB() * 2 + 1];
        }
        else
        {
            Lev += mLevelBL[mPsg->amplitudeEnv()];
        }
    }

    if (mTonEnC)
    {
        if ((!mEnvelopeEnC) || (mPsg->tonCLo() > 4)) ///< @todo check this !!!
        {
            k = mTonC;
        }
        else
        {
            k = 1;
        }
    }
    else
    {
        k = 1;
    }
    if (mNoiseEnC)
    {
        k &= mNoise.val;
    }
    if (k)
    {
        if (mEnvelopeEnC)
        {
            Lev += mLevelCL[mPsg->amplitudeC() * 2 + 1];
        }
        else
        {
            Lev += mLevelCL[mPsg->amplitudeEnv()];
        }
    }

    mLeftChan += Lev;
}

void Sound::synthesizerMono16()
{
    int tickcounter = 0;
    while (mLoopCount.high)
    {
        synthesizerLogicQ();
        synthesizerMixerQMono();
        tickcounter++;
        mLoopCount.high--;
    }
    mLoopCount.both += mLoopCountInit;
    setBufferPtrW(mLeftChan /
                  tickcounter); // @todo check this,write to mixing buffer
    mSndBufferPtr += 2;
    mLeftChan = 0;
    if (mSndBufferPtr >= mSndBufferEnd)
    {
        mSndBufferPtr = mSndBuffer;
        mBufferFull = true;
    }
}

void Sound::synthesizerMono8()
{
    int tickcounter = 0;
    while (mLoopCount.high)
    {
        synthesizerLogicQ();
        synthesizerMixerQMono();
        tickcounter++;
        mLoopCount.high--;
    }
    mLoopCount.both += mLoopCountInit;
    setBufferPtrU(128 +
                  mLeftChan /
                      tickcounter); // @todo check this,write to mixing buffer
    mSndBufferPtr++;
    mLeftChan = 0;
    if (mSndBufferPtr >= mSndBufferEnd)
    {
        mSndBufferPtr = mSndBuffer;
        mBufferFull = true;
    }
}

void Sound::calculateLevelTables()
{
    int i, b, l, r;
    int mIndexA, mIndexB, mIndexC;
    double k;

    mIndexA = mIndexAL;
    mIndexB = mIndexBL;
    mIndexC = mIndexCL;
    l = mIndexA + mIndexB + mIndexC;
    r = mIndexAR + mIndexBR + mIndexCR;
    if (stereo())
    {
        if (l < r)
        {
            l = r;
        }
    }
    else
    {
        l += r;
        mIndexA += mIndexAR;
        mIndexB += mIndexBR;
        mIndexC += mIndexCR;
    }
    if (l == 0)
    {
        l++;
    }
    if (!bits())
    { // 8 bits per sample?
        r = 127;
    }
    else
    {
        r = 32767;
    }
    l = 255 * r / l;
    for (i = 0; i < 16; i++)
    {
        b = (int)rint(mIndexA / 255.0 * mAmplitudesAY[i]);
        b = (int)rint(b / 65535.0 * l);
        mLevelAL[i * 2] = b;
        mLevelAL[i * 2 + 1] = b;
        b = (int)rint(mIndexAR / 255.0 * mAmplitudesAY[i]);
        b = (int)rint(b / 65535.0 * l);
        mLevelAR[i * 2] = b;
        mLevelAR[i * 2 + 1] = b;
        b = (int)rint(mIndexB / 255.0 * mAmplitudesAY[i]);
        b = (int)rint(b / 65535.0 * l);
        mLevelBL[i * 2] = b;
        mLevelBL[i * 2 + 1] = b;
        b = (int)rint(mIndexBR / 255.0 * mAmplitudesAY[i]);
        b = (int)rint(b / 65535.0 * l);
        mLevelBR[i * 2] = b;
        mLevelBR[i * 2 + 1] = b;
        b = (int)rint(mIndexC / 255.0 * mAmplitudesAY[i]);
        b = (int)rint(b / 65535.0 * l);
        mLevelCL[i * 2] = b;
        mLevelCL[i * 2 + 1] = b;
        b = (int)rint(mIndexCR / 255.0 * mAmplitudesAY[i]);
        b = (int)rint(b / 65535.0 * l);
        mLevelCR[i * 2] = b;
        mLevelCR[i * 2 + 1] = b;
    }
    k = exp(volume() * log(2) / mPreAmpMax) - 1;
    for (i = 0; i < 32; i++)
    {
        mLevelAL[i] = (int)rint(mLevelAL[i] * k);
        mLevelAR[i] = (int)rint(mLevelAR[i] * k);
        mLevelBL[i] = (int)rint(mLevelBL[i] * k);
        mLevelBR[i] = (int)rint(mLevelBR[i] * k);
        mLevelCL[i] = (int)rint(mLevelCL[i] * k);
        mLevelCR[i] = (int)rint(mLevelCR[i] * k);
    }
    if (!bits())
    { // 8 bits per sample?
        mLevelTape = -(int)rint((TAPE_VOLUME / 2) * k);
    }
    else
    {
        mLevelTape = -(int)rint((TAPE_VOLUME * 128) * k);
    }
    for (i = 0, b = 255; i < 256; i++)
    { // calculate the 256 levels of the Digiblaster/Soundplayer
        mLevelPP[i] = -(int)rint(((b << 8) / 65535.0 * l) * k);
        b--;
    }
}

void Sound::resetAYChipEmulation()
{
    mTonCounterA.both = 0;
    mTonCounterB.both = 0;
    mTonCounterC.both = 0;
    mNoiseCounter.both = 0;
    mEnvelopeCounter.both = 0;
    mTonA = 0;
    mTonB = 0;
    mTonC = 0;
    mLeftChan = 0;
    mRightChan = 0;
    mNoise.seed = 0xffff;
}

void Sound::initAYCounterVars()
{
    mCycleCountInit.both = (INT64)rint(
        (4000000 * ((cpc.speed() * 25) / 100.0)) / mFreqTable[playbackRate()] *
        4294967296.0); // number of Z80 cycles per sample

    mLoopCountInit = (INT64)rint(
        1000000.0 / (4000000.0 * ((cpc.speed() * 25) / 100.0)) / 8.0 *
        mCycleCountInit.both); // number of AY counter increments per sample
    mLoopCount.both = mLoopCountInit;
}

void Sound::initAY()
{
    mIndexAL = 255;
    mIndexAR = 13;
    mIndexBL = 170;
    mIndexBR = 170;
    mIndexCL = 13;
    mIndexCR = 255;
    mPreAmpMax = 100;

    calculateLevelTables();
    initAYCounterVars();
    resetAYChipEmulation();

    if (stereo()) // stereo mode?
    {
        if (bits())
            setSynthesizer(&Sound::synthesizerStereo16); // 16 bits per sample?
        else
            setSynthesizer(&Sound::synthesizerStereo8); // 8 bits
    }
    else // mono
    {
        if (bits())
            setSynthesizer(&Sound::synthesizerMono16); // 16 bits per sample?
        else
            setSynthesizer(&Sound::synthesizerMono8); // 8 bits
    }
}
