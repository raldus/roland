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


//#ifdef HAVE_CONFIG_H
	#include <config.h>
//#endif

#ifdef USE_MMX
	#include "mmx_memcpy.h"
#else
	#include <memory>
#endif

#ifdef DEBUG
	#define DOUT(a) cout << a
#else
	#define DOUT(a)
#endif


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "SDL.h"

#include "types.h"
#include "cpc.h"
#include "prefs.h"
#include "fileselect.h"
#include "clock.h"
#include "keytrans.h"


using namespace std;



KeyTrans  keytrans;


SDL_Surface*   screen     = 0;
SDL_Surface*   buffer     = 0;
SDL_AudioSpec* audio_spec = 0;


int width  = CPC_VISIBLE_SCR_WIDTH*2+3;
int height = CPC_VISIBLE_SCR_HEIGHT*2+3;
int depth  = 0;

bool joystick=false;

Clock mainClock;
Clock displayClock;

uint framecount    = 0;
uint framecountsum = 0;
bool showfps       = false;
bool running       = true;


DWORD dwSndMinSafeDist = 0;
DWORD dwSndMaxSafeDist = 0;

bool sndBufferCopied = false;


void init();
void quit();
void clearBuffer();
void waitstates();
void mainloop();

inline uint* calcScreenStart();
inline uint* calcScreenEnd();

inline void update();
inline void display();
inline void audioPause();
inline void audioResume();
inline void audioUpdate(void *userdata, UBYTE *stream, int len);

inline void fillRect(SDL_Rect* rect);

Prefs prefs;
Cpc cpc(&prefs);

void init()
{
	cpc.init();
	showfps=prefs.getBool("showfps");

	//cpc.vdu().setDisplay(&display);

	//*** Video @todo 24bit OK ?
	if(SDL_MUSTLOCK(screen))
		while (SDL_LockSurface(screen))
		{
			cerr << "[SDL] Could not lock screen: " << SDL_GetError() << endl;
			SDL_Delay(20);
		}
	if (screen->format->BitsPerPixel == 16) cpc.vdu().setBpp(Vdu::Bpp16);
	if (screen->format->BitsPerPixel == 24) cpc.vdu().setBpp(Vdu::Bpp24);
	if (screen->format->BitsPerPixel == 32) cpc.vdu().setBpp(Vdu::Bpp32);

	cpc.colours().setDepth(screen->format->BitsPerPixel);

	cpc.vdu().setScrBase(calcScreenStart());
	cpc.vdu().setScrEnd(calcScreenEnd());

	cerr << "[CORE] ScrBase: " << screen->pixels << "\n";
	cerr << "[CORE] ScrEnd:  " << (uint*) screen->pixels+((screen->pitch/4) * height) << "\n";

	cpc.vdu().setScrLineOffset(screen->pitch/2);

	SDL_UnlockSurface(screen);
	// *** Video !!!!


	clearBuffer();

	mainClock.init();
	displayClock.init();
}


inline uint* calcScreenStart()
{
	uint vp = (height - (CPC_VISIBLE_SCR_HEIGHT * 2)) / 2;
	uint hp = (width  - (CPC_VISIBLE_SCR_WIDTH  * 2)) / 2;

	if (screen->format->BitsPerPixel == 16)
	{
		unsigned short int* pix = (unsigned short int*) screen->pixels+((screen->pitch / 2)*vp)+hp;
		return (uint*) pix;
	}
	else
	{
		uint* pix = (uint*) screen->pixels+((screen->pitch / 4)*vp)+hp;
		return (uint*) pix;
	}
}

inline uint* calcScreenEnd()
{
	if (screen->format->BitsPerPixel == 16)
	{
		return (uint*) (unsigned short int*) screen->pixels+((screen->pitch/2) * height);
	}
	else
	{
		return (uint*) screen->pixels+((screen->pitch/4) * height);
	}
}



