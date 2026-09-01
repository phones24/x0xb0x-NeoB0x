/*
 * The software for the x0xb0x is available for use in accordance with the
 * following open source license (MIT License). For more information about
 * OS licensing, please visit -> http://www.opensource.org/
 *
 * For more information about the x0xb0x project, please visit
 * -> http://www.ladyada.net/make/x0xb0x
 *
 *                                     *****
 * Copyright (c) 2005 Limor Fried
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *                                     *****
 *
 */

// Version reported to c0nb0x = MAJOR*100+MINOR (100 -> "1.00").
// Single source of truth for the project version; Makefile and CI parse it
// from here. Changelog: see the CHANGELOG file.
#define VERSION 100

#define SYNC_OUT    1 // undef if you don't want to have it - saves some bytes
#define DIN_SYNC_IN 1 //  ... both together: 708 bytes

//  some not so important features ... might be left out to save a few bytes
#define HASRANDOM    // with Random (useless noodling plus some blinkenlights ) Mode (56 Bytes)
#define KEYLED_TEST  // Keys switch on their associated LEDs in "Computer Control" Mode. For testing
					 // the tact switches and LEDs. (78 Bytes)
					 // ... and yes, you can only see if (LED or switch) is broken
#define DACBITS_TEST // set/clear Portbits directly by keys for hardware debugging

#define WITHRANGECHECK    // LED on/off range check. (change memory only if LED no. is valid)
// #define FLASHCRCCHECK  2            // 1=80/2=92 (betterLED pattern) Bytes, program flash code
// CRC check on startup. Makes CRC slower. ATN: REQUIRES externally calculated/set CRC.
#define FASTCRC           // 20/28 Bytes more, but a little bit faster
#define HASDOUBLECLICKF   // 76 Bytes doubleclick F fills pattern
#define HASPATTERNCLRFILL // Pattern clear/fill function on REST, DOWN/UP in pattern edit when
						  // stopped (where you load patterns with 1...8)

#define AUTO_INC_RUNNING   // undef, if you want to have the old behavior back, where auto increment
						   // only works when sequencer stopped.
#define STEP17           0 // 0 or 1, not undef'd! Wanna have "step17" as leave step modes?
#define CFG_SPI_CLOCK_FREQ_MHZ                                                                     \
	8 // SPI clock.  1,2,4 or 8 (MHz)
	  // either you have a very old EEPROM, than it must be 2MHz - or it could be 8MHz
	  // the HCMOS for the LEDs can do 20MHz, current EEPROMS can do 10MHz, old ones 3MHz.
	  // 25LC320A <== new, w/o the trailing A they have 3MHz. "25320LI" is a fast one, if in doubt
	  // google your chip Note: Stock-OS and Sokkos had 1MHz, the comment stating 2MHz was wrong!

#define MAX_TEMPO                                                                                  \
	255 // Limited to 255 as that fits in one byte: saves code and time (don't have to wait for
		// "first byte ready" on eeprom write. )
#define MIN_TEMPO 20 // original 20. 40 or larger saves 38 bytes, not below 20 on MEGA162

#define DEBOUNCETIME                                                                               \
	13 // time to wait between keyboard debounce polls. Must be longer than switch bounce time.
	   // (10...20ms)

// #define DOUBLE_LATCH    // Double Latch pulse for IC9

#define NOP asm("nop")

#define CLK_CALIBRATION_EEADDR 0x1 // unused
#define MIDIIN_ADDR_EEADDR     0x3
#define MIDIOUT_ADDR_EEADDR    0x4
#define TEMPO_EEADDR_H         0x5 // not used anymore
#define TEMPO_EEADDR_L         0x6 // Tempo 20  ... 255

#define USERSETTINGS_EEADDR 0x7
#define SETTINGS0 0 //  settings0 is as in Sokkos2, 4 new ones following  |  settings 1 =C#
#define SETTINGS1 1 // Settings 2=D#
#define SETTINGS2 2 // Settings3=F#
#define SETTINGS3 3 // settings4=G#,
#define SETTINGS4 4 // 5 byte settings, total   |  settings5=A#
#define SETTINGS_VERSION                                                                           \
	5 //  check OS that wrote the settings (we use SETTINGS_VERSION_MAGIC here, if it is ours)
