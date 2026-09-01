MCU    ?= atmega2561
TARGET  = NeoB0x_$(MCU)
OUT     = out/$(MCU)
SRC_DIR = src
CC      = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size

SRC = $(addprefix $(SRC_DIR)/,main.c compcontrol.c delay.c dinsync.c eeprom.c \
      keyboard.c led.c midi.c pattern_edit.c pattern_play.c randomizer.c \
      switch.c synth.c)
# sorted (alphabetical) link order: LTO code layout is order-sensitive, this
# order measured smallest for the flash-constrained atmega162 build
OBJ = $(addprefix $(OUT)/,$(sort $(notdir $(SRC:.c=.o))))

CPU_FLAGS_atmega162  = -mmcu=atmega162
CPU_FLAGS_atmega2561 = -mmcu=atmega2561

EXTRA_FLAGS_atmega162  =
EXTRA_FLAGS_atmega2561 =

# Size optimization: the atmega162 build only fits with all features (incl. DIN
# sync-out and scale-aware transpose) when prologues become calls and several
# optimizations are traded for size.
SIZE_FLAGS = -mcall-prologues -fno-inline-functions-called-once -fno-partial-inlining \
             -fno-tree-dominator-opts -fno-tree-fre -fno-move-loop-invariants

CFLAGS  = $(CPU_FLAGS_$(MCU)) $(EXTRA_FLAGS_$(MCU)) -Os -Wall -I$(SRC_DIR) -fpack-struct -fshort-enums \
          -funsigned-char -funsigned-bitfields $(SIZE_FLAGS) \
          -ffunction-sections -fdata-sections -flto
# size flags must also be on the link line: with -flto the actual code
# generation happens at link time
LDFLAGS = $(CPU_FLAGS_$(MCU)) $(SIZE_FLAGS) -Wl,--relax -Wl,--gc-sections -flto -lm

all: $(OUT)/$(TARGET).hex $(OUT)/$(TARGET).lss size

$(OUT):
	mkdir -p $(OUT)

$(OUT)/$(TARGET).elf: $(OBJ) | $(OUT)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

$(OUT)/%.o: $(SRC_DIR)/%.c | $(OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/$(TARGET).hex: $(OUT)/$(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(OUT)/$(TARGET).eep: $(OUT)/$(TARGET).elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

$(OUT)/$(TARGET).lss: $(OUT)/$(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

size: $(OUT)/$(TARGET).elf
	$(SIZE) $(OUT)/$(TARGET).elf

clean:
	rm -rf out

.PHONY: all clean size
