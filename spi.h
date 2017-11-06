/* A lot of code is borrowed from EasyPIO.h*/
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////

// GPIO FSEL Types
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

#define GPFSEL   ((volatile unsigned int *) (gpio + 0))
#define GPSET    ((volatile unsigned int *) (gpio + 7))
#define GPCLR    ((volatile unsigned int *) (gpio + 10))
#define GPLEV    ((volatile unsigned int *) (gpio + 13))
#define SPI0clk  (*(volatile unsigned int *)  (spi + 2))
#define SPI0cs   (*(volatile unsigned int *) spi)
#define SPI0FIFO (*(volatile unsigned int *)  (spi+1))

// Physical addresses
#define BCM2836_PERI_BASE        0x3F000000
#define GPIO_BASE               (BCM2836_PERI_BASE + 0x200000)
#define BLOCK_SIZE              (4*1024)
#define SPI_BASE		       (GPIO_BASE + 0x4000)

// Pointers that will be memory mapped when pioInit() is called
volatile unsigned int *gpio; //pointer to base of gpio
volatile unsigned int *spi; 

void pioInit();
void spiInit(int frequency, int settings);
void pinMode(int pin, int function);
char SPIsendReceive(char send);

void pioInit(){
     int mem_fd;
     void *spi_map;
     void *reg_map;

     if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
     	printf("can't open /dev/mem \n");
	exit(-1);
	}

	reg_map = mmap(
		  NULL,             //Address at which to start local mapping (null means don't-care)
		  BLOCK_SIZE,       //Size of mapped memory block
		  PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
		  MAP_SHARED,       // This program does not have exclusive access to this memory
		  mem_fd,           // Map to /dev/mem
		  GPIO_BASE);       // Offset to GPIO peripheral

			if (reg_map == MAP_FAILED) {
		      printf("gpio mmap error %d\n", (int)reg_map);
		      close(mem_fd);
		      exit(-1);
								      }

	spi_map = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, SPI_BASE);

	spi = (volatile unsigned *)spi_map;
	gpio = (volatile unsigned *)reg_map;
}
/*Sets pins function*/
void pinMode(int pin, int function){
     int reg = pin/10;
     int offset = (pin%10)*3;
     GPFSEL[reg] &= ~((0b111 & ~function)<<offset); // zeros
     GPFSEL[reg] |= ((0b111 & function)<<offset); //ones
}

/*Intializes SPI0 pins*/
void spiInit(int frequency, int settings){

     // set pins to SPI mode
     pinMode(11,ALT0); //SCLK pin
     pinMode(10,ALT0); //MOSI pin
     pinMode(9,ALT0); //MISO pin
     pinMode(8,ALT0); //CS pin
     SPI0clk=250000000/frequency;
     SPI0cs &= ~(~settings<<2); // set zeros
     SPI0cs |= settings<<2; // set ones
     SPI0cs |= 0x80; //set TA to 1 to enable
}

/*Command sends word to SPI device and returns response*/
char SPIsendReceive(char send){
     //send data out
     SPI0FIFO=send; 
     //wait for DONE
     while(! SPI0cs>>16);
     //return data in
     return SPI0FIFO; // return data in
}
