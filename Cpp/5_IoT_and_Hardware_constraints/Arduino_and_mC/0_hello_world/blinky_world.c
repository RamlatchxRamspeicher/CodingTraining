#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    //set led (PORTB5) as output
    DDRB = DDRB | (1 << DDB5);

    //forever
    while (1) {
        
        //turn LED on for one second
        PORTB = PORTB | (1 << PORTB5);

        _delay_ms(1000);

        //turn LED off for one second
        PORTB = PORTB & ~(1 << PORTB5);

        _delay_ms(1000);
    }
    
}

/*compile with makefile
1. only compile
avr compiler | optimize for speed | CPU clock is 16 MHz (quarz on arduino) | microcontroller type | only compile dont create ELF file
avr-gcc        -Os                  -DF_CPU=16000000UL                       -mmcu=atmega328p       -c -o blinky_world.o blinky_world.c

2. link and create ELF file (binary) from the object file
avr-gcc -o blinky_world.bin blinky_world.o

3. create intel hexformat file for avrdude flasher 
take the ELF file and remove the .eeprom section and output it as intel hex 
avr-objcopy -O ihex -R .eeprom blinky_world.bin blinky_world.hex

4. flash the arduino
flasher take file and work verbose | use arduino programming process | on Device     | with port           | and serial bitrate | to write to flash, wich is writeable, the hex file 
sudo avrdude -F -V                   -c arduino                        -p ATMEGA328P   -P /path/to/arduino   -b 115200            -U flash:w:blinky_world.hex
*/