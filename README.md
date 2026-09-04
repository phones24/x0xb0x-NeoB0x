# NeoB0x

Firmware for the [x0xb0x](https://www.adafruit.com/) (TB-303 style bassline
sequencer/synth clone) - a continuation of **MarOS 1.5** for the
original ATmega162 CPU and the ATmega2561 ("x0xlarge") upgrade.

NeoB0x adds to MarOS: extended randomizer scales (phrygian, locrian,
mixolydian) with scale-aware transpose, a batch of bug fixes collected from
the original forum thread (pattern clear, undo restore, auto-stop on lost
external clock, settings defaults, held-notes leak, atomic swing timers, MIDI
RX overflow, tempo EEPROM wear, etc...).

## Flashing

[Flash NeoB0x and other firmwares with the web-based flasher](https://x0.randomware-audio.cc/).

## Documentation

[One-page HTML cheatsheet](https://phones24.github.io/x0xb0x-NeoB0x/cheatsheet.html) —
  searchable reference for every mode and panel control.

## Prepare

The firmware builds with the standard AVR toolchain (avr-gcc, avr-libc, binutils and make).

**Linux**

```sh
sudo apt install gcc-avr avr-libc make    # Debian / Ubuntu
```

**macOS**

```sh
brew tap osx-cross/avr
brew install avr-binutils avr-gcc avr-libc
```

**Windows**

Use Microchip Studio (the successor of Atmel Studio) to build. 
Alternatively, install an avr-gcc toolchain with make.

## Building

Build either target from the repository root:

```sh
./compile.sh atmega162    # original x0xb0x CPU (default)
./compile.sh atmega2561   # x0xlarge 
```

On Windows, run `compile.cmd <mcu>` instead.

The resulting hex file is written to `out/<mcu>/NeoB0x_<version>_<mcu>.hex` (e.g. `NeoB0x_1.00_atmega162.hex`).

## Credits & history

NeoB0x stands on the shoulders of the x0xb0x firmware family:

- **Limor Fried / [adafruit](https://www.adafruit.com/) / [ladyada](http://www.ladyada.net/)** —
  original x0xb0x hardware, firmware and bootloader (MIT license)
- **Sokkan** — sokkOS
- **mario1089** — MarOS 1.0 – 1.5.1
- **Nordcore** — MarOS 1.5.1-r1 … V1.82 maintenance releases
- **phones24** — NeoB0x fork: randomizer scales, bug fixes

Original MarOS thread (bug reports, history, releases):
<https://forums.adafruit.com/viewtopic.php?t=33914>
