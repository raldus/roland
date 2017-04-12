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
#include "fdc.h"

#include "fdcconst.h"
#include <fstream>

#define LOAD_RESULT_WITH_STATUS                                                \
    mResult[RES_ST0] |= 0x40; /* AT */                                         \
    mResult[RES_ST1] |= 0x80; /* End of Cylinder */                            \
    if (mCommand[CMD_CODE] != 0x42)                                            \
    {/* continue only if not a read track command */                           \
        if ((mResult[RES_ST1] & 0x7f) || (mResult[RES_ST2] & 0x7f))            \
        {                             /* any 'error bits' set? */              \
            mResult[RES_ST1] &= 0x7f; /* mask out End of Cylinder */           \
            if ((mResult[RES_ST1] & 0x20) || (mResult[RES_ST2] & 0x20))        \
            {                             /* DE and/or DD? */                  \
                mResult[RES_ST2] &= 0xbf; /* mask out Control Mark */          \
            }                                                                  \
            else if (mResult[RES_ST2] & 0x40)                                  \
            {                             /* Control Mark? */                  \
                mResult[RES_ST0] &= 0x3f; /* mask out AT */                    \
                mResult[RES_ST1] &= 0x7f; /* mask out End of Cylinder */       \
            }                                                                  \
        }                                                                      \
    }

#define LOAD_RESULT_WITH_CHRN                                                  \
    mResult[RES_C] =                                                           \
        mCommand[CMD_C]; /* load result with current CHRN values */            \
    mResult[RES_H] = mCommand[CMD_H];                                          \
    mResult[RES_R] = mCommand[CMD_R];                                          \
    mResult[RES_N] = mCommand[CMD_N];

namespace cpcx
{
    Fdc::Fdc()
    {
        mCmdTable.get(0).set(0x03, 3, 0, FDC_TO_CPU, &Fdc::specify); // specify
        mCmdTable.get(1)
            .set(0x04, 2, 1, FDC_TO_CPU, &Fdc::drvstat); // sense device status
        mCmdTable.get(2).set(0x07, 2, 0, FDC_TO_CPU, &Fdc::recalib); // recalibrate
        mCmdTable.get(3)
            .set(0x08, 1, 2, FDC_TO_CPU, &Fdc::intstat); // sense interrupt status
        mCmdTable.get(4).set(0x0f, 3, 0, FDC_TO_CPU, &Fdc::seek); // seek
        mCmdTable.get(5)
            .set(0x42, 9, 7, FDC_TO_CPU, &Fdc::readtrk); // read diagnostic
        mCmdTable.get(6).set(0x45, 9, 7, CPU_TO_FDC, &Fdc::write); // write data
        mCmdTable.get(7).set(0x46, 9, 7, FDC_TO_CPU, &Fdc::read);  // read data
        mCmdTable.get(8)
            .set(0x49, 9, 7, CPU_TO_FDC, &Fdc::write); // write deleted data
        mCmdTable.get(9).set(0x4a, 2, 7, FDC_TO_CPU, &Fdc::readID); // read id
        mCmdTable.get(10)
            .set(0x4c, 9, 7, FDC_TO_CPU, &Fdc::read); // read deleted data
        mCmdTable.get(11).set(0x4d, 6, 7, CPU_TO_FDC, &Fdc::writeID); // write id
        mCmdTable.get(12).set(0x51, 9, 7, CPU_TO_FDC, &Fdc::scan);    // scan equal
        mCmdTable.get(13)
            .set(0x59, 9, 7, CPU_TO_FDC, &Fdc::scan); // scan low or equal
        mCmdTable.get(14)
            .set(0x5d, 9, 7, CPU_TO_FDC, &Fdc::scan); // scan high or equal

        mTimeout = 0;
        mMotor = 0;
        mLed = false;
        mFlags = 0;
        mPhase = 0;
        mByteCount = 0;
        mBufferCount = 0;
        mCmdLength = 0;
        mResLength = 0;
        mCmdDirection = 0;

        pbGPBuffer = 0;
        mBufferPtr = 0;
        mBufferEndPtr = 0;

        for (int i = 0; i < 12; i++)
            mCommand[i] = 0;
        for (int i = 0; i < 8; i++)
            mResult[i] = 0;

        mActiveDrive = 0;
        mActiveTrack = 0;

        mReadStatusDelay = 0;
        mBytesTransferred = 0;

        mPhase = CMD_PHASE;
        mFlags = STATUSDRVA_flag | STATUSDRVB_flag;

        pbGPBuffer = new tUBYTE[128 * 1024];
    }

