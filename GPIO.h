#ifndef _INCLUDE_GPIO_
#define _INCLUDE_GPIO_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>


// Mem locations for Periferals
#define perifBase 0x3F000000
#define GPIOBASE perifBase+0x200000

// Constants for Pin funct
#define OUTPUT 0b001

//LED output pin
#define LEDPIN 21

volatile unsigned int *GPIO;  // pointer to base of GPIO registers

// from http://mygeekyfun.blogspot.com/2012/06/gpio-on-raspberry-pi.html
// map a page (4096 bytes) starting at physical address base
volatile unsigned int *phys_to_virt(long base)
{
  int mem_fd;
  char *io_mem;
  char *io_m;
  volatile unsigned *io;

  mem_fd=open("/dev/mem",O_RDWR|O_SYNC);

  // create page-aligned pointer to 4096 bytes
  io_mem=(char *) malloc(8192);
  io_mem+=4096;
  io_mem=(char*) (((int) io_mem) & 0xfffff000);

  io_m=(char *) mmap((caddr_t)io_mem,4096,
                     PROT_READ|PROT_WRITE,
                     MAP_SHARED|MAP_FIXED,
                     mem_fd,base);
  //io=(volatile unsigned *)io_m;
  return((int *)io_m);
}

// Sets up the pointers for GPIO.h
void SetUpPerif()
{
	GPIO = phys_to_virt(GPIOBASE);
}

// sets a GPIO pin acording to func
void setPinType( int pin, int func)
{
	int reg = pin/10;
	int offset = (pin % 10)*3;
	volatile unsigned int* loc = GPIO + reg;
	*(loc) = *(volatile unsigned int*) (loc) &~((0b111 & ~ func) << offset);
	*(loc) =  *(volatile unsigned int*) (loc) | ((0b111 &   func) << offset);
}

// Used to write a pin high or low
void digitalWrite(int pin, int val)
{
	int reg = pin / 32;
	int offset = pin % 32;
	volatile unsigned int* loc = GPIO + reg + 7;
	if (val){
		*(loc) = 1 << offset;
	}
	else {
		loc += 3;
	}	*(loc) = 1 << offset;
}

#endif

int digitalRead(int pin) {
    int reg = pin / 32;
    int offset = pin % 32;

    return (GPLEV[reg] >> offset) & 0x00000001;
}


void digitalWrites(int pins[], int numPins, int val) {
    int i;
    for(i=0; i<numPins; i++) {
        digitalWrite(pins[i], (val & 0x00000001));
        val = val >> 1;
    }
}

int digitalReads(int pins[], int numPins) {
    int i, val = digitalRead(pins[0]);
    
    for(i=1; i<numPins; i++) {
        val |= (digitalRead(pins[i]) << i);
    }
    return val;
}