void mainloop()
{
	SDL_Event event;
	UBYTE cpc_key;
	static uint frametime = (int)((double) 20.0 / (double)((cpc.speed() * 25) / 100.0));
	static uint cyclesElapsed = 0;
	static int  ticksadjust   = 0;
	bool frameskip=false;

	// audioResume();

	// Check key events
	while (running)
	{
		if (keytrans.hasSequence())
		{
			KeyTrans::SeqPair sp=keytrans.sequenceVal();
			cpc_key = sp.keyval;
			if (sp.down) cpc.keyboard().setValue((cpc_key >> 4), (cpc.keyboard().value((cpc_key >> 4)) & ~bit_values[(cpc_key & 7)]));
			else cpc.keyboard().setValue((cpc_key >> 4), (cpc.keyboard().value((cpc_key >> 4)) | bit_values[(cpc_key & 7)]));
		}
		else
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
				case SDL_KEYDOWN:
					//std::cout << "Key: " << event.key.keysym.sym << "\n";
					cpc_key = keytrans.get(event);
					if (cpc_key != 0xff)
					{
						cpc.keyboard().setValue((cpc_key >> 4), (cpc.keyboard().value((cpc_key >> 4)) & ~bit_values[(cpc_key & 7)]));
					}
					else
					{
						switch ((int)event.key.keysym.sym)
						{

						case SDLK_F2:
							{
								audioPause();
								SDL_Delay(20);
								FileSelect* f=new FileSelect(screen,
								                             prefs.getPath("diskdir"), prefs.getPath("diska"), "A: ");
								SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
								int ret=f->loop();
								SDL_EnableKeyRepeat(0, 0);
								if (!ret)
								{
									std::cout << "A: " << f->filename() << "\n";
									cpc.fdc().dsk_eject(0);
									cpc.fdc().dsk_load(f->filename().c_str(), 0);
									prefs.set("diska", f->filename());
								}
								clearBuffer();
								audioResume();
								delete f;
							}
							break;

						case SDLK_F3:
							{
								audioPause();
								SDL_Delay(20);
								FileSelect* f=new FileSelect(screen,
								                             prefs.getPath("diskdir"), prefs.getPath("diskb"), "B: ");
								SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
								int ret=f->loop();
								SDL_EnableKeyRepeat(0, 0);
								if (!ret)
								{
									std::cout << "B: " << f->filename() << "\n";
									cpc.fdc().dsk_eject(1);
									cpc.fdc().dsk_load(f->filename().c_str(), 1);
									prefs.set("diskb", f->filename());
								}
								clearBuffer();
								audioResume();
								delete f;
							}
							break;

						case SDLK_F9:
							audioPause();
							SDL_Delay(20);
							init();
							audioResume();
							break;

						case SDLK_F5:
							cout << "Joystick: " << boolalpha << keytrans.toggleJoystick() << "\n";
							break;

						case SDLK_F4:
							keytrans.sequenceCatRun();
							break;

						case SDLK_F10:
							running=false;
							return;
							break;

						case SDLK_F12:
							audioPause();
							SDL_Delay(20);
							SDL_WM_ToggleFullScreen(screen);
							audioResume();
							break;

						default:
							break;
						}
					}
					break;

				case SDL_KEYUP:
					cpc_key = keytrans.get(event);
					if (cpc_key != 0xff)
						cpc.keyboard().setValue((cpc_key >> 4), (cpc.keyboard().value((cpc_key >> 4)) | bit_values[(cpc_key & 7)]));
					break;

				case SDL_QUIT: // We've been politely asked to exit, so let's leave
					running=false;
					return;

				default:
					break;
				}
			}


		DWORD dwSndDist;
		if (cpc.sound().stream() < cpc.sound().bufferPtr())
		{
			dwSndDist = cpc.sound().bufferPtr()-cpc.sound().stream(); // determine distance between play and write cursors
			//cerr << dwSndDist << " ";
		}
		else
		{
			dwSndDist = (cpc.sound().bufferEnd()-cpc.sound().stream())+(cpc.sound().bufferPtr()-cpc.sound().buffer());
		}
		if (dwSndDist < dwSndMinSafeDist)
		{
			ticksadjust=-1; // speed emulation up to compensate @todo what to do here ???
			//cerr <<  "X";

		}
		else if (dwSndDist > dwSndMaxSafeDist)
		{
			ticksadjust=1; // slow emulation down to compensate
			//SDL_Delay(1);
			//cerr <<  "O";
			continue;
			//cyclesElapsed=0;
		}



		/*
				while (mainClock.elapsed() < frametime+ticksadjust)
				{
					SDL_Delay(1);
				}
		*/

		//cerr << dwSndDist << " ";
		//if (ticksadjust > 0) {ticksadjust=0; continue;}

		//int t;
		//t=frametime-mainClock.elapsed();
		//cerr << t << " ";
		//if (mainClock.elapsed()<20) continue;
		//mainClock.init();


		//cyclesElapsed = cpc.z80().execute(CYCLE_COUNT_INIT);
		//cerr << cyclesElapsed  << " ";


		cyclesElapsed += cpc.z80().execute(CYCLE_COUNT_INIT);
		//cyclesElapsed = cpc.z80().execute(CYCLE_COUNT_INIT-cyclesElapsed);

		if (cpc.sound().bufferFull())
		{
			while (!sndBufferCopied) {} // delay emulation
			sndBufferCopied=false;
			cpc.sound().setBufferFull(false);
			continue;
		}

		if (cpc.vdu().frameCompleted()) // video emulation finished building frame?
		{
			cyclesElapsed=0;
			cpc.vdu().setFrameCompleted(false);

			//if (frameskip) {frameskip=false;mainClock.init();}
			//else {display();framecount++;}


			display();
			framecount++;

			cpc.vdu().setScrBase(calcScreenStart()); //@todo do not calculate if no pageflip
			cpc.vdu().setScrEnd(calcScreenEnd());

			//if (mainClock.elapsed() > frametime) frameskip=true; // skip next frame
			if (ticksadjust >= 0) {while (mainClock.elapsed()<frametime) {}}
			//while (mainClock.elapsed()<frametime) {} @todo is this  OK ???
			mainClock.init();
		}
	}

}

