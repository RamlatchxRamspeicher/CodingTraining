default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o blinky_world.o blinky_world.c
	avr-gcc -o blinky_world.bin blinky_world.o

	avr-objcopy -O ihex -R .eeprom blinky_world.bin blinky_world.hex
	sudo avrdude -F -V -c arduino -p ATMEGA328P -P /path/to/arduino -b 115200 -U flash:w:blinky_world.hex