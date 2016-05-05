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
#include "cpc.h"

#include <iostream>
//#include <memory>

using namespace std;

Cpc::Cpc(Prefs *prefs)
{
    mSound.init(&mPsg); //@todo Change this:cannot init again.
    init(prefs);
}

int Cpc::init(Prefs *prefs)
{
    if (prefs)
        mPrefs = prefs;
    // if (!mPrefs->read()) cerr << "*** Could not find Preferences-File\n";

    mCpcType = (CpcType)mPrefs->getNum("cpctype");
    mSpeed = mPrefs->getNum("cpcspeed");
    // mMonitor = (Monitor) mPrefs->getNum("monitor");

    mGatearray.init();
    mPpi.init();
    // mPpi.setJumpers(Ppi::Schneider | Ppi::Refresh50Hz | Ppi::Expansion);
    mPpi.setJumpers(mPrefs->getNum("jumpers"));

    mCrtc.init(&mPpi);
    mVdu.init(true, &mCrtc, &mGatearray, &mZ80);
    mVdu.setBorder(mPrefs->getBool("border"));
    //mVdu.setDoublescan(mPrefs->getBool("doublescan"));

    mColours.setIntensity(mPrefs->getNum("intensity"));
    mColours.setMonitor(mPrefs->getNum("monitor"));

    mMemman.init(&mZ80, &mGatearray);
    int ret = mMemman.init(mPrefs->getNum("ramsize"), mPrefs->getPath("cpcrom"),
                           mPrefs->getPath("amsdos"));
    if (ret)
    {
        if (ret == MemMan::ErrRamSize)
            cerr << "Incorrect RAM-size.\n";
        if (ret == MemMan::ErrMemory)
            cerr << "Not enough Memory.\n";
        if (ret & MemMan::ErrCpcRom)
            cerr << "Could not read CPC-ROM file.\n";
        if (ret & MemMan::ErrAmsdos)
            cerr << "Could not read AMSDOS-ROM file.\n";
        return ret;
    }

    // colours.init(screen->format->BitsPerPixel, vdu.scrIntensity());
    // mColours.init(32, 10); // @todo dont hardcode
    for (int n = 0; n < 17; n++) // loop for all colours + border
    {
        mGatearray.setPalette(n, mColours.get(mGatearray.ink(n)));
    }

    // mVdu.setScrIntensity(10);
    mVdu.setCpcRamBase(mMemman.base());

    // mPsg.init();
    // mSound.init(&mPsg);

    mKeyboard.init();

    mZ80.init();

    mZ80.setInHandler(&Cpc::z80_in_handler);
    mZ80.setOutHandler(&Cpc::z80_out_handler);
    mZ80.setWsHandler(&Cpc::waitstates);

    mZ80.setMembank_read(0, mMemman.lowerRom());
    mZ80.setMembank_read(1, mMemman.memBankConfig(0, 1));
    mZ80.setMembank_read(2, mMemman.memBankConfig(0, 2));
    mZ80.setMembank_read(3, mMemman.upperRom());

    mZ80.setMembank_write(0, mMemman.memBankConfig(0, 0));
    mZ80.setMembank_write(1, mMemman.memBankConfig(0, 1));
    mZ80.setMembank_write(2, mMemman.memBankConfig(0, 2));
    mZ80.setMembank_write(3, mMemman.memBankConfig(0, 3));

    return 0;
}