inline void fillRect(SDL_Rect* rect)
{
	for (int iy=0; iy<rect->h; iy++)
	{
		for (int ix=0; ix<rect->w*screen->format->BytesPerPixel; ix++)
		{
			*(((UBYTE*)screen->pixels)
			  + ((screen->pitch * (rect->y+iy)) )
			  + ((rect->x*screen->format->BytesPerPixel)+ix)) = 0;
		}
	}
}

inline void display()
{
	Font fnt;



	if (cpc.fdc().led())
	{
		SDL_Rect rect;
		rect.w=20;
		rect.h=12;
		rect.x=screen->w-(rect.w+3);
		rect.y=screen->h-(rect.h+3);
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 96, 96, 96));

		rect.w=18;
		rect.h=10;
		rect.x=screen->w-(rect.w+4);
		rect.y=screen->h-(rect.h+4);
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 255, 0));
	}

	if (showfps)
	{
		if (displayClock.elapsed() >= 1000)
		{
			displayClock.init();
			framecountsum=framecount;
			framecount=0;
		}
		SDL_Rect rectFPS;
		rectFPS.w=56;
		rectFPS.h=15;
		rectFPS.x=5;
		rectFPS.y=screen->h-18;
		//fillRect(&rectFPS);
		SDL_FillRect(screen, &rectFPS, SDL_MapRGB(screen->format, 96, 96, 96));
		rectFPS.w=54;
		rectFPS.h=13;
		rectFPS.x=6;
		rectFPS.y=screen->h-17;
		SDL_FillRect(screen, &rectFPS, SDL_MapRGB(screen->format, 0, 0, 0));
		//SDL_FillRect(screen, &rectFPS, 0);
		fnt.write(screen, 5,  screen->h-17, framecountsum);
		fnt.write(screen, 30, screen->h-17, "/50");
	}

	if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

	//SDL_UpdateRect(screen, 0, 0, width, height);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	//SDL_UpdateRect(screen, rectFPS.x, rectFPS.y, rectFPS.w, rectFPS.h);

	//SDL_Flip(screen);

	if(SDL_MUSTLOCK(screen))
		while (SDL_LockSurface(screen))
		{
			cerr << "[SDL] Could not lock screen: " << SDL_GetError() << "\n";
			SDL_Delay(20);
		}
}

void clearBuffer()
{
	SDL_Event event;
	cpc.keyboard().init();
	while(SDL_PollEvent(&event)) {};
}



inline void audioUpdate(void *userdata, UBYTE *stream, int len)
{
	#ifdef USE_MMX
		mmx_memcpy(stream, cpc.sound().stream(), len);
	#else
		memcpy(stream, cpc.sound().stream(), len);
	#endif

	sndBufferCopied = true;

	cpc.sound().setStream(cpc.sound().stream() + len);
	if (cpc.sound().stream() >= cpc.sound().bufferEnd()) cpc.sound().setStream(cpc.sound().buffer());
}

int audioAlignSamples(int given)
{
	int actual=1;
	while (actual < given)
	{
		actual <<= 1;
	}
	return actual; // return the closest match as 2^n
}

