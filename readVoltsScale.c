#include <stdio.h>
#include "EasyPIO.h"

void setPins(){
	//TimeScale Pins
       pinMode(13,INPUT); 
       pinMode(19,INPUT); 
       pinMode(26,INPUT); 
}


 int main(void){
   /*Intialize PIO*/
   pioInit();
   int voltScale[3];
   setPins();

   if(digitalRead(13)==1) printf("13");
   else if(digitalRead(19)==1)printf("19");
   else if(digitalRead(26)==1)printf("26");

   return 0;
}