    void Fdc::write_data(tUBYTE val)
    {
        int idx;

    #ifdef DEBUG_FDC
        if (mPhase == CMD_PHASE)
        {
            if (mByteCount)
            {
                fprintf(pfoDebug, "%02x ", val);
            }
            else
            {
                fprintf(pfoDebug, "\n%02x: ", val);
            }
        }
    #endif

        switch (mPhase)
        {
            case CMD_PHASE: // in mCommand mPhase?
                if (mByteCount)
                {                                 // receiving mCommand parameters?
                    mCommand[mByteCount++] = val; // copy to buffer
                    if (mByteCount == mCmdLength)
                    {                        // received all mCommand UBYTEs?
                        mByteCount = 0;      // clear tUBYTE counter
                        mPhase = EXEC_PHASE; // switch to execution mPhase
                        (this->*mCmdHandler)();
                    }
                }
                else
                { // first mCommand tUBYTE received
                    if (val & 0x20)
                    {                        // skip DAM or DDAM?
                        mFlags |= SKIP_flag; // DAM/DDAM will be skipped
                        val &= ~0x20;        // reset skip bit in mCommand tUBYTE
                    }
                    else
                    {
                        mFlags &= ~SKIP_flag; // make sure skip inidicator is off
                    }
                    for (idx = 0; idx < MAX_CMD_COUNT; idx++)
                    { // loop through all known FDC mCommands
                        if (mCmdTable.get(idx).cmd() == val)
                        { // do we have a match?
                            break;
                        }
                    }
                    if (idx != MAX_CMD_COUNT)
                    { // valid mCommand received
                        mCmdLength = mCmdTable.get(idx)
                                         .cmdLength(); // mCommand length in UBYTEs
                        mResLength = mCmdTable.get(idx)
                                         .resLength(); // result length in UBYTEs
                        mCmdDirection = mCmdTable.get(idx)
                                            .cmdDirection(); // direction is CPU to
                                                             // FDC, or FDC to CPU
                        mCmdHandler =
                            mCmdTable.get(idx)
                                .cmdHandler(); // pointer to mCommand handler

                        mCommand[mByteCount++] =
                            val; // copy mCommand code to buffer
                        if (mByteCount == mCmdLength)
                        {                   // already received all mCommand UBYTEs?
                            mByteCount = 0; // clear tUBYTE counter
                            mPhase = EXEC_PHASE; // switch to execution mPhase
                            (this->*mCmdHandler)();
                            // mCmdHandler();
                        }
                    }
                    else
                    {                      // unknown mCommand received
                        mResult[0] = 0x80; // indicate invalid mCommand
                        mResLength = 1;
                        mPhase = RESULT_PHASE; // switch to result mPhase
                    }
                }
                break;
            case EXEC_PHASE: // in execution mPhase?
                if (mCmdDirection == CPU_TO_FDC)
                { // proper direction?
                    mTimeout = OVERRUN_TIMEOUT;
                    if ((mFlags & SCAN_flag))
                    { // processing any of the scan mCommands?
                        if (val != 0xff)
                        { // no comparison on CPU tUBYTE = 0xff
                            switch ((mCommand[CMD_CODE] & 0x1f))
                            {
                                case 0x51: // scan equal
                                    if (val != *mBufferPtr)
                                    {
                                        mResult[RES_ST2] &=
                                            0xf7; // reset Scan Equal Hit
                                        mFlags |= SCANFAILED_flag;
                                    }
                                    break;
                                case 0x59: // scan low or equal
                                    if (val != *mBufferPtr)
                                    {
                                        mResult[RES_ST2] &=
                                            0xf7; // reset Scan Equal Hit
                                    }
                                    if (val > *mBufferPtr)
                                    {
                                        mFlags |= SCANFAILED_flag;
                                    }
                                    break;
                                case 0x5d: // scan high or equal
                                    if (val != *mBufferPtr)
                                    {
                                        mResult[RES_ST2] &=
                                            0xf7; // reset Scan Equal Hit
                                    }
                                    if (val < *mBufferPtr)
                                    {
                                        mFlags |= SCANFAILED_flag;
                                    }
                                    break;
                            }
                        }
                        mBufferPtr++; // advance sector data pointer
                    }
                    else
                    {
                        *mBufferPtr++ = val; // write tUBYTE to sector
                    }
                    if (mBufferPtr > mBufferEndPtr)
                    {
                        mBufferPtr = mActiveTrack->data(); // wrap around
                    }
                    if (--mBufferCount == 0)
                    { // processed all data?
                        if ((mFlags & SCAN_flag))
                        { // processing any of the scan mCommands?
                            if ((mFlags & SCANFAILED_flag) &&
                                (mCommand[CMD_R] != mCommand[CMD_EOT]))
                            {
                                mCommand[CMD_R] +=
                                    mCommand[CMD_STP]; // advance to next sector
                                cmd_scan();
                            }
                            else
                            {
                                if ((mFlags & SCANFAILED_flag))
                                {
                                    mResult[RES_ST2] |= 0x04; // Scan Not Satisfied
                                }

                                LOAD_RESULT_WITH_CHRN

                                mPhase = RESULT_PHASE; // switch to result mPhase
                            }
                        }
                        else if (mCommand[CMD_CODE] == 0x4d)
                        { // write ID mCommand?
                            tUWORD sector_size, track_size;
                            tUBYTE *pbPtr, *pbDataPtr;

                            if (mActiveTrack->sectors() != 0)
                            { // track is formatted?
                                delete[] mActiveTrack
                                    ->data(); // dealloc memory for old track data
                            }
                            sector_size = 128
                                          << mCommand[CMD_C]; // determine number of
                                                              // UBYTEs from N value
                            if (((sector_size + 62 + mCommand[CMD_R]) *
                                 mCommand[CMD_H]) >
                                6144 - 154) /**< @todo make this configureable !!
                                               CPC.max_tracksize*/
                            {               // track size exceeds maximum?
                                mActiveTrack->setSectors(0); // 'unformat' track
                            }
                            else
                            {
                                int sector;

                                track_size = sector_size * mCommand[CMD_H];
                                mActiveTrack->setSectors(mCommand[CMD_H]);
                                mActiveTrack->setData(
                                    new tUBYTE[track_size]); // attempt to allocate
                                                            // the required memory
                                pbDataPtr = mActiveTrack->data();
                                pbPtr = pbGPBuffer;
                                for (sector = 0; sector < mCommand[CMD_H]; sector++)
                                {
                                    memcpy(mActiveTrack->sector(sector).CHRN(),
                                           pbPtr, 4); // copy CHRN
                                    memset(mActiveTrack->sector(sector).flags(), 0,
                                           2); // clear ST1 & ST2
                                    mActiveTrack->sector(sector).setData(
                                        pbDataPtr); // store pointer to sector data
                                    pbDataPtr += sector_size;
                                    pbPtr += 4;
                                }
                                memset(mActiveTrack->data(), mCommand[CMD_N],
                                       track_size); // fill track data with
                                                    // specified tUBYTE value
                            }
                            pbPtr =
                                pbGPBuffer +
                                ((mCommand[CMD_H] - 1) *
                                 4); // pointer to the last CHRN passed to writeID
                            memcpy(&mResult[RES_C], pbPtr,
                                   4); // copy sector's CHRN to result buffer
                            mResult[RES_N] = mCommand[CMD_C]; // overwrite with the
                                                              // N value from the
                                                              // writeID mCommand

                            mActiveDrive->setAltered(
                                1); // indicate that the image has been modified
                            mPhase = RESULT_PHASE; // switch to result mPhase
                        }
                        else if (mCommand[CMD_R] != mCommand[CMD_EOT])
                        {                      // haven't reached End of Track?
                            mCommand[CMD_R]++; // advance to next sector
                            cmd_write();
                        }
                        else
                        {
                            mActiveDrive->setAltered(
                                1); // indicate that the image has been modified

                            mResult[RES_ST0] |= 0x40; // AT
                            mResult[RES_ST1] |= 0x80; // End of Cylinder

                            LOAD_RESULT_WITH_CHRN

                            mPhase = RESULT_PHASE; // switch to result mPhase
                        }
                    }
                }
                break;
        }
    }

    tUBYTE Fdc::read_status()
    {
        tUBYTE val;

        val = 0x80; // data register ready
        if (mPhase == EXEC_PHASE)
        { // in execution mPhase?
            if (mReadStatusDelay)
            {
                val = 0x10; // FDC is busy
                mReadStatusDelay--;
            }
            else
            {
                val |= 0x30; // FDC is executing & busy
            }
            if (mCmdDirection == FDC_TO_CPU)
            {
                val |= 0x40; // FDC is sending data to the CPU
            }
        }
        else if (mPhase == RESULT_PHASE)
        {                // in result mPhase?
            val |= 0x50; // FDC is sending data to the CPU, and is busy
        }
        else
        { // in mCommand mPhase
            if (mByteCount)
            {                // receiving mCommand parameters?
                val |= 0x10; // FDC is busy
            }
        }
        return val;
    }

