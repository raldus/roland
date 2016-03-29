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
#ifndef SOUND_H
#define SOUND_H

#include "psg.h"
#include "types.h"

#define TAPE_VOLUME 32
#define CYCLE_COUNT_INIT 80000 // 4MHz divided by 50Hz = number of CPU cycles per frame

/** @author Fred Klaus */
class Sound
{

public:
	Sound(Psg* psg=0);
	~Sound() {}

	typedef void (Sound::*Synthesizer)(void);
	typedef void (Sound::*CaseEnvType)(void);

	void init(Psg* psg=0);

	void setSynthesizer(Synthesizer syn) {mSynthesizer=syn;}
	Synthesizer synthesizer()            {return mSynthesizer;}

	inline void setMixerRegister(UBYTE value);
	inline void setAmplA(UBYTE value);
	inline void setAmplB(UBYTE value);
	inline void setAmplC(UBYTE value);
	void caseEnvType0_3__9();
	void caseEnvType4_7__15();
	void caseEnvType8();
	void caseEnvType10();
	void caseEnvType11();
	void caseEnvType12();
	void caseEnvType13();
	void caseEnvType14();
	inline void setEnvelopeRegister(UBYTE value);
	inline void setAYRegister(int num, UBYTE value);
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

	UBYTE* buffer()    {return mSndBuffer;}
	UBYTE* bufferEnd() {return mSndBufferEnd;}
	UBYTE* stream()    {return mSndStream;}

	void setBuffer   (UBYTE* ptr) {mSndBuffer=ptr;}
	void setBufferEnd(UBYTE* ptr) {mSndBufferEnd=ptr;}
	void setStream   (UBYTE* ptr) {mSndStream=ptr;}

	DWORD freqTable(int num) {return mFreqTable[num];}

	void setEnabled(bool value)      {mSndEnabled=value;}
	void setPlaybackRate(uint value) {mSndPlaybackRate=value;}
	void setBits(uint value)         {mSndBits=value;}
	void setStereo(uint value)       {mSndStereo=value;}
	void setVolume(uint value)       {mSndVolume=value;}
	void setDevice(uint value)       {mSndDevice=value;}
	void setBufferSize(uint value)   {mSndBufferSize=value;}
	void setBufferPtr(UBYTE* ptr)    {mSndBufferPtr=ptr;}
	void setBufferPtrDW(DWORD ptr)   {*(DWORD*)mSndBufferPtr=ptr;}
	void setBufferPtrW(WORD ptr)     {*(WORD*)mSndBufferPtr=ptr;}
	void setBufferPtrU(UBYTE ptr)    {*(UBYTE*)mSndBufferPtr=ptr;}
	
	void setBufferFull(bool bf)      {mBufferFull=bf;}
	bool bufferFull()   {return mBufferFull;}

	bool enabled()      {return mSndEnabled;}
	uint playbackRate() {return mSndPlaybackRate;}
	uint bits()         {return mSndBits;}
	uint stereo()       {return mSndStereo;}
	uint volume()       {return mSndVolume;}
	uint device()       {return mSndDevice;}
	uint bufferSize()   {return mSndBufferSize;}
	UBYTE* bufferPtr()  {return mSndBufferPtr;}


	const INT64 & cycleCountInitBoth() const {return mCycleCountInit.both;}
	uint  cycleCountInitLow()          const {return mCycleCountInit.low;}
	uint  cycleCountInitHigh()         const {return mCycleCountInit.high;}

	const INT64 & cycleCountBoth()     const {return mCycleCount.both;}
	uint  cycleCountLow()              const {return mCycleCount.low;}
	uint  cycleCountHigh()             const {return mCycleCount.high;}

	void setCycleCountInitBoth(const INT64 & value) {mCycleCountInit.both=value;}
	void setCycleCountBoth(const INT64 & value)     {mCycleCount.both=value;}
	void setCycleCountLow (uint value)              {mCycleCount.low =value;}
	void setCycleCountHigh(uint value)              {mCycleCount.high=value;}

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
	UBYTE* mSndBufferPtr;
	// **############################***********
	// **############################***********
	// qtodo change and init this !!!!!
	// **############################***********
	static DWORD mFreqTable[];
	UBYTE* mSndBuffer;
	UBYTE* mSndBufferEnd;
	UBYTE* mSndStream;
	UBYTE  mTapeLevel;
	// **############################***********
	// **############################***********
	// **############################***********
	
	bool mBufferFull;

	union
	{
		struct
		{
			uint  low;
			uint  high;
		};
		INT64 both;
	} mCycleCount;

	union
	{
      struct
	  {
         uint low;
         uint high;
      };
      INT64 both;
   } mCycleCountInit;

	//DWORD mLoopCount[2];
	INT64* mLoopCount64;

	INT64 mLoopCountInit;

	union
	{
		struct
		{
			DWORD low;
			DWORD high;
		};
		INT64 both;
	} mLoopCount;

	union TCounter
	{
		struct
		{
			WORD low;
			WORD high;
		};
		DWORD both;
	};
	TCounter mTonCounterA, mTonCounterB, mTonCounterC, mNoiseCounter;

	union
	{
		struct
		{
			WORD low;
			WORD val;
		};
		DWORD seed;
	} mNoise;

	union
	{
		struct
		{
			DWORD low;
			DWORD high;
		};
		INT64 both;
	} mEnvelopeCounter;

	CaseEnvType mCaseEnvType;

	int mLevelPP[256];

	static UWORD mAmplitudesAY[16];

	bool mTonEnA;
	bool mTonEnB;
	bool mTonEnC;
	bool mNoiseEnA;
	bool mNoiseEnB;
	bool mNoiseEnC;
	bool mEnvelopeEnA;
	bool mEnvelopeEnB;
	bool mEnvelopeEnC;

	UBYTE mTonA;
	UBYTE mTonB;
	UBYTE mTonC;

	int mLevelAR[32];
	int mLevelAL[32];
	int mLevelBR[32];
	int mLevelBL[32];
	int mLevelCR[32];
	int mLevelCL[32];

	int   mLevelTape;
	UBYTE mIndexAL;
	UBYTE mIndexAR;
	UBYTE mIndexBL;
	UBYTE mIndexBR;
	UBYTE mIndexCL;
	UBYTE mIndexCR;

	int mPreAmp;
	int mPreAmpMax;
	int mLeftChan;
	int mRightChan;

};


inline void Sound::setAYRegister(int num, UBYTE value)
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

inline void Sound::setMixerRegister(UBYTE value)
{
	mPsg->setMixer(value);
	mTonEnA   = value & 1  ? false : true;
	mNoiseEnA = value & 8  ? false : true;
	mTonEnB   = value & 2  ? false : true;
	mNoiseEnB = value & 16 ? false : true;
	mTonEnC   = value & 4  ? false : true;
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

#endif
