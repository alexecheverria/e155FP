#include "spi.h"
 
 void  main(void){
   int output;
   int fclk;
   float vin;
   int vdd=5;
   int out;

   /*Intialize PIO*/
   pioInit();

   fclk = 122000; // ADC has 3.2MHz clk at 5V

   /*Voltage Scale Logic*/
   //TODO: Determine what voltage scales we want. Max input 5 Volts
   bool voltScale[3]; // [x volts/div, y volts div, 1 volt/div]
   pinMode(16,INPUT); //x volts/div
   pinMode(20,INPUT); //y volts/div
   pinMode(21,INPUT); //1 volt/div
  
   voltsScale[0] = digitalRead(16);
   voltsScale[1] = digitalRead(20);
   voltsScale[2] = digitalRead(21);

   /*Time Scale Logic*/
   bool timeScale[3]; // [x volts/div, y volts div, 1 volt/div]
   pinMode(13,INPUT); //x volts/div
   pinMode(19,INPUT); //y volts/div
   pinMode(26,INPUT); //1 volt/div
  
   timeScale[0] = digitalRead(13);
   timeScale[1] = digitalRead(19);
   timeScale[2] = digitalRead(26);

   /*Initialize SPI pins*/
   spiInit(fclk, 00); // set both phase and pol to 0


   //OLD ADC CODE
   /* Retreive data from the ADC*/
   int out1 = (int)  SPIsendReceive(0b01101000);
   int out2 = (int) SPIsendReceive(0b00000000);
   out1 &= 0b11; // get rid of first 6 bits
   out1 = out1 << 8;
   out = out1 | out2;
   // convert digital output code to voltage
   vin = (float) out * vdd / 1024;

   printf("%s%c%c\n", "Content-Type: text/html;charset=iso-8859-1",13,10);
   printf("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"0;url=/cgi-bin/lab6\">");
   printf("<b>The light sensor's voltage is: %f </b><p>", vin);
   printf("<i>Press the back button to return.</i>\n\n");

   return;
 }
