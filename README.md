# Roland CPC Emulator
Roland is an Emulator of the homecomputers Amstrad/Schneider from the series CPC 464/664/6128
for Linux und Windows.

Roland needs SDL > 1.2 and SDL-Image.

## Usage of the Roland CPC Emulator
Taste|Funktion
-----|--------
F2|insert disk in drive A:
F3|insert disk in drive B:
F4|execute cat/run (buggy)
F5|enable/disable joystickemulation (cursorkeys + alt)
F9|reset
F10|exit
F12|switch to fullscreen (does not work in Windows; instead set fullscreen=1 in roland.cfg)

## Usage of the CPC 464/664/6128
Insert a disk, enter **cat**  **Enter** and
you can see the directory. Enter **run "filename"**  **Enter** (mostly *.bas).

### Example

1. press **F2**
2. search a disk with **arrowkeys** (HarveyH.dsk) **Enter**
3. enter **cat**
4. in this case the file is named **Harvey**
5. enter **run "harvey** (closing **"** is not necessary) **Enter**
6. press **F5** to emulate a joystick with **arrowkeys** and **ALT**

### Example 2 (Bjack1.dsk)

1. ...
2. ...
3. ...
4. in this case the file is named **Bomb.bin**
5. enter **run "bomb**  **Enter**

In most cases, there is a *.bas file on the disk
to start the appropriate software. e.g.: disc.bas

## Configuration
Change settings in file roland.cfg inside the installation directory
(Windows) or in file ~/.roland (Linux).

working settings:
* showfps    (true/false) framerate
* fullscreen (true/false) window/fullscreen
* monitor    (0=color, 1=green) color-/greenscreen
* border     (true/false) border on/off, does not change display size (for now)
* intensity  (1-20) brightness
* doublescan (true/false) the real cpc had scanlines ...
* jumpers    (DIP-switch of the CPC - for experts)

## Linux
Before you can start the Linux version, you have to adjust some pathes in
**roland-linux.cfg**.
```text
cpcrom=/mnt/shared/emu/cpc/roms/cpc6128.rom -> /../dieserOrdner/roms/cpc6128.rom
amsdos=/mnt/shared/emu/cpc/roms/amsdos.rom  -> /../dieserOrdner/roms/amsdos.rom
diskdir=/mnt/shared/emu/cpc/dsk             -> /../dieserOrdner/disks
```
After that, put **roland-linux.cfg** renamed to **.roland** in your homedir.