    tUBYTE Fdc::read_data()
    {
        tUBYTE val;

        val = 0xff; // default value
        switch (mPhase)
        {
            case EXEC_PHASE: // in execution mPhase?
                if (mCmdDirection == FDC_TO_CPU)
                { // proper direction?
                    mTimeout = OVERRUN_TIMEOUT;
                    val = *mBufferPtr++; // read tUBYTE from current sector
    #ifdef DEBUG_FDC
                    if (!(mFlags & OVERRUN_flag))
                    {
                        dwBytesTransferred++;
                    }
    #endif
                    if (mBufferPtr >= mBufferEndPtr)
                    {
                        mBufferPtr = mActiveTrack->data(); // wrap around
                    }
                    if (!(--mBufferCount))
                    { // completed the data transfer?
                        if (mFlags & RNDDE_flag)
                        { // simulate random Data Errors?
                            // ***! random DE handling
                        }
                        mActiveDrive->setCurrentSector(
                            mActiveDrive->currentSector() +
                            1); // increase sector index
                        if (mFlags & OVERRUN_flag)
                        { // overrun condition detected?
                            mFlags &= ~OVERRUN_flag;
                            mResult[RES_ST0] |= 0x40; // AT
                            mResult[RES_ST1] |= 0x10; // Overrun

                            LOAD_RESULT_WITH_CHRN

                            mPhase = RESULT_PHASE; // switch to result mPhase
                        }
                        else
                        {
                            if (mCommand[CMD_CODE] == 0x42)
                            { // read track mCommand?
                                if ((--mCommand[CMD_EOT]))
                                { // continue reading sectors?
                                    if (mActiveDrive->currentSector() >=
                                        mActiveTrack->sectors())
                                    { // index beyond number of sectors for this
                                      // track?
                                        mActiveDrive->setCurrentSector(
                                            0); // reset index
                                    }
                                    mCommand[CMD_R]++; // advance to next sector
                                    cmd_readtrk();
                                }
                                else
                                {
                                    LOAD_RESULT_WITH_STATUS

                                    LOAD_RESULT_WITH_CHRN

                                    mPhase =
                                        RESULT_PHASE; // switch to result mPhase
                                }
                            }
                            else
                            { // normal read (deleted) data mCommand
                                if (!((mResult[RES_ST1] & 0x31) ||
                                      (mResult[RES_ST2] & 0x21)))
                                { // no error bits set?
                                    if (mCommand[CMD_R] != mCommand[CMD_EOT])
                                    { // haven't reached End of Track?
                                        mCommand[CMD_R]++; // advance to next sector
                                        cmd_read();
                                    }
                                    else
                                    {
                                        LOAD_RESULT_WITH_STATUS

                                        LOAD_RESULT_WITH_CHRN

                                        mPhase =
                                            RESULT_PHASE; // switch to result mPhase
                                    }
                                }
                                else
                                {
                                    LOAD_RESULT_WITH_STATUS

                                    LOAD_RESULT_WITH_CHRN

                                    mPhase =
                                        RESULT_PHASE; // switch to result mPhase
                                }
                            }
                        }
                    }
                }
                break;
            case RESULT_PHASE:               // in result mPhase?
                val = mResult[mByteCount++]; // copy value from buffer

    #ifdef DEBUG_FDC
                if (dwBytesTransferred)
                {
                    fprintf(pfoDebug, "{%d} ", dwBytesTransferred);
                    dwBytesTransferred = 0;
                }
                fprintf(pfoDebug, "[%02x] ", val);
    #endif

                if (mByteCount == mResLength)
                {                         // sent all result UBYTEs?
                    mFlags &= ~SCAN_flag; // reset scan mCommand flag
                    mByteCount = 0;       // clear tUBYTE counter
                    mPhase = CMD_PHASE;   // switch to mCommand mPhase
                    mLed = false;         // turn the drive LED off
                }
                break;
        }
        return val;
    }

    void Fdc::specify()
    {
        mPhase = CMD_PHASE; // switch back to mCommand mPhase (Fdc::specify has no
                            // result mPhase!)
    }

    void Fdc::drvstat()
    {
        tUBYTE val;

        check_unit();                 // switch to target drive
        val = mCommand[CMD_UNIT] & 7; // keep head and unit of mCommand
        if ((mActiveDrive->writeProtected()) || (mActiveDrive->tracks() == 0))
        {                // write protected, or disk missing?
            val |= 0x48; // set Write Protect + Two Sided (?)
        }
        if ((mActiveDrive->tracks()) && (mMotor))
        {
            val |= 0x20; // set Ready
        }
        if (mActiveDrive->currentTrack() == 0)
        {                // drive head is over track 0?
            val |= 0x10; // set Track 0
        }
        mResult[RES_ST0] = val;
        mPhase = RESULT_PHASE; // switch to result mPhase
    }

    void Fdc::recalib()
    {
        mCommand[CMD_C] = 0; // seek to track 0
        seek();
    }

    void Fdc::intstat()
    {
        tUBYTE val;

        val = mResult[RES_ST0] & 0xf8; // clear Head Address and Unit bits
        if (mFlags & SEEKDRVA_flag)
        {                // seek completed on drive A?
            val |= 0x20; // set Seek End
            mFlags &=
                ~(SEEKDRVA_flag |
                  STATUSDRVA_flag); // clear seek done and status change mFlags
            mResult[RES_ST0] = val;
            mResult[RES_ST1] = mDriveA.currentTrack();
        }
        else if (mFlags & SEEKDRVB_flag)
        {                // seek completed on drive B?
            val |= 0x21; // set Seek End
            mFlags &=
                ~(SEEKDRVB_flag |
                  STATUSDRVB_flag); // clear seek done and status change mFlags
            mResult[RES_ST0] = val;
            mResult[RES_ST1] = mDriveB.currentTrack();
        }
        else if (mFlags & STATUSDRVA_flag)
        {               // has the status of drive A changed?
            val = 0xc0; // status change
            if ((mDriveA.tracks() == 0) || (!mMotor))
            {                // no DSK in the drive, or drive mMotor is turned off?
                val |= 0x08; // not ready
            }
            mFlags &= ~STATUSDRVA_flag; // clear status change flag
            mResult[RES_ST0] = val;
            mResult[RES_ST1] = mDriveA.currentTrack();
        }
        else if (mFlags & STATUSDRVB_flag)
        {               // has the status of drive B changed?
            val = 0xc1; // status change
            if ((mDriveB.tracks() == 0) || (!mMotor))
            {                // no DSK in the drive, or drive mMotor is turned off?
                val |= 0x08; // not ready
            }
            mFlags &= ~STATUSDRVB_flag; // clear status change flag
            mResult[RES_ST0] = val;
            mResult[RES_ST1] = mDriveB.currentTrack();
        }
        else
        {
            val = 0x80; // Invalid Command
            mResult[RES_ST0] = val;
            mResLength = 1;
        }
        mPhase = RESULT_PHASE; // switch to result mPhase
    }

