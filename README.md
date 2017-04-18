# Roland CPC Emulator

## Overview
Roland is an emulator of the homecomputers Amstrad/Schneider from the series CPC 464/664/6128
for Linux und Windows.
Roland depends on SDL > 1.2 and SDL-Image.
For further information visit https://www.rolandemu.de/

## Build
The source is written in c++11 code and needs at leat a c++11 enabled Compiler. Supported are GCC native, Visual Studio 2015 or CygWin/MinGw. 

### Status
OS|Detail|Status
---|---|--:
Ubuntu|32 & 64 bit|[![Build Status](https://travis-ci.org/raldus/roland.svg?branch=master)](https://travis-ci.org/raldus/roland)
Windows|32 & 64 bit|[![Build status](https://ci.appveyor.com/api/projects/status/ced6joh6ichpeqaf/branch/master?svg=true)](https://ci.appveyor.com/project/raldus/roland-0xe5q/branch/master)
-|CodeClimate|[![Code Climate](https://codeclimate.com/github/raldus/roland/badges/gpa.svg)](https://codeclimate.com/github/raldus/roland)|
-|Codacy|[![Codacy Badge](https://api.codacy.com/project/badge/Grade/19a42389d36f4f99b2827aff199d2f8c)](https://www.codacy.com/app/raldus/roland?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=raldus/roland&amp;utm_campaign=Badge_Grade)|

### General
You will need cmake and git installed.
```bash
git clone https://github.com/raldus/roland.git
cd roland
```

### Linux
Folowing an example of an unix release build:
```bash
cmake -H. -Bbuild -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
If you plan to build an RPM package or to install with make,"-DCMAKE_BUILD_TYPE=" **must be** "Release".
If you plan to build an run from source directory,"-DCMAKE_BUILD_TYPE=" **must be** "Debug".

### RPM package

```bash
cmake --build build --target package
```

### Installation

```bash
cmake --build build --target install
```

### Windows
At the moment Visual Studio 2015 32bit and MinGW are the only valid build environments.
Following an example of a Visual Studio release build:
```bash
cmake -H. -Bbuild -G"Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Documentation
You will need Doxygen installed to generate the documentation.
```bash
cmake --build build --target doc
```

## Usage
Key|Function
-----|--------
F1|toggle gui on/off
F2|insert disk in drive A:
F3|insert disk in drive B:
F4|enable/disable joystick emulation (cursorkeys + alt)
F6|toggle bilinear filtering in OpenGL graphics driver
F7|switch to 2D graphics driver depending on OS (default)
F8|switch to OpenGL graphics driver
F9|reset
F10|exit
F12|toggle fullscreen

### Usage of the CPC 464/664/6128
Insert a disk, enter **cat**  **Enter** and
you will see the directory. Enter **run "filename"**  **Enter** (mostly *.bas).

#### Example
>
1. press **F2**
2. search a disk with **arrowkeys** (HarveyH.dsk) **Enter**
3. type **cat**  **Enter**
4. in this case the file is named **Harvey**
5. type **run "harvey"**  **Enter**
6. press **F5** to emulate a joystick with **arrowkeys** and **ALT**

#### Example 2 (Bjack1.dsk)
>
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
<br><br><br>
[ ![Ready](https://github.com/raldus/roland/blob/master/assets/img/ready.png)](https://www.rolandemu.de/index.html)
