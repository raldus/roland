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
#include "audio.h"

namespace sdltk
{

	Cpc* Audio::mCpc=0;
	volatile bool Audio::mBufferCopied=false;



	Audio::Audio(Cpc * cpc)
	{
		mCpc          = cpc;
		isInit        = false;
		mBufferCopied = false;
		mSpec         = 0;
		mMinSafeDist  = 0;
		mMaxSafeDist  = 0;
	}

	Audio::~Audio()
	{
		quit();
	}

	void Audio::quit()
	{
		if (!isInit) return;
		SDL_Delay(20);
		SDL_CloseAudio();
		if (mCpc->sound().buffer()) delete [] mCpc->sound().buffer();
		if (mSpec) delete mSpec;
		isInit = false;
	}

	uint Audio::alignSamples(uint given)
	{
		uint actual=1;
		while (actual < given)
		{
			actual <<= 1;
		}
		return actual; // return the closest match as 2^n
	}

	int Audio::init()
	{
		if (isInit) quit();

		SDL_AudioSpec *desired;

		if (!mCpc->sound().enabled()) return 0;

		desired = new SDL_AudioSpec;
		mSpec   = new SDL_AudioSpec;

		desired->freq     = mCpc->sound().freqTable(mCpc->sound().playbackRate());
		desired->format   = mCpc->sound().bits() ? AUDIO_S16LSB : AUDIO_S8;
		desired->channels = mCpc->sound().stereo()+1;
		//desired->samples  = audioAlignSamples(desired->freq / 100);
		desired->samples  = alignSamples(desired->freq / 50); ///@todo 100,75 or 50 ??? desired is 20ms (50 is default) at the given frequency
		desired->callback =  &Audio::update;
		desired->userdata = NULL;

		IOUT("[Audio]", "alignSamples", (alignSamples(desired->freq / 50)));

		if (SDL_OpenAudio(desired, mSpec) < 0)
		{
			cerr << "Audio Could not open audio: " << SDL_GetError() << "\n";
			return 1;
		}
		else isInit=true;

		IOUT("[Audio]", "frequence", mCpc->sound().freqTable(mCpc->sound().playbackRate()));
		IOUT("[Audio]", "bits",     (mCpc->sound().bits() ? 16 : 8));
		IOUT("[Audio]", "channels", (mCpc->sound().stereo()+1));
		IOUT("[Audio]", "desired->samples",  desired->samples);
		IOUT("[Audio]", "obtained->samples", mSpec->samples);

		delete desired;

		mCpc->sound().setBufferSize(mSpec->size);                         // size is samples * channels * bytes per sample (1 or 2)
		mCpc->sound().setBuffer(new tUBYTE[mCpc->sound().bufferSize()*6]);       // allocate a ring buffer with 10 segments
		mCpc->sound().setBufferEnd(mCpc->sound().buffer() + (mCpc->sound().bufferSize()*6));
		mCpc->sound().setStream(mCpc->sound().buffer());                        // put the play cursor at the start
		memset(mCpc->sound().buffer(), 0, mCpc->sound().bufferSize()*6);
		mCpc->sound().setBufferPtr(mCpc->sound().buffer() + (mCpc->sound().bufferSize()*3)); // init write cursor

		mMinSafeDist = mCpc->sound().bufferSize()*2; // the closest together the cursors may be
		mMaxSafeDist = mCpc->sound().bufferSize()*4; // the farthest apart the cursors may be

		mCpc->sound().initAY();

		IOUT("[Audio]", "init", "done");

		return 0;
	}

	void Audio::pause(bool value)
	{
		SDL_Delay(20);
		if (mCpc->sound().enabled() && value) SDL_PauseAudio(1);
		else if (mCpc->sound().enabled()) SDL_PauseAudio(0);

		IOUT("[Audio]", "pause", boolalpha << value);
	}

	void Audio::update(void *userdata, Uint8 *stream, int len)
	{

#ifdef USE_MMX
		mmx_memcpy(stream, mCpc->sound().stream(), len);
#else
		memcpy(stream, mCpc->sound().stream(), len);
#endif

		mCpc->sound().setStream(mCpc->sound().stream() + len);
		if (mCpc->sound().stream() >= mCpc->sound().bufferEnd())
		{
			mCpc->sound().setStream(mCpc->sound().buffer());
			mBufferCopied = true;
		}
	}

} //sdl
