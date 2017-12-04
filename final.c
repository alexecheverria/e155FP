#include <stdio.h>
#include "EasyPIO.h"

void setPins(){
	//TimeScale Pins
       pinMode(4,INPUT); 
       pinMode(17,INPUT); 
       pinMode(27,INPUT); 
       pinMode(22,INPUT); 
       pinMode(5,INPUT); 
       pinMode(6,INPUT);
       pinMode(24,INPUT); //newNumber
       pinMode(18,INPUT); //dataPi
       pinMode(23,OUTPUT); //piRequest

}


 int main(void){
 
   float voltage;
   char data;
   int empty = 0;
   /*Intialize PIO*/
   pioInit();
   spiInit(6440000,0);
   pinMode(23,INPUT);
   int timeScale[6];
   int timeCountTrue;
   int timeCount = 0;
   float deltaT = 0.0000128;
   float timeToPlot = 0;
   setPins();
   while(1){
	timeCount = 0;
	if(timeCountTrue == 1){
		 printf("Done with one round of measurements\nStarting over...\n");
		 timeCountTrue = 0;
	}
	while(timeCountTrue == 0){
		while(empty == 1){
			empty = digitalRead(23);
			printf("The buffer is empty\n");
		}

		data =  (spiSendReceive('1'));
    		voltage =  data*0.01960784313;
		printf("voltage is %f\n",voltage);
   		printf("data is %d\n",data);
   		//Check timeScale
	   	  if(timeScale[5] == 1){
	   	  	timeCountTrue = (timeCount == 312500);
	   	  }
	   	  else if(timeScale[4] == 1){
	   	  	timeCountTrue = !(timeCount == 31250);
	   	  }
	   	  else if(timeScale[3] == 1){
	   	  	timeCountTrue = !(timeCount == 3125);
	   	  }
	   	  else if(timeScale[2] == 1){
	   	  	timeCountTrue = !(timeCount == 313);
	   	  }
	   	  else if(timeScale[1] == 1){
	   	  	timeCountTrue = !(timeCount == 32);
	   	  }
	   	  else if(timeScale[0] == 1){
			timeCountTrue = !(timeCount == 8);
	        	}
		timeToPlot = timeCount*deltaT;
		timeCount = timeCount + 1;
		FILE * fp;
		// open the file for writing
		if(timeCount < 2) fp = fopen("values.data","w");
		else fp = fopen ("values.data","a");
		fprintf (fp, "%f %f\n",timeToPlot,voltage);
		// close the file
		fclose (fp);

		   timeScale[0] = digitalRead(4);
		   timeScale[1] = digitalRead(17);
		   timeScale[2] = digitalRead(27);
		   timeScale[3] = digitalRead(22);
		   timeScale[4] = digitalRead(5);
		   timeScale[5] = digitalRead(6);

	}
   }
   return 0;
}