    void Fdc::seek()
    {
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentTrack(mCommand[CMD_C]);
            if (mActiveDrive->currentTrack() >= DSK_TRACKMAX)
            { // beyond valid range?
                mActiveDrive->setCurrentTrack(DSK_TRACKMAX - 1); // limit to maximum
            }
        }
        mFlags |= (mCommand[CMD_UNIT] & 1)
                      ? SEEKDRVB_flag
                      : SEEKDRVA_flag; // signal completion of seek operation
        mPhase = CMD_PHASE; // switch back to mCommand mPhase (Fdc::seek has no
                            // result mPhase!)
    }

    void Fdc::readtrk()
    {
        mLed = true;  // turn the drive LED on
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentSide((mCommand[CMD_UNIT] & 4) >>
                                         2); // extract target side
            tUWORD side = mActiveDrive->sides()
                             ? mActiveDrive->currentSide()
                             : 0; // single sided drives only acccess side 1
            if ((mActiveDrive->flipped()))
            { // did the user request to access the "other" side?
                side = side ? 0 : 1; // reverse the side to access
            }
            mActiveTrack = &mActiveDrive->track(mActiveDrive->currentTrack(), side);
            if (mActiveTrack->sectors() != 0)
            {                                      // track is formatted?
                mCommand[CMD_R] = 1;               // set sector ID to 1
                mActiveDrive->setCurrentSector(0); // reset sector table index

                cmd_readtrk();
            }
            else
            {                             // unformatted track
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x01; // Missing AM

                LOAD_RESULT_WITH_CHRN

                mPhase = RESULT_PHASE; // switch to result mPhase
            }
        }
        else
        { // drive was not ready
            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    void Fdc::write()
    {
        mLed = true;  // turn the drive LED on
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentSide((mCommand[CMD_UNIT] & 4) >>
                                         2); // extract target side
            tUWORD side = mActiveDrive->sides()
                             ? mActiveDrive->currentSide()
                             : 0; // single sided drives only acccess side 1
            if ((mActiveDrive->flipped()))
            { // did the user request to access the "other" side?
                side = side ? 0 : 1; // reverse the side to access
            }
            mActiveTrack = &mActiveDrive->track(mActiveDrive->currentTrack(), side);
            if (mActiveDrive->writeProtected())
            {                             // is write protect tab set?
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x02; // Not Writable

                LOAD_RESULT_WITH_CHRN

                mPhase = RESULT_PHASE; // switch to result mPhase
            }
            else if (mActiveTrack->sectors() != 0)
            { // track is formatted?
                cmd_write();
            }
            else
            {                             // unformatted track
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x01; // Missing AM

                LOAD_RESULT_WITH_CHRN

                mPhase = RESULT_PHASE; // switch to result mPhase
            }
        }
        else
        { // drive was not ready
            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    void Fdc::read()
    {
        mLed = true;  // turn the drive LED on
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentSide((mCommand[CMD_UNIT] & 4) >>
                                         2); // extract target side
            tUWORD side = mActiveDrive->sides()
                             ? mActiveDrive->currentSide()
                             : 0; // single sided drives only acccess side 1
            if ((mActiveDrive->flipped()))
            { // did the user request to access the "other" side?
                side = side ? 0 : 1; // reverse the side to access
            }
            mActiveTrack = &mActiveDrive->track(mActiveDrive->currentTrack(), side);
            if (mActiveTrack->sectors() != 0)
            { // track is formatted?
                cmd_read();
            }
            else
            {                             // unformatted track
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x01; // Missing AM

                LOAD_RESULT_WITH_CHRN

                mPhase = RESULT_PHASE; // switch to result mPhase
            }
        }
        else
        { // drive was not ready
            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    void Fdc::readID()
    {
        mLed = true;  // turn the drive LED on
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentSide((mCommand[CMD_UNIT] & 4) >>
                                         2); // extract target side
            tUWORD side = mActiveDrive->sides()
                             ? mActiveDrive->currentSide()
                             : 0; // single sided drives only acccess side 1
            if ((mActiveDrive->flipped()))
            { // did the user request to access the "other" side?
                side = side ? 0 : 1; // reverse the side to access
            }
            mActiveTrack = &mActiveDrive->track(mActiveDrive->currentTrack(), side);
            if (mActiveTrack->sectors() != 0)
            { // track is formatted?
                tUWORD idx;

                idx = mActiveDrive->currentSector(); // get the active sector index
                if (idx >= mActiveTrack->sectors())
                {            // index beyond number of sectors for this track?
                    idx = 0; // reset index
                }
                memcpy(&mResult[RES_C], mActiveTrack->sector(idx).CHRN(),
                       4); // copy sector's CHRN to result buffer
                mActiveDrive->setCurrentSector(
                    idx + 1); // update sector table index for active drive
            }
            else
            {                             // unformatted track
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x01; // Missing AM

                LOAD_RESULT_WITH_CHRN
            }
        }
        mPhase = RESULT_PHASE; // switch to result mPhase
    }

    void Fdc::writeID()
    {
        mLed = true;  // turn the drive LED on
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentSide((mCommand[CMD_UNIT] & 4) >>
                                         2); // extract target side
            tUWORD side = mActiveDrive->sides()
                             ? mActiveDrive->currentSide()
                             : 0; // single sided drives only acccess side 1
            if ((mActiveDrive->flipped()))
            { // did the user request to access the "other" side?
                side = side ? 0 : 1; // reverse the side to access
            }
            mActiveTrack = &mActiveDrive->track(mActiveDrive->currentTrack(), side);
            if (mActiveDrive->writeProtected())
            {                             // is write protect tab set?
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x02; // Not Writable

                LOAD_RESULT_WITH_CHRN

                mPhase = RESULT_PHASE; // switch to result mPhase
            }
            else
            {
                mBufferCount = mCommand[CMD_H] << 2; // number of sectors * 4 =
                                                     // number of UBYTEs still
                                                     // outstanding
                mBufferPtr =
                    pbGPBuffer; // buffer to temporarily hold the track format
                mBufferEndPtr = pbGPBuffer + mBufferCount;
                mTimeout = INITIAL_TIMEOUT;
                mReadStatusDelay = 1;
            }
        }
        else
        { // drive was not ready
            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    void Fdc::scan()
    {
        mLed = true;  // turn the drive LED on
        check_unit(); // switch to target drive
        if (init_status_regs() == 0)
        { // drive Ready?
            mActiveDrive->setCurrentSide((mCommand[CMD_UNIT] & 4) >>
                                         2); // extract target side
            tUWORD side = mActiveDrive->sides()
                             ? mActiveDrive->currentSide()
                             : 0; // single sided drives only acccess side 1
            if ((mActiveDrive->flipped()))
            { // did the user request to access the "other" side?
                side = side ? 0 : 1; // reverse the side to access
            }
            mActiveTrack = &mActiveDrive->track(mActiveDrive->currentTrack(), side);
            if (mActiveTrack->sectors() != 0)
            { // track is formatted?
                if (mCommand[CMD_STP] > 2)
                {
                    mCommand[CMD_STP] = 2; // step can only be 1 or 2
                }
                mFlags |= SCAN_flag; // scan mCommand active
                cmd_scan();
            }
            else
            {                             // unformatted track
                mResult[RES_ST0] |= 0x40; // AT
                mResult[RES_ST1] |= 0x01; // Missing AM

                LOAD_RESULT_WITH_CHRN

                mPhase = RESULT_PHASE; // switch to result mPhase
            }
        }
        else
        { // drive was not ready
            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    void Fdc::check_unit()
    {
        switch (mCommand[CMD_UNIT] &
                1) // check unit selection bits of active command
        {
            case 0: // target for command is drive A
                mActiveDrive = &mDriveA;
                break;
            case 1: // target for command is drive B
                mActiveDrive = &mDriveB;
                break;
        }
    }

    int Fdc::init_status_regs()
    {
        tUBYTE val;

        memset(&mResult, 0, sizeof(mResult)); // clear result codes buffer
        val = mCommand[CMD_UNIT] & 7;         // keep head and unit of command
        if ((mActiveDrive->tracks() == 0) || (!mMotor))
        {                // no DSK in the drive, or drive mMotor is turned off?
            val |= 0x48; // Abnormal Termination + Not Ready
        }
        mResult[RES_ST0] = val; // write ST0 to result codes buffer
        return (val &
                8); // return value indicates whether drive is ready (0) or not (8)
    }

    Sector *Fdc::find_sector(tUBYTE *requested_CHRN)
    {
        int loop_count;
        tUWORD idx;
        Sector *sector;

        sector = NULL;  // return value indicates 'sector not found' by default
        loop_count = 0; // detection of index hole counter
        idx = mActiveDrive->currentSector(); // get the active sector index
        do
        {
            if (!(memcmp(mActiveTrack->sector(idx).CHRN(), requested_CHRN, 4)))
            { // sector matches requested ID?
                sector = &mActiveTrack->sector(
                    idx); // return value points to sector information
                if ((sector->flag(0) & 0x20) || (sector->flag(1) & 0x20))
                { // any Data Errors?
                    if (mActiveDrive->randomDEs())
                    { // simulate 'random' DEs?
                        mFlags |= RNDDE_flag;
                    }
                }
                mResult[RES_ST2] &=
                    ~(0x02 |
                      0x10); // remove possible Bad Cylinder + No Cylinder flags
                break;
            }
            tUBYTE cylinder = mActiveTrack->sector(idx).CHRN(0); // extract C
            if (cylinder == 0xff)
            {
                mResult[RES_ST2] |= 0x02; // Bad Cylinder
            }
            else if (cylinder != mCommand[CMD_C])
            {                             // does not match requested C?
                mResult[RES_ST2] |= 0x10; // No Cylinder
            }
            idx++; // increase sector table index
            if (idx >= mActiveTrack->sectors())
            {                 // index beyond number of sectors for this track?
                idx = 0;      // reset index
                loop_count++; // increase 'index hole' count
            }
        } while (loop_count <
                 2); // loop until sector is found, or index hole has passed twice
        if (mResult[RES_ST2] & 0x02)
        {                              // Bad Cylinder set?
            mResult[RES_ST2] &= ~0x10; // remove possible No Cylinder flag
        }
        mActiveDrive->setCurrentSector(
            idx); // update sector table index for active drive
        return sector;
    }

    inline void Fdc::cmd_write()
    {
        Sector *sector;

        sector = find_sector(
            &mCommand[CMD_C]); // locate the requested sector on the current track
        if (sector)
        { // sector found
            int sector_size;

            sector->setFlag(0, 0); // clear ST1 for this sector
            if (mCommand[CMD_CODE] == 0x45)
            {                          // write data command?
                sector->setFlag(1, 0); // clear ST2
            }
            else
            {                             // write deleted data
                sector->setFlag(1, 0x40); // set Control Mark
            }

            if (mCommand[CMD_N] == 0)
            { // use DTL for length?
                sector_size =
                    mCommand[CMD_DTL]; // size of sector is defined by DTL value
                if (sector_size > 0x80)
                {
                    sector_size = 0x80; // max DTL value is 128
                }
            }
            else
            {
                sector_size =
                    128
                    << mCommand[CMD_N]; // determine number of UBYTEs from N value
            }
            mBufferCount = sector_size;  // init number of UBYTEs to transfer
            mBufferPtr = sector->data(); // pointer to sector data
            mBufferEndPtr =
                mActiveTrack->data() +
                mActiveTrack->size(); // pointer beyond end of track data
            mTimeout = INITIAL_TIMEOUT;
            mReadStatusDelay = 1;
        }
        else
        {                             // sector not found
            mResult[RES_ST0] |= 0x40; // AT
            mResult[RES_ST1] |= 0x04; // No Data

            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    inline void Fdc::cmd_read()
    {
        Sector *sector;

    loop:
        sector = find_sector(
            &mCommand[CMD_C]); // locate the requested sector on the current track
        if (sector)
        { // sector found
            mResult[RES_ST1] =
                sector->flag(0) & 0x25; // copy ST1 to result, ignoring unused bits
            mResult[RES_ST2] =
                sector->flag(1) & 0x61; // copy ST2 to result, ignoring unused bits
            if (mCommand[CMD_CODE] == 0x4c)
            {                             // read deleted data command?
                mResult[RES_ST2] ^= 0x40; // invert Control Mark
            }
            if ((mFlags & SKIP_flag) && (mResult[RES_ST2] &= 0x40))
            { // skip sector?
                if (mCommand[CMD_R] != mCommand[CMD_EOT])
                {                      // continue looking?
                    mCommand[CMD_R]++; // advance to next sector
                    goto loop;
                }
                else
                { // no data to transfer -> no execution mPhase
                    LOAD_RESULT_WITH_STATUS

                    LOAD_RESULT_WITH_CHRN

                    mPhase = RESULT_PHASE; // switch to result mPhase
                }
            }
            else
            { // sector data is to be transferred
                int sector_size;

                if (mResult[RES_ST2] &= 0x40)
                { // does the sector have an AM opposite of what we want?
                    mCommand[CMD_EOT] =
                        mCommand[CMD_R]; // execution ends on this sector
                }
                if (mCommand[CMD_N] == 0)
                { // use DTL for length?
                    sector_size =
                        mCommand[CMD_DTL]; // size of sector is defined by DTL value
                    if (sector_size > 0x80)
                    {
                        sector_size = 0x80; // max DTL value is 128
                    }
                }
                else
                {
                    sector_size = 128 << mCommand[CMD_N]; // determine number of
                                                          // UBYTEs from N value
                }
                mBufferCount = sector_size;  // init number of UBYTEs to transfer
                mBufferPtr = sector->data(); // pointer to sector data
                mBufferEndPtr =
                    mActiveTrack->data() +
                    mActiveTrack->size(); // pointer beyond end of track data
                mTimeout = INITIAL_TIMEOUT;
                mReadStatusDelay = 1;
            }
        }
        else
        {                             // sector not found
            mResult[RES_ST0] |= 0x40; // AT
            mResult[RES_ST1] |= 0x04; // No Data

            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    inline void Fdc::cmd_readtrk()
    {
        int sector_size;
        Sector *sector;

        sector = &mActiveTrack->sector(mActiveDrive->currentSector());
        if (memcmp(sector->CHRN(), &mCommand[CMD_C], 4))
        {                             // sector does not match requested ID?
            mResult[RES_ST1] |= 0x04; // No Data
        }
        mResult[RES_ST2] &= 0xbf; // clear Control Mark, if it was set
        mResult[RES_ST1] |=
            sector->flag(0) & 0x25; // copy ST1 to result, ignoring unused bits
        mResult[RES_ST2] |=
            sector->flag(1) & 0x61; // copy ST2 to result, ignoring unused bits
        if (mCommand[CMD_N] == 0)
        { // use DTL for length?
            sector_size =
                mCommand[CMD_DTL]; // size of sector is defined by DTL value
            if (sector_size > 0x80)
            {
                sector_size = 0x80; // max DTL value is 128
            }
        }
        else
        {
            sector_size =
                128 << mCommand[CMD_N]; // determine number of UBYTEs from N value
        }
        mBufferCount = sector_size;  // init number of UBYTEs to transfer
        mBufferPtr = sector->data(); // pointer to sector data
        mBufferEndPtr = mActiveTrack->data() +
                        mActiveTrack->size(); // pointer beyond end of track data
        mTimeout = INITIAL_TIMEOUT;
        mReadStatusDelay = 1;
    }

    inline void Fdc::cmd_scan()
    {
        Sector *sector;

    loop:
        sector = find_sector(
            &mCommand[CMD_C]); // locate the requested sector on the current track
        if (sector)
        { // sector found
            mResult[RES_ST1] =
                sector->flag(0) & 0x25; // copy ST1 to result, ignoring unused bits
            mResult[RES_ST2] =
                sector->flag(1) & 0x61; // copy ST2 to result, ignoring unused bits
            if ((mFlags & SKIP_flag) && (mResult[RES_ST2] &= 0x40))
            { // skip sector?
                if (mCommand[CMD_R] != mCommand[CMD_EOT])
                {                                         // continue looking?
                    mCommand[CMD_R] += mCommand[CMD_STP]; // advance to next sector
                    goto loop;
                }
                else
                { // no data to transfer -> no execution mPhase
                    LOAD_RESULT_WITH_STATUS

                    LOAD_RESULT_WITH_CHRN

                    mPhase = RESULT_PHASE; // switch to result mPhase
                }
            }
            else
            { // sector data is to be transferred
                int sector_size;

                if (mResult[RES_ST2] &= 0x40)
                { // does the sector have an AM opposite of what we want?
                    mCommand[CMD_EOT] =
                        mCommand[CMD_R]; // execution ends on this sector
                }
                sector_size =
                    128
                    << mCommand[CMD_N]; // determine number of UBYTEs from N value
                mBufferCount = sector_size;  // init number of UBYTEs to transfer
                mBufferPtr = sector->data(); // pointer to sector data
                mBufferEndPtr =
                    mActiveTrack->data() +
                    mActiveTrack->size();   // pointer beyond end of track data
                mFlags &= ~SCANFAILED_flag; // reset scan faimLed flag
                mResult[RES_ST2] |= 0x08; // assume data matches: set Scan Equal Hit
                mTimeout = INITIAL_TIMEOUT;
                mReadStatusDelay = 1;
            }
        }
        else
        {                             // sector not found
            mResult[RES_ST0] |= 0x40; // AT
            mResult[RES_ST1] |= 0x04; // No Data

            LOAD_RESULT_WITH_CHRN

            mPhase = RESULT_PHASE; // switch to result mPhase
        }
    }

    int Fdc::dsk_load(const char *pchFileName, int drv, char chID)
    {
        Drive *drive = &mDriveA;

        if (drv == 0) drive = &mDriveA;
        if (drv == 1) drive = &mDriveB;

        int iRetCode;
        tUWORD dwTrackSize, track, side, sector, dwSectorSize, dwSectors;
        tUBYTE *pbPtr, *pbDataPtr, *pbTempPtr, *pbTrackSizeTable;

        iRetCode = 0;
        dsk_eject(drv);
        if ((pfileObject = fopen(pchFileName, "rb")) != NULL)
        {
            fread(pbGPBuffer, 0x100, 1, pfileObject); // read DSK header
            pbPtr = pbGPBuffer;

            if (memcmp(pbPtr, "MV - CPC", 8) == 0)
            {                                      // normal DSK image?
                drive->setTracks(*(pbPtr + 0x30)); // grab number of tracks
                if (drive->tracks() > DSK_TRACKMAX)
                {                                   // compare against upper limit
                    drive->setTracks(DSK_TRACKMAX); // limit to maximum
                }
                drive->setSides(*(pbPtr + 0x31)); // grab number of sides
                if (drive->sides() > DSK_SIDEMAX)
                { // abort if more than maximum
                    iRetCode = ERR_DSK_SIDES;
                    goto exit;
                }
                dwTrackSize =
                    (*(pbPtr + 0x32) + (*(pbPtr + 0x33) << 8)) -
                    0x100; // determine track size in UBYTEs, minus track header
                drive->setSides(drive->sides() - 1); // zero base number of sides
                for (track = 0; track < drive->tracks(); track++)
                { // loop for all tracks
                    for (side = 0; side <= drive->sides(); side++)
                    { // loop for all sides
                        fread(pbGPBuffer + 0x100, 0x100, 1,
                              pfileObject); // read track header
                        pbPtr = pbGPBuffer + 0x100;
                        if (memcmp(pbPtr, "Track-Info", 10) != 0)
                        { // abort if ID does not match
                            iRetCode = ERR_DSK_INVALID;
                            goto exit;
                        }
                        dwSectorSize =
                            0x80
                            << *(pbPtr + 0x14); // determine sector size in UBYTEs
                        dwSectors = *(pbPtr + 0x15); // grab number of sectors
                        if (dwSectors > DSK_SECTORMAX)
                        { // abort if sector count greater than maximum
                            iRetCode = ERR_DSK_SECTORS;
                            goto exit;
                        }
                        drive->track(track, side)
                            .setSectors(dwSectors); // store sector count
                        drive->track(track, side)
                            .setSize(dwTrackSize); // store track size
                        drive->track(track, side)
                            .setData(new tUBYTE[dwTrackSize]); // attempt to allocate
                                                              // the required memory
                        if (drive->track(track, side).data() == NULL)
                        { // abort if not enough
                            iRetCode = ERR_OUT_OF_MEMORY;
                            goto exit;
                        }
                        pbDataPtr =
                            drive->track(track, side)
                                .data();       // pointer to start of memory buffer
                        pbTempPtr = pbDataPtr; // keep a pointer to the beginning of
                                               // the buffer for the current track
                        for (sector = 0; sector < dwSectors; sector++)
                        { // loop for all sectors
                            memcpy(drive->track(track, side).sector(sector).CHRN(),
                                   (pbPtr + 0x18), 4); // copy CHRN
                            memcpy(drive->track(track, side).sector(sector).flags(),
                                   (pbPtr + 0x1c), 2); // copy ST1 & ST2
                            drive->track(track, side)
                                .sector(sector)
                                .setSize(dwSectorSize);
                            drive->track(track, side)
                                .sector(sector)
                                .setData(pbDataPtr); // store pointer to sector data
                            pbDataPtr += dwSectorSize;
                            pbPtr += 8;
                        }
                        if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject))
                        { // read entire track data in one go
                            iRetCode = ERR_DSK_INVALID;
                            goto exit;
                        }
                    }
                }
                drive->setAltered(0); // disk is as yet unmodified
            }
            else
            {
                if (memcmp(pbPtr, "EXTENDED", 8) == 0)
                {                                      // extended DSK image?
                    drive->setTracks(*(pbPtr + 0x30)); // number of tracks
                    if (drive->tracks() > DSK_TRACKMAX)
                    { // limit to maximum possible
                        drive->setTracks(DSK_TRACKMAX);
                    }
                    drive->setRandomDEs(*(pbPtr + 0x31) &
                                        0x80); // simulate random Data Errors?
                    drive->setSides(*(pbPtr + 0x31) & 3); // number of sides
                    if (drive->sides() > DSK_SIDEMAX)
                    { // abort if more than maximum
                        iRetCode = ERR_DSK_SIDES;
                        goto exit;
                    }
                    pbTrackSizeTable =
                        pbPtr + 0x34; // pointer to track size table in DSK header
                    drive->setSides(drive->sides() -
                                    1); // zero base number of sides
                    for (track = 0; track < drive->tracks(); track++)
                    { // loop for all tracks
                        for (side = 0; side <= drive->sides(); side++)
                        { // loop for all sides
                            dwTrackSize =
                                (*pbTrackSizeTable++ << 8); // track size in UBYTEs
                            if (dwTrackSize != 0)
                            { // only process if track contains data
                                dwTrackSize -= 0x100; // compensate for track header
                                fread(pbGPBuffer + 0x100, 0x100, 1,
                                      pfileObject); // read track header
                                pbPtr = pbGPBuffer + 0x100;
                                if (memcmp(pbPtr, "Track-Info", 10) != 0)
                                { // valid track header?
                                    iRetCode = ERR_DSK_INVALID;
                                    goto exit;
                                }
                                dwSectors =
                                    *(pbPtr +
                                      0x15); // number of sectors for this track
                                if (dwSectors > DSK_SECTORMAX)
                                { // abort if sector count greater than maximum
                                    iRetCode = ERR_DSK_SECTORS;
                                    goto exit;
                                }
                                drive->track(track, side)
                                    .setSectors(dwSectors); // store sector count
                                drive->track(track, side)
                                    .setSize(dwTrackSize); // store track size
                                drive->track(track, side)
                                    .setData(new tUBYTE[dwTrackSize]); // attempt to
                                                                      // allocate
                                                                      // the
                                                                      // required
                                                                      // memory
                                if (drive->track(track, side).data() == NULL)
                                { // abort if not enough
                                    iRetCode = ERR_OUT_OF_MEMORY;
                                    goto exit;
                                }
                                pbDataPtr = drive->track(track, side)
                                                .data(); // pointer to start of
                                                         // memory buffer
                                pbTempPtr = pbDataPtr;   // keep a pointer to the
                                                         // beginning of the buffer
                                                         // for the current track
                                for (sector = 0; sector < dwSectors; sector++)
                                { // loop for all sectors
                                    memcpy(drive->track(track, side)
                                               .sector(sector)
                                               .CHRN(),
                                           (pbPtr + 0x18), 4); // copy CHRN
                                    memcpy(drive->track(track, side)
                                               .sector(sector)
                                               .flags(),
                                           (pbPtr + 0x1c), 2); // copy ST1 & ST2
                                    dwSectorSize = *(pbPtr + 0x1e) +
                                                   (*(pbPtr + 0x1f)
                                                    << 8); // sector size in UBYTEs
                                    drive->track(track, side)
                                        .sector(sector)
                                        .setSize(dwSectorSize);
                                    drive->track(track, side)
                                        .sector(sector)
                                        .setData(pbDataPtr); // store pointer to
                                                             // sector data
                                    pbDataPtr += dwSectorSize;
                                    pbPtr += 8;
                                }
                                if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject))
                                { // read entire track data in one go
                                    iRetCode = ERR_DSK_INVALID;
                                    goto exit;
                                }
                            }
                            else
                            {
                                drive->track(track, side)
                                    .set(0, dwTrackSize); // track not formatted
                            }
                        }
                    }
                    drive->setAltered(0); // disk is as yet unmodified
                }
                else
                {
                    iRetCode = ERR_DSK_INVALID; // file could not be identified as a
                                                // valid DSK
                }
            }
        exit:
            fclose(pfileObject);
        }
        else
        {
            iRetCode = ERR_FILE_NOT_FOUND;
        }

        if (iRetCode != 0)
        { // on error, 'eject' disk from drive
            dsk_eject(drv);
        }
        return iRetCode;
    }

    void Fdc::dsk_eject(int drv)
    {
        tUWORD track, side;
        Drive *drive = &mDriveA; // default

        if (drv == 0) drive = &mDriveA;
        if (drv == 1) drive = &mDriveB;

        for (track = 0; track < DSK_TRACKMAX; track++)
        { // loop for all tracks
            for (side = 0; side < DSK_SIDEMAX; side++)
            { // loop for all sides
                if (drive->track(track, side).data())
                { // track is formatted?
                    // free(drive->track(track, side).data()); // release memory
                    // allocated for this track
                    drive->track(track, side).clear();
                    /** @todo check memory hole !!! */
                }
            }
        }
        tUWORD dwTemp = drive->currentTrack(); // save the drive head position
        drive->init();                        // clear drive info structure
        drive->setCurrentTrack(dwTemp);
    }
} // namespace cpcx

/*
int dsk_save (char *pchFileName, t_drive *drive, char chID)
{
   t_DSK_header dh;
   t_track_header th;
   dword track, side, pos, sector;

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      memset(&dh, 0, sizeof(dh));
      strcpy(dh.id, "EXTENDED CPC DSK File\r\nDisk-Info\r\n");
      strcpy(dh.unused1, "Caprice32\r\n");
      dh.tracks = drive->tracks;
      dh.sides = (drive->sides+1) | (drive->random_DEs); // correct side count
and indicate random DEs, if necessary
      pos = 0;
      for (track = 0; track < drive->tracks; track++) { // loop for all tracks
         for (side = 0; side <= drive->sides; side++) { // loop for all sides
            if (drive->track[track][side].size) { // track is formatted?
               dh.track_size[pos] = (drive->track[track][side].size + 0x100) >>
8; // track size + header in bytes
            }
            pos++;
         }
      }
      if (!fwrite(&dh, sizeof(dh), 1, pfileObject)) { // write header to file
         fclose(pfileObject);
         return ERR_DSK_WRITE;
      }

      memset(&th, 0, sizeof(th));
      strcpy(th.id, "Track-Info\r\n");
      for (track = 0; track < drive->tracks; track++) { // loop for all tracks
         for (side = 0; side <= drive->sides; side++) { // loop for all sides
            if (drive->track[track][side].size) { // track is formatted?
               th.track = track;
               th.side = side;
               th.bps = 2;
               th.sectors = drive->track[track][side].sectors;
               th.gap3 = 0x4e;
               th.filler = 0xe5;
               for (sector = 0; sector < th.sectors; sector++) {
                  memcpy(&th.sector[sector][0],
drive->track[track][side].sector[sector].CHRN, 4); // copy CHRN
                  memcpy(&th.sector[sector][4],
drive->track[track][side].sector[sector].flags, 2); // copy ST1 & ST2
                  th.sector[sector][6] =
drive->track[track][side].sector[sector].size & 0xff;
                  th.sector[sector][7] =
(drive->track[track][side].sector[sector].size >> 8) & 0xff; // sector size in
bytes
               }
               if (!fwrite(&th, sizeof(th), 1, pfileObject)) { // write track
header
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
               if (!fwrite(drive->track[track][side].data,
drive->track[track][side].size, 1, pfileObject)) { // write track data
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
            }
         }
      }
      fclose(pfileObject);
   } else {
      return ERR_DSK_WRITE; // write attempt failed
   }

//    char *pchTmpBuffer = new char[MAX_LINE_LEN];
//    LoadString(hAppInstance, MSG_DSK_SAVE, chMsgBuffer, sizeof(chMsgBuffer));
//    snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, chID, chID == 'A' ?
CPC.drvA_file : CPC.drvB_file);
//    add_message(pchTmpBuffer);
//    delete [] pchTmpBuffer;
//    return 0;
}



int dsk_format (t_drive *drive, int iFormat)
{
   int iRetCode = 0;
   drive->tracks = disk_format[iFormat].tracks;
   if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
      drive->tracks = DSK_TRACKMAX; // limit to maximum
   }
   drive->sides = disk_format[iFormat].sides;
   if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
      iRetCode = ERR_DSK_SIDES;
      goto exit;
   }
   drive->sides--; // zero base number of sides
   for (dword track = 0; track < drive->tracks; track++) { // loop for all
tracks
      for (dword side = 0; side <= drive->sides; side++) { // loop for all sides
         dword dwSectorSize = 0x80 << disk_format[iFormat].sector_size; //
determine sector size in bytes
         dword dwSectors = disk_format[iFormat].sectors;
         if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than
maximum
            iRetCode = ERR_DSK_SECTORS;
            goto exit;
         }
         dword dwTrackSize = dwSectorSize * dwSectors; // determine track size
in bytes, minus track header
         drive->track[track][side].sectors = dwSectors; // store sector count
         drive->track[track][side].size = dwTrackSize; // store track size
         drive->track[track][side].data = (byte *)malloc(dwTrackSize); //
attempt to allocate the required memory
         if (drive->track[track][side].data == NULL) { // abort if not enough
            iRetCode = ERR_OUT_OF_MEMORY;
            goto exit;
         }
         byte *pbDataPtr = drive->track[track][side].data; // pointer to start
of memory buffer
         byte *pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the
buffer for the current track
         byte CHRN[4];
         CHRN[0] = (byte)track;
         CHRN[1] = (byte)side;
         CHRN[3] = (byte)disk_format[iFormat].sector_size;
         for (dword sector = 0; sector < dwSectors; sector++) { // loop for all
sectors
            CHRN[2] = disk_format[iFormat].sector_ids[side][sector];
            memcpy(drive->track[track][side].sector[sector].CHRN, CHRN, 4); //
copy CHRN
            drive->track[track][side].sector[sector].size = dwSectorSize;
            drive->track[track][side].sector[sector].data = pbDataPtr; // store
pointer to sector data
            pbDataPtr += dwSectorSize;
         }
         memset(pbTempPtr, disk_format[iFormat].filler_byte, dwTrackSize);
      }
   }
   drive->altered = 1; // flag disk as having been modified

exit:
   if (iRetCode != 0) { // on error, 'eject' disk from drive
      dsk_eject(drive);
   }
   return iRetCode;
}
*/
