#######################################################################
######                                                           ######
#######################################################################
###### Project:   dirarare-matrixdriver
###### Developer: Peter Shaw
###### Date: Jan/2014
###### URL: 
#######################################################################
# Fuses: 0xff 0xdf 0xff
#
PROJECTNAME = dirarare-matrixdriver

include environment/build.mk
include environment/applications.mk
include environment/board.mk
include environment/programmer.mk
include environment/fuse.mk

include options.mk

# C Source files
PRJSRC = $(shell find ./src* -name *.c)

# additional includes (e.g. -I/path/to/mydir)
INC = 

# libraries to link in (e.g. -lmylib)
LIBS = 

# Optimization level,
# use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL = s

# HEXFORMAT -- format for .hex file output
HEXFORMAT = ihex

# compiler
CFLAGS = -I$(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL)  \
		 $(DASH_VARS)							   \
         -DF_CPU=$(F_CPU)                          \
         $(BOARD_OPTS)								\
		 --std=gnu99 							   \
         -fpack-struct -fshort-enums               \
         -funsigned-bitfields -funsigned-char      \
         -Wall -Wstrict-prototypes                 \
         -Wa,-ahlms=$(OBJECTDIR)/$(firstword       \
                    $(filter %.lst, $(<:.c=.lst)))

# linker
LDFLAGS = -Wl,-Map,$(TRG).map -mmcu=$(MCU)  \
          -L "$${LIBPATH}"                  \
          -lm $(LIBS)


##### automatic target names ####
TRG=$(TARGETDIR)/$(PROJECTNAME).out
DUMPTRG=$(TARGETDIR)/$(PROJECTNAME).s

HEXROMTRG=$(TARGETDIR)/$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(TARGETDIR)/$(PROJECTNAME).ee.hex

# Start by splitting source files by type
#  C
CFILES=$(filter %.c, $(PRJSRC))

# List all object files we need to create
_OBJDEPS=$(CFILES:.c=.o)
OBJDEPS=$(_OBJDEPS:./src=$(OBJECTDIR)/src)

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

# All the possible generated assembly
# files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s))

.SUFFIXES : .c .o .out .s .h .hex .ee.hex

dir_guard=$(MKDIR_P) $(OBJECTDIR)/$(@D)

# Make targets:
# all, disasm, stats, hex, writeflash/install, clean
all: prepare $(TRG) $(HEXTRG)

prepare:
	$(MKDIR_P) $(OBJECTDIR)
	$(MKDIR_P) $(TARGETDIR)

test:
	$(MAKE) -C tests
	./tests/target/tests

$(TRG): $(OBJDEPS)
	$(CC) $(LDFLAGS) -o $(TRG) $(shell find $(OBJECTDIR) -type f -name *.o) -I$(INC) -lm $(LIBS)

#### Generating assembly ####
# asm from C
%.s: %.c
	$(dir_guard)
	$(CC) -S $(CFLAGS) $< -o $(OBJECTDIR)/$@ 

#### Generating object files ####
# object from C
.c.o:
	$(dir_guard)
	$(CC) $(CFLAGS) -c $< -o $(OBJECTDIR)/$@ 

#### Generating hex files ####
# hex files from elf
.out.hex:
	$(OBJCOPY) -j .text                    \
	           -j .data                    \
	           -O $(HEXFORMAT) $< $@

.out.ee.hex:
	$(OBJCOPY) -j .eeprom                     \
	           --change-section-lma .eeprom=0 \
	           -O $(HEXFORMAT) $< $@

#### Information ####
info:
	which $(CC)
	$(CC) -v
	avr-ld -v

#### Upload ####
upload:
	$(AVRDUDE) -c $(PROGRAMMER) -P usb -Uflash:w:$(HEXROMTRG) -p $(PROGRAMMER_MCU)

#### Cleanup ####
clean:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) -r $(TARGETDIR)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) -r $(OBJECTDIR)
	$(MAKE) -C tests clean

#### Set Fusebits ####
fuse:
	$(AVRDUDE) -c $(PROGRAMMER) -P usb -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m -p $(PROGRAMMER_MCU)
