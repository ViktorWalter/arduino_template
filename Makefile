#
# Simple Arduino Makefile
#
# Author: Nick Gammon
# Date: 18th March 2015

# where you installed the Arduino app
ARDUINO_DIR = /usr/share/arduino/

# various programs
# CC = "$(ARDUINO_DIR)hardware/tools/avr/bin/avr-gcc"
CC = avr-gcc
CPP = avr-g++
AR = avr-ar
OBJ_COPY = avr-objcopy

MAIN_SKETCH = sketch.cpp
ON_SKETCH = on.cpp

# compile flags for g++ and gcc

# may need to change these
F_CPU = 16000000
MCU = atmega328p

# compile flags
GENERAL_FLAGS = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MCU) -DF_CPU=$(F_CPU)L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=106
CPP_FLAGS = $(GENERAL_FLAGS) -fno-exceptions
CC_FLAGS  = $(GENERAL_FLAGS)

# location of include files
INCLUDE_FILES = "-I$(ARDUINO_DIR)hardware/arduino/cores/arduino" "-I$(ARDUINO_DIR)hardware/arduino/variants/standard"

# library sources
LIBRARY_DIR = "$(ARDUINO_DIR)hardware/arduino/cores/arduino/"

#board
CHIP = ATMEGA328P
BAUD = 57600
PORT = /dev/ttyUSB0

all:
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR) $(MAIN_SKETCH) -o $(MAIN_SKETCH).o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)avr-libc/malloc.c -o malloc.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)avr-libc/realloc.c -o realloc.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WInterrupts.c -o WInterrupts.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring.c -o wiring.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_analog.c -o wiring_analog.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_digital.c -o wiring_digital.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_pulse.c -o wiring_pulse.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_shift.c -o wiring_shift.c.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)CDC.cpp -o CDC.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial.cpp -o HardwareSerial.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HID.cpp -o HID.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)IPAddress.cpp -o IPAddress.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)main.cpp -o main.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)new.cpp -o new.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Print.cpp -o Print.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Stream.cpp -o Stream.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Tone.cpp -o Tone.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)USBCore.cpp -o USBCore.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WMath.cpp -o WMath.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WString.cpp -o WString.cpp.o 
	rm -f core.a
	$(AR) rcs core.a malloc.c.o 
	$(AR) rcs core.a realloc.c.o 
	$(AR) rcs core.a WInterrupts.c.o 
	$(AR) rcs core.a wiring.c.o 
	$(AR) rcs core.a wiring_analog.c.o 
	$(AR) rcs core.a wiring_digital.c.o 
	$(AR) rcs core.a wiring_pulse.c.o 
	$(AR) rcs core.a wiring_shift.c.o 
	$(AR) rcs core.a CDC.cpp.o 
	$(AR) rcs core.a HardwareSerial.cpp.o 
	$(AR) rcs core.a HID.cpp.o 
	$(AR) rcs core.a IPAddress.cpp.o 
	$(AR) rcs core.a main.cpp.o 
	$(AR) rcs core.a new.cpp.o 
	$(AR) rcs core.a Print.cpp.o 
	$(AR) rcs core.a Stream.cpp.o 
	$(AR) rcs core.a Tone.cpp.o 
	$(AR) rcs core.a USBCore.cpp.o 
	$(AR) rcs core.a WMath.cpp.o 
	$(AR) rcs core.a WString.cpp.o 
	$(CC) -Os -Wl,--gc-sections -mmcu=$(MCU) -o $(MAIN_SKETCH).elf $(MAIN_SKETCH).o core.a -lm 
	$(OBJ_COPY) -O ihex -R .eeprom $(MAIN_SKETCH).elf $(MAIN_SKETCH).hex 
	avrdude  -F -V -p $(CHIP) -P $(PORT) -b $(BAUD) -c arduino -U flash:w:$(MAIN_SKETCH).hex:i

on:
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR) $(ON_SKETCH) -o $(ON_SKETCH).o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)avr-libc/malloc.c -o malloc.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)avr-libc/realloc.c -o realloc.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WInterrupts.c -o WInterrupts.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring.c -o wiring.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_analog.c -o wiring_analog.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_digital.c -o wiring_digital.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_pulse.c -o wiring_pulse.c.o 
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_shift.c -o wiring_shift.c.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)CDC.cpp -o CDC.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial.cpp -o HardwareSerial.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HID.cpp -o HID.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)IPAddress.cpp -o IPAddress.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)main.cpp -o main.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)new.cpp -o new.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Print.cpp -o Print.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Stream.cpp -o Stream.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Tone.cpp -o Tone.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)USBCore.cpp -o USBCore.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WMath.cpp -o WMath.cpp.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WString.cpp -o WString.cpp.o 
	rm -f core.a
	$(AR) rcs core.a malloc.c.o 
	$(AR) rcs core.a realloc.c.o 
	$(AR) rcs core.a WInterrupts.c.o 
	$(AR) rcs core.a wiring.c.o 
	$(AR) rcs core.a wiring_analog.c.o 
	$(AR) rcs core.a wiring_digital.c.o 
	$(AR) rcs core.a wiring_pulse.c.o 
	$(AR) rcs core.a wiring_shift.c.o 
	$(AR) rcs core.a CDC.cpp.o 
	$(AR) rcs core.a HardwareSerial.cpp.o 
	$(AR) rcs core.a HID.cpp.o 
	$(AR) rcs core.a IPAddress.cpp.o 
	$(AR) rcs core.a main.cpp.o 
	$(AR) rcs core.a new.cpp.o 
	$(AR) rcs core.a Print.cpp.o 
	$(AR) rcs core.a Stream.cpp.o 
	$(AR) rcs core.a Tone.cpp.o 
	$(AR) rcs core.a USBCore.cpp.o 
	$(AR) rcs core.a WMath.cpp.o 
	$(AR) rcs core.a WString.cpp.o 
	$(CC) -Os -Wl,--gc-sections -mmcu=$(MCU) -o $(ON_SKETCH).elf $(ON_SKETCH).o core.a -lm 
	$(OBJ_COPY) -O ihex -R .eeprom $(ON_SKETCH).elf $(ON_SKETCH).hex 
	avrdude  -F -V -p $(CHIP) -P $(PORT) -b $(BAUD) -c arduino -U flash:w:$(ON_SKETCH).hex:i
upload:
	avrdude  -F -V -p $(CHIP) -P $(PORT) -b $(BAUD) -c arduino -U flash:w:$(MAIN_SKETCH).hex:i
	#stty -f $(PORT) hupcl
