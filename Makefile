# =============================================================
# AUTHOR:   Daniel Moura
# EMAIL:    oxesoft@gmail.com
# DATE:     2012-06-25
# COMMENTS: Done to speed up the build of a sketch
# =============================================================
ifndef SKETCH
    $(error Please inform you Sketch filename using "make SKETCH=sketch_name.ino ARDUINO_PATH=path/to/arduino")
endif
PROJECT:=$(basename $(SKETCH))
ifndef ARDUINO_PATH
    ARDUINO_PATH:=/usr/share/arduino
endif
ifeq ($(ARDUINO_PATH), )
    $(error Please inform the Arduino path like "make SKETCH=sketch_name.ino ARDUINO_PATH=path/to/arduino")
endif

SRC_PATH:=$(shell find $(ARDUINO_PATH) -type f -name Arduino.h | head -n 1)
ifeq ($(SRC_PATH), )
    $(error Arduino.h not found)
endif
SRC_PATH:=$(dir $(SRC_PATH))

VARIANTS_PATH:=$(shell find $(ARDUINO_PATH) -type d -name variants | head -n 1)
ifeq ($(VARIANTS_PATH), )
    $(error "variants" dir not found)
endif

BIN_DIR:=$(shell find $(ARDUINO_PATH) -type f -name avr-gcc)
ifeq ($(BIN_DIR), )
    BIN_DIR:=$(shell which avr-gcc)
endif
ifeq ($(BIN_DIR), )
    $(error avr-gcc not found)
endif
BIN_DIR:=$(dir $(BIN_DIR))

AVRDUDE:=$(shell find $(ARDUINO_PATH) -type f -name avrdude | head -n 1)
ifeq ($(AVRDUDE), )
    AVRDUDE:=$(shell which avrdude)
endif
ifeq ($(AVRDUDE), )
    $(error avrdude not found)
endif

INCLUDE:=-I$(SRC_PATH)
INCLUDE+=-I$(VARIANTS_PATH)/mega

CFLAGS=-c -g -Os -ffunction-sections -fdata-sections -mmcu=atmega2560 -DF_CPU=16000000L -DUSB_VID=null -DUSB_PID=null -DARDUINO=101
CXXFLAGS=$(CFLAGS) -fno-exceptions

AR     =$(BIN_DIR)/avr-ar
CC     =$(BIN_DIR)/avr-gcc
CXX    =$(BIN_DIR)/avr-g++
OBJCOPY=$(BIN_DIR)/avr-objcopy
OBJDUMP=$(BIN_DIR)/avr-objdump

CORE_SRC_C:=WInterrupts.c
CORE_SRC_C+=wiring.c
CORE_SRC_C+=wiring_analog.c
CORE_SRC_C+=wiring_digital.c
CORE_SRC_C+=wiring_pulse.c
CORE_SRC_C+=wiring_shift.c

CORE_SRC_CPP:=CDC.cpp
CORE_SRC_CPP+=HardwareSerial.cpp
CORE_SRC_CPP+=HID.cpp
CORE_SRC_CPP+=IPAddress.cpp
CORE_SRC_CPP+=main.cpp
CORE_SRC_CPP+=new.cpp
CORE_SRC_CPP+=Print.cpp
CORE_SRC_CPP+=Stream.cpp
CORE_SRC_CPP+=Tone.cpp
CORE_SRC_CPP+=USBCore.cpp
CORE_SRC_CPP+=WMath.cpp
CORE_SRC_CPP+=WString.cpp

CORE_OBJS=$(addsuffix .o, $(CORE_SRC_C)) $(addsuffix .o, $(CORE_SRC_CPP))

all: $(PROJECT).cpp.hex

$(PROJECT).cpp.hex: core.a $(PROJECT).cpp.o
	@echo "Building hex file"
	@$(CC) -Os -Wl,--gc-sections,--relax -mmcu=atmega2560 -o $(PROJECT).cpp.elf $(PROJECT).cpp.o core.a -lm 
	@$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $(PROJECT).cpp.elf $(PROJECT).cpp.eep
	@$(OBJCOPY) -O ihex -R .eeprom $(PROJECT).cpp.elf $(PROJECT).cpp.hex

$(PROJECT).cpp.o: $(PROJECT).ino
	@echo "Compiling project"
	@echo "#include <Arduino.h>" > $(PROJECT).cpp
	@cat $(PROJECT).ino >> $(PROJECT).cpp
	@$(CXX) -Wall $(CXXFLAGS) $(INCLUDE) $(PROJECT).cpp -o $(PROJECT).cpp.o

flash:
	@echo "Flasing"
	$(AVR_DIR)/avrdude -C $(AVR_DIR)/avrdude.conf -p atmega2560 -c stk500v2 -P /dev/ttyACM0 -b 115200 -D -U flash:w:$(PROJECT).cpp.hex:i

dump: $(PROJECT).cpp.o
	@echo "Dump resulting asm to $(PROJECT).cpp.asm"
	@$(OBJDUMP) -wdS --no-show-raw-insn $(PROJECT).cpp.o > $(PROJECT).cpp.asm

core.a:
	@for i in $(CORE_SRC_C); \
	do \
		echo "Compiling $$i" ; \
		$(CC) -w $(CFLAGS) $(INCLUDE) $(SRC_PATH)/$$i -o $$i.o ; \
	done
	@for i in $(CORE_SRC_CPP); \
	do \
		echo "Compiling $$i" ; \
		$(CXX) -w $(CXXFLAGS) $(INCLUDE) $(SRC_PATH)/$$i -o $$i.o ; \
	done
	@for i in $(CORE_OBJS); \
	do \
		echo "Archiving $$i" ; \
		$(AR) rcs core.a $$i ; \
	done
	@rm $(CORE_OBJS)

clean:
	@rm -f core.a $(CORE_OBJS) $(PROJECT).cpp.eep $(PROJECT).cpp.elf $(PROJECT).cpp.hex $(PROJECT).cpp $(PROJECT).cpp.o