tUBYTE Cpc::z80_in_handler(tREGPAIR port)
{
    tUBYTE retval = 0xff;

    // **********************************************************************
    // *** CRTC
    // **********************************************************************
    if (!(port.b.h & 0x40)) // CRTC Read selected register
    {
        if ((port.b.h & 3) == 3)
        {
            if ((mCrtc.selected() > 11) && (mCrtc.selected() < 18))
                retval = mCrtc.read(); // valid range?
            else
                retval = 0; // write only registers retval=0
        }
    }
    // **********************************************************************
    // *** PPI
    // **********************************************************************
    else if (!(port.b.h & 0x08)) // PPI Port ?
    {
        tUBYTE mPpi_port = port.b.h & 3;
        switch (mPpi_port)
        {
            case 0:                        // read from port A?
                if (mPpi.control() & 0x10) // port A set to input?
                {
                    if ((mPsg.control() & 0xc0) ==
                        0x40) // PSG control set to read?
                    {
                        if (mPsg.selected() < 16) // within valid range?
                        {
                            if (mPsg.selected() == 14) // PSG port A?
                            {
                                if (!(mPsg.registerAY(7) & 0x40))
                                    retval = mKeyboard.value(
                                        mKeyboard.row() &
                                        0x0f); // port A in input mode? read
                                               // mKeyboard matrix node status
                                else
                                    retval =
                                        mPsg.registerAY(14) &
                                        mKeyboard.value(mKeyboard.row() &
                                                        0x0f); // retval=last
                                                               // value w/ logic
                                                               // AND of input
                            }
                            else if (mPsg.selected() == 15) // PSG port B?
                            {
                                if ((mPsg.registerAY(7) & 0x80))
                                    retval = mPsg.registerAY(
                                        15); // port B in output
                                             // mode?retval=stored value
                            }
                            else
                                retval = mPsg.registerAY(); // read PSG register
                        }
                    }
                }
                else
                    retval = mPpi.portA();
                break;

            case 1: // PPI PortB
                if (mPpi.control() & 2)
                    retval = (mPpi.jumpers() | (mCrtc.flags() & Crtc::VS));
                else
                    retval = mPpi.portB();

                /*
                    if (mPpi.control() & 2) // port B set to input?
                    {
                    retval= bTapeLevel | // tape level when reading
                    (CPC.printer ? 0 : 0x40) | // ready line of connected printer
                    (CPC.jumpers & 0x7f) | // manufacturer + 50Hz
                    (CRTC.flags & VS_flag); // VSYNC status
        }
                    else  retval=mPpi.portB(); */ // retval=last programmed value
                break;

            case 2: // PPI PortC
            {
                tUBYTE direction = mPpi.control() & 9;
                tUBYTE retval = mPpi.portC();
                if (direction) // either half set to input?
                {
                    if (direction & 8) // upper half set to input?
                    {
                        retval &= 0x0f; // blank out upper half
                        tUBYTE value =
                            mPpi.portC() & 0xc0; // isolate PSG control bits
                        if (value == 0xc0)       // PSG specify register?
                        {
                            value = 0x80; // change to PSG write register
                        }
                        retval |=
                            value | 0x20; // casette write data is always set

                        // if (CPC.tape_motor) retval |= 0x10;    // set the bit
                        // if the tape motor is running
                    }
                    if (!(direction & 1)) // lower half set to output?
                    {
                        retval |= 0x0f; // invalid - set all bits
                    }
                }
                retval = retval;
                break;
            }
        }
    }
    // **********************************************************************
    // *** FDC
    // **********************************************************************
    else if (!(port.b.h & 0x04)) // external peripheral?
    {
        if (!(port.b.l & 0x01)) // FDC status register?
        {
            retval = mFdc.read_status();
        }
        else // FDC data register
        {
            retval = mFdc.read_data();
        }
        /*
        if (!(port.b.h & 0x04)) { // external peripheral?
        if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) { // FDC?
        if (!(port.b.l & 0x01)) { // FDC status register?
        ret_val = mFdc_read_status();
    } else { // FDC data register
        ret_val = mFdc_read_data();
    }
    }
    }
        */
    }
    return retval;
}

