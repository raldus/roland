# Roland Emulator
Roland is an emulator of the Amstrad/Schneider CPC 464/664/6128 Homecomputer.

# Linux
Um die Linuxversion zu starten, ist es notwendig in der Datei "roland-linux.cfg" die folgenden
Pfade anzupassen:

cpcrom=/mnt/shared/emu/cpc/roms/cpc6128.rom -> /../dieserOrdner/roms/cpc6128.rom
amsdos=/mnt/shared/emu/cpc/roms/amsdos.rom  -> /../dieserOrdner/roms/amsdos.rom
diskdir=/mnt/shared/emu/cpc/dsk             -> /../dieserOrdner/disks

Dann die "roland-linux.cfg" als ".roland" ins Homeverzeichnis kopieren und roland starten.
Es wird die SDL-Bibliothek > 1.2 benötigt.

# Usage
Taste|Funktion
-----|--------
F2|Diskette in Laufwerk A: einlegen
F3|Diskette in Laufwerk B: einlegen
F4|cat/run Anweisungen ausführen (buggy)
F5|Joystickemulation ein-/aus-schalten (Cursortasten + Alt)
F9|Reset
F10|Exit
F12|umschalten auf Vollbild
      (funktioniert unter Windows nicht, stattdessen fullscreen=1 in der roland.cfg)


Nach einlegen einer Diskette <cat> eingeben und
nach Ausgabe des Verzeichnises <run "filename"> (meistens *.bas).
Beispiel:
1. [F2] drücken
2. Mit den Pfeiltasten eine Disk suchen (HarveyH.dsk) [Enter]
3. [cat] eingeben
4. Die zu startende Datei heißt hier "Harvey."
5. [run "harvey] eingeben. (abschließendes " ist nicht nötig)[Enter]
6. Taste F5 drücken um den Joystick über die Pfeiltasten und ALT zu emulieren.

oder (Bjack1.dsk)
...
4. Die zu startende Datei heißt hier "Bomb.bin"
5. [run "bomb] eingeben. [Enter]

In den meisten Fällen ist auf der Disk eine *.bas Datei, die das
entsprechende Spiel dann startet. z.B.: disc.bas



Einstellungen kann man im Installationsverzeichniss in der Datei
roland.cfg ändern, es sind aber noch nicht alle implementiert.
Es funktionieren:
-showfps    (true/false) Framerate
-fullscreen (true/false) Fenster/Vollbild
-monitor    (0=color, 1=green) Farb-/Grünmonitor
-border     (true/false) Rand an/aus, andert (noch) nicht die Anzeigegröße
-intensity  (1-20) Helligkeit der Darstellung
-doublescan (true/false) Der CPC hatte Scanlines...
-jumpers    (DIP-Schalter des CPC - für Experten)

ToDo:
- Einige Tasten sind noch nicht richtig zugeordnet...
- Prozessorauslastung unter Windows ständig bei 100%
- vieles mehr...



