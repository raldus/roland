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
#ifndef CPC_SOUND_H
#define CPC_SOUND_H

#include "psg.h"
#include "config.h"
#include "types.h"

namespace cpcx
{

    //! The sound chip of the CPC.
    class Sound final
    {

    public:
        Sound(Psg* psg=0);
        ~Sound() ROLAND_DEFAULT

        typedef void (Sound::*Synthesizer)(void);
        typedef void (Sound::*CaseEnvType)(void);

        void init(Psg* psg=0);

        void setSynthesizer(Synthesizer syn) {mSynthesizer=syn;}
        Synthesizer synthesizer()            {return mSynthesizer;}

        inline void setMixerRegister(tUBYTE value);
        inline void setAmplA(tUBYTE value);
        inline void setAmplB(tUBYTE value);
        inline void setAmplC(tUBYTE value);
        void caseEnvType0_3__9();
        void caseEnvType4_7__15();
        void caseEnvType8();
        void caseEnvType10();
        void caseEnvType11();
        void caseEnvType12();
        void caseEnvType13();
        void caseEnvType14();
        inline void setEnvelopeRegister(tUBYTE value);
        inline void setAYRegister(int num, tUBYTE value);
        inline void synthesizerLogicQ();
        inline void synthesizerMixerQ();
        void synthesizerStereo16();
        void synthesizerStereo8();
        void synthesizerMixerQMono();
        void synthesizerMono16();
        void synthesizerMono8();
        void calculateLevelTables();
        void resetAYChipEmulation();
        void initAYCounterVars();
        void initAY();

        tUBYTE* buffer()    const {return mSndBuffer;}
        tUBYTE* bufferEnd() const {return mSndBufferEnd;}
        tUBYTE* stream()    const {return mSndStream;}

        void setBuffer   (tUBYTE* ptr) {mSndBuffer=ptr;}
        void setBufferEnd(tUBYTE* ptr) {mSndBufferEnd=ptr;}
        void setStream   (tUBYTE* ptr) {mSndStream=ptr;}

        tDWORD freqTable(int num) const {return mFreqTable[num];}

        void setEnabled(bool value)      {mSndEnabled=value;}
        void setPlaybackRate(uint value) {mSndPlaybackRate=value;}
        void setBits(uint value)         {mSndBits=value;}
        void setStereo(uint value)       {mSndStereo=value;}
        void setVolume(uint value)       {mSndVolume=value;}
        void setDevice(uint value)       {mSndDevice=value;}
        void setBufferSize(uint value)   {mSndBufferSize=value;}
        void setBufferPtr(tUBYTE* ptr)    {mSndBufferPtr=ptr;}
        void setBufferPtrDW(tDWORD ptr)   {*(tDWORD*)mSndBufferPtr=ptr;}
        void setBufferPtrW(tWORD ptr)     {*(tWORD*)mSndBufferPtr=ptr;}
        void setBufferPtrU(tUBYTE ptr)    {*(tUBYTE*)mSndBufferPtr=ptr;}

        void setBufferFull(bool bf)      {mBufferFull=bf;}
        bool bufferFull()          const {return mBufferFull;}

        bool enabled()      const {return mSndEnabled;}
        uint playbackRate() const {return mSndPlaybackRate;}
        uint bits()         const {return mSndBits;}
        uint stereo()       const {return mSndStereo;}
        uint volume()       const {return mSndVolume;}
        uint device()       const {return mSndDevice;}
        uint bufferSize()   const {return mSndBufferSize;}
        tUBYTE* bufferPtr() const {return mSndBufferPtr;}


        const tINT64 & cycleCountInitBoth() const {return mCycleCountInit.both;}
        uint  cycleCountInitLow()           const {return mCycleCountInit.s.low;}
        uint  cycleCountInitHigh()          const {return mCycleCountInit.s.high;}

        const tINT64 & cycleCountBoth()  const {return mCycleCount.both;}
        uint  cycleCountLow()            const {return mCycleCount.s.low;}
        uint  cycleCountHigh()           const {return mCycleCount.s.high;}

        void setCycleCountInitBoth(const tINT64 & value) {mCycleCountInit.both=value;}
        void setCycleCountBoth(const tINT64 & value)     {mCycleCount.both=value;}
        void setCycleCountLow (uint value)               {mCycleCount.s.low =value;}
        void setCycleCountHigh(uint value)               {mCycleCount.s.high=value;}