#define FAV_RAND_SETUP 6 // Rand Setup the users liked
#define SELECTED_SCALE 7 // MarOS-compatible slot: selected randomizer scale (0..6)
//                      8 unused
//                      9  unused
//                      0xA  unused
//                      0xB  unused
//                      0xC  unused
//                      0xD  unused
//                      0xE  unused
//                      0xF  unused
#define NUM_SETTINGS_TOTAL (0x10) // total settings in "user-settings" area
//                      0x17 // = next free address

#define SETTINGS_VERSION_MAGIC 0x41

#define MAX_NUM_C_SETTINGS 5 // max 5, as there are no more black keys which select among them

// Bits in Settings0	Sokkos C# Group
#define SETTINGS0_NOHIGHOCT                                                                        \
	0 // Key #1	disable high octave (shift = +2, the blinking one)  (as in Sokkos 2)
#define SETTINGS0_NOSTORETEMPO  1 // Key #2	don't store tempo changes immediately (as in Sokkos 2)
#define SETTINGS0_LIVE_SETCHAIN 2 // Key #3	Live Edit sets edited pattern in chain (as in Sokkos 2)
// #define SETTINGS0_EDIT303TSYLE	3	// Key #4	reserved (from Sokkos 2 ... will be completely
// impossible to do this - that would need twice the flash size or more... )
#define SETTINGS0_KEEP_PATT     4 // Key #5	preserve selected pattern when changing mode
// #define SETTINGS0_KEY6            5   // Key #6   (unused )
#define SETTINGS0_AUTO_INC      6 // Key #7	auto increment for step mode
// #define SETTINGS0_TIJUANATAXI		7	// Key #8	reserved (from Sokkos 2, MEGA2561 only )

// If you know what setting you need you might compile it "fixed" - saves some bytes
// #define FIXED_SETTING_0 ( (1<<SETTINGS0_LIVE_SETCHAIN) | (1<<SETTINGS0_KEEP_PATT) )

// Bits in Settings1 MarOS: D# Group
#define SETTINGS1_STARTMODE                                                                        \
	0 // Key #1  LED OFF = Start only on MIDI-NoteOn, LED ON = Midi-Start "old" behavior (like
	  // Sokkos or Stock FW),
#define SETTINGS1_NOSYNCFIX                                                                        \
	1 // Key #2  LED ON = disable Fix (fakes a clock if starting midi note comes immediately after
	  // the real clock)
#define SETTINGS1_PRESERVE_TIMING                                                                  \
	2                       // Key #3  LED ON = preserve swing & gate len when loading param preset
#define SETTINGS1_HELPLED 3 // Key #4  LED ON = Dark Light on LED 1,5,9,13 for orientation
#define SETTINGS1_FAST_TEMPOMEAS                                                                   \
	4 // Key #5  LED ON = Less accurate, but faster MIDI sync tempo measure (if jitter is too high
	  // to get any results)
#define SETTINGS1_CONTINUESTART                                                                    \
	5 // Key #6  LED ON = Midi Continue behaves like midi start (resets to first note in pattern)
#define SETTINGS1_DIM2nd   6 // Key #7  LED ON = diminished 2nd for phrygian and arabic scales
#define SETTINGS1_HARM_MIN 7 // Key #8  LED ON = major 7th in minor for harmonic minor
// #define FIXED_SETTING_1
// ((1<<SETTINGS1_STARTMODE)|(1<<SETTINGS1_NOSYNCFIX)|(1<<SETTINGS1_HELPLED))

// Bits in Settings2 : F# Group
#define SETTINGS2_FILLWITHC2                                                                       \
	0 // Key #1   Randomizer Pattern Fill on F doubleclick fills with C2 notes instead of RESTs
// #define SETTINGS2_KEY2           1    // Key #2   (unused )
// #define SETTINGS2_KEY3           2    // Key #3   (unused )
// #define SETTINGS2_KEY4           3    // Key #4   (unused )
#define SETTINGS2_KEEP_STEPMODE                                                                    \
	4 // Key #5   Keep step edit mode (and current Step)  when changing between Run & Stop
#define SETTINGS2_ENC_Q2 5 // Key #6  LED ON = also use Encoder Quadrature 2 (Std: LL->HL->HH->LH
// #define SETTINGS2_KEY7           6    // Key #7   (unused )
// #define SETTINGS2_KEY8           7    // Key #8   (unused )