int initSound()
{
	SDL_AudioSpec *desired, *obtained;

	if (!cpc.sound().enabled()) return 0;

	desired  = new SDL_AudioSpec;
	obtained = new SDL_AudioSpec;

	desired->freq     = cpc.sound().freqTable(cpc.sound().playbackRate());
	desired->format   = cpc.sound().bits() ? AUDIO_S16LSB : AUDIO_S8;
	desired->channels = cpc.sound().stereo()+1;
	//desired->samples  = audioAlignSamples(desired->freq / 100);
	desired->samples  = audioAlignSamples(desired->freq / 50); // @todo 100,75 or 50 ??? desired is 20ms (50 is default) at the given frequency
	desired->callback = audioUpdate;
	desired->userdata = NULL;

	cout << "[SDL_AUDIO] daudioAlign:  " << (audioAlignSamples(desired->freq / 50)) << "\n";

	if (SDL_OpenAudio(desired, obtained) < 0)
	{
		cerr << "[SDL_AUDIO] Could not open audio: " << SDL_GetError() << "\n";
		return 1;
	}
	cout << "[SDL_AUDIO] Frequence: " << cpc.sound().freqTable(cpc.sound().playbackRate()) << "\n";
	cout << "[SDL_AUDIO] Bits:      " << (cpc.sound().bits() ? 16 : 8) << "\n";
	cout << "[SDL_AUDIO] Channels:  " << (cpc.sound().stereo()+1) << "\n";

	cout << "[SDL_AUDIO] desired->samples:  " << desired->samples << "\n";
	delete desired;
	audio_spec = obtained;
	cout << "[SDL_AUDIO] obtained->samples: " << desired->samples << "\n";

	cpc.sound().setBufferSize(audio_spec->size);                         // size is samples * channels * bytes per sample (1 or 2)
	cpc.sound().setBuffer(new UBYTE[cpc.sound().bufferSize()*6]);        // allocate a ring buffer with 10 segments
	cpc.sound().setBufferEnd(cpc.sound().buffer() + (cpc.sound().bufferSize()*6));
	cpc.sound().setStream(cpc.sound().buffer());                         // put the play cursor at the start
	memset(cpc.sound().buffer(), 0, cpc.sound().bufferSize()*6);
	cpc.sound().setBufferPtr(cpc.sound().buffer() + (cpc.sound().bufferSize()*3)); // init write cursor

	dwSndMinSafeDist = cpc.sound().bufferSize()*2; // the closest together the cursors may be
	dwSndMaxSafeDist = cpc.sound().bufferSize()*4; // the farthest apart the cursors may be

	cpc.sound().initAY();

	cout << "[SDL_AUDIO] init OK\n";

	return 0;
}

inline void audioPause()
{
	if (cpc.sound().enabled()) SDL_PauseAudio(1);
	cout << "[SDL_AUDIO] pause\n";
}

inline void audioResume()
{
	if (cpc.sound().enabled()) SDL_PauseAudio(0);
	cout << "[SDL_AUDIO] resume\n";
}

void audioShutdown()
{
	SDL_Delay(20);
	SDL_CloseAudio();
	if (cpc.sound().buffer()) delete [] cpc.sound().buffer();
	if (audio_spec)           delete audio_spec;
}

void quit()
{
	audioShutdown();
	SDL_Quit();
}

int main(int argc, char *argv[])
{

	/* Initializes Audio and Video */
	if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER)< 0)
	{
		cerr <<"[SDL] Could not init: " << SDL_GetError() << endl;
		SDL_Quit();
	}



#ifdef USE_MMX
	cout << "[CORE] MMX enabled" << "\n";
#endif

	bool fs=prefs.getBool("fullscreen");

	screen = SDL_SetVideoMode(width, height, depth,
	                          SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_HWPALETTE|SDL_ANYFORMAT| (fs ? SDL_FULLSCREEN : 0));
	if (screen == NULL)
	{
		cerr << "[SDL_VIDEO] Couldn't set " << width << "x" << height << "-" << screen->format->BitsPerPixel << " video mode: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	char buf[16];
	memset(buf, 0, 16);
	SDL_VideoDriverName(buf, 15);
	string driver=buf;
	cout << "[SDL_VIDEO] Driver: " << driver << "\n";
	cout << "[SDL_VIDEO] BPP: " << (int) screen->format->BitsPerPixel <<"\n";

	SDL_WM_SetCaption(PACKAGE_STRING, NULL);
	//SDL_WM_GrabInput (SDL_GRAB_ON);
	SDL_ShowCursor (0);
	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_EnableKeyRepeat(0, 0);




	initSound();
	init();

	if(SDL_MUSTLOCK(screen))
		while (SDL_LockSurface(screen))
		{
			cerr << "[SDL_VIDEO] Could not lock screen: " << SDL_GetError() << endl;
			SDL_Delay(20);
		}


	audioResume();

	mainloop();

	quit();
}