    private:
        Psg* mPsg;

        Synthesizer mSynthesizer;

        bool   mSndEnabled;
        uint   mSndPlaybackRate;
        uint   mSndBits;
        uint   mSndStereo;
        uint   mSndVolume;
        uint   mSndDevice;
        uint   mSndBufferSize;
        tUBYTE* mSndBufferPtr;
        // **############################***********
        // **############################***********
        // TODO change and init this !!!!!
        // **############################***********
        tUBYTE* mSndBuffer;
        tUBYTE* mSndBufferEnd;
        tUBYTE* mSndStream;
        tUBYTE  mTapeLevel;
        // **############################***********
        // **############################***********
        // **############################***********

        bool mBufferFull;

        union
        {
            struct S
            {
                uint  low;
                uint  high;
            } s;
            tINT64 both;
        } mCycleCount;

        union
        {
          struct S
          {
             uint low;
             uint high;
          } s;
          tINT64 both;
       } mCycleCountInit;

        //tDWORD mLoopCount[2];
        tINT64* mLoopCount64;

        tINT64 mLoopCountInit;

        union LoopCount
        {
            struct S
            {
                tDWORD low;
                tDWORD high;
            } s;
            tINT64 both;
        } mLoopCount;

        union TCounter
        {
            struct S
            {
                tWORD low;
                tWORD high;
            } s;
            tDWORD both;
        };
        TCounter mTonCounterA, mTonCounterB, mTonCounterC, mNoiseCounter;

        union
        {
            struct S
            {
                tWORD low;
                tWORD val;
            } s;
            tDWORD seed;
        } mNoise;

        union
        {
            struct S
            {
                tDWORD low;
                tDWORD high;
            } s;
            tINT64 both;
        } mEnvelopeCounter;

        CaseEnvType mCaseEnvType;

        int mLevelPP[256];

        static const tUWORD mAmplitudesAY[16];
        static const tDWORD mFreqTable[5];

        bool mTonEnA;
        bool mTonEnB;
        bool mTonEnC;
        bool mNoiseEnA;
        bool mNoiseEnB;
        bool mNoiseEnC;
        bool mEnvelopeEnA;
        bool mEnvelopeEnB;
        bool mEnvelopeEnC;

        tUBYTE mTonA;
        tUBYTE mTonB;
        tUBYTE mTonC;

        int mLevelAR[32];
        int mLevelAL[32];
        int mLevelBR[32];
        int mLevelBL[32];
        int mLevelCR[32];
        int mLevelCL[32];

        int   mLevelTape;
        tUBYTE mIndexAL;
        tUBYTE mIndexAR;
        tUBYTE mIndexBL;
        tUBYTE mIndexBR;
        tUBYTE mIndexCL;
        tUBYTE mIndexCR;

        int mPreAmp;
        int mPreAmpMax;
        int mLeftChan;
        int mRightChan;

    };


    inline void Sound::setAYRegister(int num, tUBYTE value)
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

    inline void Sound::setEnvelopeRegister(tUBYTE value)
    {
        mEnvelopeCounter.s.high = 0;
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

    inline void Sound::setMixerRegister(tUBYTE value)
    {
        mPsg->setMixer(value);
        mTonEnA   = (value & 1 ) ? false : true;
        mTonEnB   = (value & 2 ) ? false : true;
        mTonEnC   = (value & 4 ) ? false : true;
        mNoiseEnA = (value & 8 ) ? false : true;
        mNoiseEnB = (value & 16) ? false : true;
        mNoiseEnC = (value & 32) ? false : true;
    }



    inline void Sound::setAmplA(tUBYTE value)
    {
        mPsg->setAmplitudeA(value);
        mEnvelopeEnA = (value & 16) ? false : true;
    }



    inline void Sound::setAmplB(tUBYTE value)
    {
        mPsg->setAmplitudeB(value);
        mEnvelopeEnB = (value & 16) ? false : true;
    }



    inline void Sound::setAmplC(tUBYTE value)
    {
        mPsg->setAmplitudeC(value);
        mEnvelopeEnC = (value & 16) ? false : true;
    }

} // namespace cpcx

#endif // CPC_SOUND_H