// #define FIXED_SETTING_2			0

#ifdef FIXED_SETTING_2
#define NUM_C_SETTINGS 2
#else
#define NUM_C_SETTINGS 3 // used ones - so completely empty can be made non selectable
#endif

// if all settings fixed, user C menu is not needed at all, saving more bytes (about 550 total,
// depends a little on your settings. )
#define ALL_FIXED_SETTING                                                                          \
	(defined(FIXED_SETTING_0) && defined(FIXED_SETTING_1) && defined(FIXED_SETTING_2))

#ifdef FIXED_SETTING_0
#define IS_SET0(bit)                                                                               \
	((FIXED_SETTING_0) & (1 << (bit))) // compiler can evaluate this = half of the code (either if
									   // or else..) gets optimized away.
#else
#define IS_SET0(bit) (settings[SETTINGS0] & (1 << (bit)))
#endif
#ifdef FIXED_SETTING_1
#define IS_SET1(bit) ((FIXED_SETTING_1) & (1 << (bit)))
#else
#define IS_SET1(bit) (settings[SETTINGS1] & (1 << (bit)))
#endif
#ifdef FIXED_SETTING_2
#define IS_SET2(bit) ((FIXED_SETTING_2) & (1 << (bit)))
#else
#define IS_SET2(bit) (settings[SETTINGS2] & (1 << (bit)))
#endif

#define BOOTLOADER_ADDR 0x3E00UL // 0x1F00 word, 15872 byte
#define INTERNAL_SYNC   0
#define DIN_SYNC        1
#define MIDI_SYNC       2
#define NO_SYNC         3 // for like, keyboard mode & stuff?
#define FALSE           0
#define TRUE            1

#define NUM_KNOB_PARAMS 11

#ifndef sbi
#define sbi(p, b) (p) |= (1 << (b))
#endif
#ifndef cbi
#define cbi(p, b) (p) &= ~(1 << (b))
#endif

/************* function prototypes */
void ioinit(void);
void change_tempo(uint16_t newtempo);

uint8_t random100(void);
uint8_t random_x(uint8_t x);
// uint8_t random(void);
#define random()    random_x(0)
// uint8_t randomIf(uint8_t percent);
#define randomIf(p) (random100() < (p))
// #define randomIf(p)  ((p)>random_x(100))

uint8_t findEOP();
void    clearPendingDinPulses();
// void	clearScheduledNotes();

void measureTempo();

void turn_off_tempo(void);
void turn_on_tempo(void);
void init_tempo(void);
void init_timer0(void);
void init_timer2(void);

void do_tempo(void);

uint8_t internal_eeprom_read8(uint16_t addr);
void    internal_eeprom_write8(uint16_t addr, uint8_t data);

void tempoval_led(void); // sets the bank led to the current tempo knob parameter value
void clock_ticks(void);
void changeTempoKnobValue(int8_t moved_ticks);

/*************   exported variables  */

extern uint8_t *const tk_paras[NUM_KNOB_PARAMS]; // table for tempo knob parameters for editing
extern const uint8_t  tk_maxval[NUM_KNOB_PARAMS];
extern const uint8_t  tk_fnkey[NUM_KNOB_PARAMS];

extern uint8_t settings[NUM_SETTINGS_TOTAL]; // "USER C" + other configuration bytes
extern uint8_t sync;

extern uint8_t pattern_buff[]; // the 'loaded' pattern buffer
extern uint8_t prev_note;
extern uint8_t note_counter;
extern uint8_t tempo;
extern uint8_t playing; // are we playing?
extern uint8_t curr_note;
extern uint8_t swing_it;
extern uint8_t curPatternRun;

extern uint16_t delay_clock9ms; // timer 3 ticks to get 9ms delay for dinsync start

// Interrupt stuff:
extern volatile uint8_t debounce_timer; // modified by timer0 in main.c
extern volatile uint8_t run_tempo;
extern volatile int8_t  tk_value; // tempo knob moved
#ifdef HASDOUBLECLICKF
extern volatile uint8_t doublePressTimeOut;
#endif
extern volatile uint16_t uart_timeout;
extern volatile uint16_t tapTempoTimer;
extern volatile uint8_t  tempoMeasure; // 0xff=external clock timeout
