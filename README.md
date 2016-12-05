# [Roland CPC Emulator&nbsp;&nbsp;<br><br>![](https://github.com/raldus/roland/blob/master/assets/img/ready.png)](https://www.rolandemu.de/index.html)

## Overview
Roland is an Emulator of the homecomputers Amstrad/Schneider from the series CPC 464/664/6128
for Linux und Windows.
Roland depends on SDL > 1.2 and SDL-Image.
For further information visit [rolandemu.de](https://www.rolandemu.de/index.html)

## Build
The source is C++11 code and needs at leat a c+11 enabled Compiler. Supported are GCC native or CygWin/MinGw. The build system

### Status
[![Build Status](https://travis-ci.org/raldus/roland.svg?branch=master)](https://travis-ci.org/raldus/roland) Ubuntu32/64 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![Build status](https://ci.appveyor.com/api/projects/status/rsa9fq8brffd5h50?svg=true)](https://ci.appveyor.com/project/raldus/roland) Windows32

### General
You will need cmake and git installed.
```bash
git clone https://github.com/raldus/roland.git
cd roland
```

### Linux
Folowing an Example of a unix release build.
```bash
cmake -H. -Bbuild -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
### Linux rpm package
```bash
cmake --build build --target package
```

### Windows
At the Moment Visual Studio 2015 32bit and MinGW are the only valid build environments.
Following an Example with Visual Studio release build:
```bash
cmake -H. -Bbuild -G"Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Documentation
You will need Doxygen installed.
```bash
cmake --build build --target doc
```

## Usage
Taste|Funktion
-----|--------
F2|insert disk in drive A:
F3|insert disk in drive B:
F4|execute cat/run (buggy)
F5|enable/disable joystickemulation (cursorkeys + alt)
F9|reset
F10|exit
F12|switch to fullscreen (does not work in Windows; instead set fullscreen=1 in roland.cfg)

### Usage of the CPC 464/664/6128
Insert a disk, enter **cat**  **Enter** and
you can see the directory. Enter **run "filename"**  **Enter** (mostly *.bas).

### Example

1. press **F2**
2. search a disk with **arrowkeys** (HarveyH.dsk) **Enter**
3. type **cat**  **Enter**
4. in this case the file is named **Harvey**
5. type **run "harvey"**  **Enter**
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

### Linux
Before you can start the Linux version, you have to adjust some pathes in
**roland-linux.cfg**.
```text
cpcrom=/mnt/shared/emu/cpc/roms/cpc6128.rom -> /../ThisFolder/roms/cpc6128.rom
amsdos=/mnt/shared/emu/cpc/roms/amsdos.rom  -> /../ThisFolder/roms/amsdos.rom
diskdir=/mnt/shared/emu/cpc/dsk             -> /../ThisFolder/disks
```
Afterwards, put **roland-linux.cfg** renamed to **.roland** into your homedir.

## Misc
[Changelog](https://github.com/raldus/roland/blob/master/CHANGELOG.md)