void Cpc::z80_out_handler(tREGPAIR port, tUBYTE value)
{
    // **********************************************************************
    // *** CRTC
    // **********************************************************************
    if (!(port.b.h & 0x40)) // CRTC select?
    {
        tUBYTE sel = (port.b.h & 3);
        if (sel == 0)
            mCrtc.select(value); // CRTC register select?
        else if (sel == 1)       // CRTC write data?
            if (mCrtc.selected() < 16)
                mCrtc.write(value); // only registers 0 - 15 can be written to
    }
    else // ***** @todo watch this !
        // **********************************************************************
        // *** GateArray
        // **********************************************************************
        if ((port.b.h & 0xc0) == 0x40) // GA chip select?
    {
        switch (value >> 6)
        {
            case 0: // select pen
                mGatearray.setPen(value & 0x10 ? 0x10 : value & 0x0f);
                break;

            case 1: // set colour
            {
                tUBYTE col = value & 0x1f; // isolate colour value
                mGatearray.setInk(col);
                mGatearray.setPalette(mColours.get(col));
            }
            break;

            case 2: // set mode and ROM enable/disable

                mGatearray.setRequestedMode(value & 0x03);

                if (value & 0x10) // delay Z80 interrupt?
                {
                    mZ80.setIntPending(0);    // clear pending interrupts
                    mGatearray.setSlCount(0); // reset GA scanline counter
                }

                mGatearray.setRomConfig(value);
                mMemman.memoryManager();
                break;

            case 3: // set memory configuration
                mGatearray.setRamConfig(value);
                mMemman.memoryManager();
                break;

            default:
                break;
        }
    }
    // **********************************************************************
    // *** ROM select
    // **********************************************************************
    if (!(port.b.h & 0x20)) // ROM select?
    {
        // cout << "selectRom: " << (int) (value & 7) << "\n";
        mGatearray.setUpperRom(value); /** @todo check this */
        // mMemman.initBanking();
        mMemman.toggleUpperRom();
    }

    // **********************************************************************
    // *** PPI
    // **********************************************************************
    if (!(port.b.h & 0x08)) // PPI chip select?
    {
        switch (port.b.h & 3)
        {
            case 0: // write to port A?
                mPpi.setA(value);
                if (!(mPpi.control() & 0x10)) // port A set to output?
                {
                    tUBYTE mPsg_data = value;
                    mPsg_write
                }
                break;

            case 1: // write to port B?
                mPpi.setB(value);
                break;

            case 2: // write to port C?
                mPpi.setC(value);

                if (!(mPpi.control() & 1))
                    mKeyboard.setRow(
                        value & 0x0F); // output lower half? @todo check 0x0f
                if (!(mPpi.control() & 8)) // output upper half?
                {
                    // cpc.tape_motor = val & 0x10; // update tape motor control
                    mPsg.setControl(value); // change PSG control
                    tUBYTE mPsg_data = mPpi.portA();
                    mPsg_write;
                }
                break;

            case 3:               // modify PPI control
                if (value & 0x80) // change PPI configuration
                {
                    mPpi.setControl(value);
                    mPpi.setA(0); // clear data for all ports
                    mPpi.setB(0);
                    mPpi.setC(0);
                }
                else // bit manipulation of port C data
                {
                    if (value & 1) // set bit?
                    {
                        tUBYTE bit = (value >> 1) & 7; // isolate bit to set
                        mPpi.setC(mPpi.portC() |
                                  bit_values[bit]); // set requested bit
                        if (!(mPpi.control() & 1))  // output lower half?
                        {
                            // CPC.mKeyboard_line = PPI.portC;
                            mKeyboard.setValue(mPpi.portC());
                        }
                        if (!(mPpi.control() & 8)) // output upper half?
                        {
                            // CPC.tape_motor = PPI.portC & 0x10;
                            mPsg.setControl(mPpi.portC()); // change PSG control
                            tUBYTE mPsg_data = mPpi.portA();
                            mPsg_write
                        }
                    }
                    else
                    {
                        tUBYTE bit = (value >> 1) & 7; // isolate bit to reset
                        mPpi.setC(mPpi.portC() &
                                  ~(bit_values[bit])); // reset requested bit
                        if (!(mPpi.control() & 1))     // output lower half?
                        {
                            // CPC.mKeyboard_line = PPI.portC;
                            mKeyboard.setValue(mPpi.portC());
                        }
                        if (!(mPpi.control() & 8)) // output upper half?
                        {
                            // CPC.tape_motor = PPI.portC & 0x10;
                            mPsg.setControl(mPpi.portC()); // change PSG control
                            tUBYTE mPsg_data = mPpi.portA();
                            mPsg_write
                        }
                    }
                }
                break;

            default:
                break;
        }
    }

    // **********************************************************************
    // *** FDC
    // **********************************************************************
    if (((port.b.h == 0xFA) && (!(port.b.l & 0x80)))) // floppy motor control?
    {
        // DOUT("[FDC] (out) motor: " << (int) value << "\n");
        mFdc.setMotor(value & 0x01);
        mFdc.addFlags(STATUSDRVA_flag | STATUSDRVB_flag);
    }
    else if ((port.b.h == 0xFB) && (!(port.b.l & 0x80))) // FDC data register?
    {
        // DOUT("[FDC] (out) data: " << (int) value << "\n");
        mFdc.write_data(value);
    }
}

void Cpc::waitstates()
{
    mVdu.access_video_memory(mZ80.cycleCount() >> 2);

    if (mSound.enabled())
    {
        mSound.setCycleCountHigh(mSound.cycleCountHigh() + mZ80.cycleCount());
        if (mSound.cycleCountHigh() >= mSound.cycleCountInitHigh())
        {
            mSound.setCycleCountBoth(mSound.cycleCountBoth() -
                                     mSound.cycleCountInitBoth());
            (mSound.*(mSound.synthesizer()))();
        }
    }

    if (mFdc.phase() == EXEC_PHASE)
    {
        mFdc.setTimeout(mFdc.timeout() - mZ80.cycleCount());
        if (mFdc.timeout() <= 0)
        {
            mFdc.addFlags(OVERRUN_flag);
            if (mFdc.cmdDirection() == FDC_TO_CPU)
            {
                mFdc.read_data();
            }
            else
            {
                mFdc.write_data(0xff);
            }
        }
    }
    /*if ((cpc.tape_motor) && (cpc.tape_play_button)) {
     iTapeCycleCount -= iCycleCount;
     if (iTapeCycleCount <= 0) {
     Tape_UpdateLevel();
 }
 }*/
}
