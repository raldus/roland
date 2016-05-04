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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
#include "audio.h"
#include "clock.h"
#include "keytrans.h"
#include "videogl.h"
#include "videostd.h"

using namespace std;

SDL_Surface *screen = 0;

bool joystick = false;

sdltk::Clock mainClock;
sdltk::Clock displayClock;

uint framecount = 0;
uint framecountsum = 0;
bool showfps = false;
bool running = true;

void init();
void quit();
void clearBuffer();
void waitstates();
void mainloop();

inline void update();
inline void display();

Video * video = nullptr;
Prefs prefs;
Cpc cpc(&prefs);
sdltk::Audio audio (&cpc);
sdltk::KeyTrans keytrans;

void init()
{
    cpc.init();
    showfps = prefs.getBool("showfps");

    cpc.fdc().dsk_eject(0);
    cpc.fdc().dsk_load(prefs.getPath("diska").c_str(), 0);

    cpc.fdc().dsk_eject(1);
    cpc.fdc().dsk_load(prefs.getPath("diskb").c_str(), 0);


    //video->init(0, 0, 0, prefs.getBool("fullscreen"));
    if (!video)
    {
        video = new sdltk::VideoStd(&cpc);
        if (video->init(0, 0, 0, false, 2) != 0)
        {
            delete video;
            EOUT("[SDLVidStd]", "could not init video device", "abort");
            quit();
        }
        //initGui();
    }

    audio.init();
    mainClock.init();
    displayClock.init();

    clearBuffer();
}

void mainloop()
{
    SDL_Event event;
    tUBYTE cpc_key;
    static uint frametime =
        (int)((double)20.0 / (double)((cpc.speed() * 25) / 100.0));
    static uint cyclesElapsed = 0;
    static int ticksadjust = 0;
    // bool frameskip=false;

    // audio.pause(false);

    // Check key events
    while (running)
    {
        if (keytrans.hasSequence())
        {
            sdltk::KeyTrans::SeqPair sp = keytrans.sequenceVal();
            cpc_key = sp.keyval;
            if (sp.down)
                cpc.keyboard().setValue((cpc_key >> 4),
                                        (cpc.keyboard().value((cpc_key >> 4)) &
                                         ~bit_values[(cpc_key & 7)]));
            else
                cpc.keyboard().setValue((cpc_key >> 4),
                                        (cpc.keyboard().value((cpc_key >> 4)) |
                                         bit_values[(cpc_key & 7)]));
        }
        else
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_KEYDOWN:
                        // std::cout << "Key: " << event.key.keysym.sym << "\n";
                        cpc_key = keytrans.get(event);
                        if (cpc_key != 0xff)
                        {
                            cpc.keyboard().setValue(
                                (cpc_key >> 4),
                                (cpc.keyboard().value((cpc_key >> 4)) &
                                 ~bit_values[(cpc_key & 7)]));
                        }
                        else
                        {
                            switch ((int)event.key.keysym.sym)
                            {

                                case SDLK_F2:
                                {
                                    audio.pause(true);
                                    SDL_Delay(20);
                                    sdltk::FileSelect *f = new sdltk::FileSelect(
                                        video->screen(), prefs.getPath("diskdir"),
                                        prefs.getPath("diska"), "A: ");
                                    SDL_EnableKeyRepeat(
                                        SDL_DEFAULT_REPEAT_DELAY,
                                        SDL_DEFAULT_REPEAT_INTERVAL);
                                    int ret = f->loop();
                                    SDL_EnableKeyRepeat(0, 0);
                                    if (!ret)
                                    {
                                        std::cout << "A: " << f->filename()
                                                  << "\n";
                                        cpc.fdc().dsk_eject(0);
                                        cpc.fdc().dsk_load(
                                            f->filename().c_str(), 0);
                                        prefs.set("diska", f->filename());
                                    }
                                    clearBuffer();
                                    audio.pause(false);
                                    delete f;
                                }
                                break;

                                case SDLK_F3:
                                {
                                    audio.pause(true);
                                    SDL_Delay(20);
                                    sdltk::FileSelect *f = new sdltk::FileSelect(
                                        video->screen(), prefs.getPath("diskdir"),
                                        prefs.getPath("diskb"), "B: ");
                                    SDL_EnableKeyRepeat(
                                        SDL_DEFAULT_REPEAT_DELAY,
                                        SDL_DEFAULT_REPEAT_INTERVAL);
                                    int ret = f->loop();
                                    SDL_EnableKeyRepeat(0, 0);
                                    if (!ret)
                                    {
                                        std::cout << "B: " << f->filename()
                                                  << "\n";
                                        cpc.fdc().dsk_eject(1);
                                        cpc.fdc().dsk_load(
                                            f->filename().c_str(), 1);
                                        prefs.set("diskb", f->filename());
                                    }
                                    clearBuffer();
                                    audio.pause(false);
                                    delete f;
                                }
                                break;

                                case SDLK_F7:
                                    audio.pause(true);
                                    delete video;
                                    video = new VideoStd(&cpc);
                                    if (video->init() != 0)
                                    {
                                        delete video;
                                        quit();
                                    }
                                    //initGui();
                                    audio.pause(false);
                                    break;

                                case SDLK_F8:
                                    audio.pause(true);
                                    delete video;
                                    video = new VideoGL(&cpc);
                                    if (video->init() != 0)
                                    {
                                        delete video;
                                        video = new VideoStd(&cpc);
                                        if (video->init() != 0)
                                        {
                                            EOUT("[Core]", "could not init any video device", "quit");
                                            delete video;
                                            quit();
                                        }
                                        IOUT("[Core]", "fallback to VideoStd", "OK");
                                    }
                                    //initGui();
                                    audio.pause(false);
                                    break;

                                case SDLK_F9:
                                    audio.pause(true);
                                    SDL_Delay(20);
                                    init();
                                    audio.pause(false);
                                    break;

                                case SDLK_F4:
                                    cout << "Joystick: " << boolalpha
                                         << keytrans.toggleJoystick() << "\n";
                                    break;

                                case SDLK_F5:
                                    audio.pause(true);
                                    video->toggleDoubling();
                                    audio.pause(false);
                                    break;

                                case SDLK_F6:
                                    keytrans.sequenceCatRun();
                                    break;

                                case SDLK_F10:
                                    running = false;
                                    return;
                                    break;

                                case SDLK_F12:
                                    audio.pause(true);
                                    SDL_Delay(20);
                                    video->toggleFullscreen();
                                    audio.pause(false);
                                    break;

                                default:
                                    break;
                            }
                        }
                        break;

                    case SDL_KEYUP:
                        cpc_key = keytrans.get(event);
                        if (cpc_key != 0xff)
                            cpc.keyboard().setValue(
                                (cpc_key >> 4),
                                (cpc.keyboard().value((cpc_key >> 4)) |
                                 bit_values[(cpc_key & 7)]));
                        break;

                    case SDL_QUIT: // We've been politely asked to exit, so
                                   // let's leave
                        running = false;
                        return;

                    default:
                        break;
                }
            }

        if (audio.dist() < audio.mindist())
        {
            ticksadjust=-1; // speed emulation up to compensate @todo what to do here ???
            //cerr <<  "X";

        }
        else if (audio.dist() > audio.maxdist())
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

        // cerr << dwSndDist << " ";
        // if (ticksadjust > 0) {ticksadjust=0; continue;}

        // int t;
        // t=frametime-mainClock.elapsed();
        // cerr << t << " ";
        // if (mainClock.elapsed()<20) continue;
        // mainClock.init();

        // cyclesElapsed = cpc.z80().execute(CYCLE_COUNT_INIT);
        // cerr << cyclesElapsed  << " ";

        cyclesElapsed += cpc.z80().execute(CYCLE_COUNT_INIT);
        //cyclesElapsed = cpc.z80().execute(CYCLE_COUNT_INIT-cyclesElapsed);

        if (cpc.sound().bufferFull())
        {
            //while (!audio.copied()) {SDL_Delay(1);} // delay emulation
            //audio.setCopied(false);
            audio.waitCopied();
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

            cpc.vdu().setScrBase(video->bufferStart()); //@todo do not calculate if no pageflip or do in Video..
            //cpc.vdu().setScrEnd (video->buffer()End);//@todo do not calculate if no pageflip

            //if (mainClock.elapsed() > frametime) frameskip=true; // skip next frame
            if (ticksadjust >= 0) while (mainClock.elapsed()<frametime) {SDL_Delay(1);}
            //while (mainClock.elapsed()<frametime) {} @todo is this  OK ???
            mainClock.init();
        }
    }
}

inline void display()
{
    video->setup();
    video->update();
    return;
    sdltk::Font fnt;

    if (cpc.fdc().led())
    {
        SDL_Rect rect;
        rect.w = 20;
        rect.h = 12;
        rect.x = screen->w - (rect.w + 3);
        rect.y = screen->h - (rect.h + 3);
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 96, 96, 96));

        rect.w = 18;
        rect.h = 10;
        rect.x = screen->w - (rect.w + 4);
        rect.y = screen->h - (rect.h + 4);
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 255, 0));
    }

    if (showfps)
    {
        if (displayClock.elapsed() >= 1000)
        {
            displayClock.init();
            framecountsum = framecount;
            framecount = 0;
        }
        SDL_Rect rectFPS;
        rectFPS.w = 56;
        rectFPS.h = 15;
        rectFPS.x = 5;
        rectFPS.y = screen->h - 18;
        SDL_FillRect(screen, &rectFPS, SDL_MapRGB(screen->format, 96, 96, 96));
        rectFPS.w = 54;
        rectFPS.h = 13;
        rectFPS.x = 6;
        rectFPS.y = screen->h - 17;
        SDL_FillRect(screen, &rectFPS, SDL_MapRGB(screen->format, 0, 0, 0));
        // SDL_FillRect(screen, &rectFPS, 0);
        fnt.write(screen, 5, screen->h - 17, framecountsum);
        fnt.write(screen, 30, screen->h - 17, "/50");
    }

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

    // SDL_UpdateRect(screen, 0, 0, width, height);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    // SDL_UpdateRect(screen, rectFPS.x, rectFPS.y, rectFPS.w, rectFPS.h);

    // SDL_Flip(screen);

    if (SDL_MUSTLOCK(screen))
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
    while (SDL_PollEvent(&event)) {};
}

void quit()
{
    audio.quit();
    SDL_Quit();
    IOUT("[Core]", "SDL", "cleanly finished");
}

int main(int argc, char *argv[])
{

    /* Initializes Audio and Video */
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        cerr << "[SDL] Could not init: " << SDL_GetError() << endl;
        SDL_Quit();
    }

#ifdef USE_MMX
    IOUT("[Core]", "MMX", "enabled");
#else
    IOUT("[Core]", "MMX", "disabled");
#endif

    //bool fs = prefs.getBool("fullscreen");

    //SDL_WM_SetCaption(PACKAGE_STRING, 0);
    Video::setCaption(PACKAGE_STRING);
    //Video::setIcon(datadir + "icon32x32-256.bmp");
    //SDL_WM_GrabInput (SDL_GRAB_ON);
    //SDL_ShowCursor (0);
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EnableKeyRepeat(0, 0);

    init();

    audio.pause(false);

    mainloop();

    quit();
}
