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

       pinMode(12,INPUT);//delta = 0.05 //use for DC values
       pinMode(16,INPUT);//delta = 0.5
       pinMode(20,INPUT);//delta = 1.5
       pinMode(21,INPUT);//delta = 4.5
}


 int main(void){
 
   float voltage;
   char data;
   int empty = 0;
   /*Intialize PIO*/
   pioInit();
   spiInit(8440000,0);
   pinMode(23,INPUT);
   int timeScale[6];
   int delta[4];
   int timeCountTrue;
   int timeCount = 0;
   float deltaT = 0.0000128;
   float timeToPlot = 0;
   float lastValue = 0;
   float deltaTolerance = 0;
   setPins();
   while(1){
	timeCount = 0;
	if(timeCountTrue == 1){
		 printf("Done with one round of measurements\nStarting over...\n");
		 return 0;
		 timeCountTrue = 0;
	}
	while(timeCountTrue == 0){
		while(empty == 1){
			empty = digitalRead(23);
			printf("The buffer is empty\n");
		}
		//Update sensitivity
		delta[0] = digitalRead(12);
		printf("delta[0] is %d\n",delta[0]);
		delta[1] = digitalRead(16);
		delta[2] = digitalRead(20);
		delta[3] = digitalRead(21);

		if(delta[0] == 1) deltaTolerance = 0.05;
		else if(delta[1] == 1) deltaTolerance = 0.5;
		else if(delta[2] == 1) deltaTolerance = 1.5;
		else if(delta[3] == 1) deltaTolerance = 4.5;
		else deltaTolerance = 10;

		lastValue = voltage;
		data =  (spiSendReceive('1'));
    		voltage = 1.3*data*0.01960784313;
		if(timeCount<5) voltage = voltage;
		else{
			if((lastValue-voltage)> deltaTolerance) voltage = lastValue;
			if((voltage-lastValue)> deltaTolerance) voltage = lastValue;
		}
		printf("voltage is %f\n",voltage);
   		printf("data is %d\n",data);
		timeScale[2] = digitalRead(27);
   		//Check timeScale
	   	  if(timeScale[5] == 1){
	   	  	timeCountTrue = (timeCount > 4);
	   	  }
	   	  else if(timeScale[4] == 1){
	   	  	timeCountTrue = (timeCount > 8);
	   	  }
	   	  else if(timeScale[3] == 1){
	   	  	timeCountTrue = (timeCount > 79);
	   	  }
	   	  else if(timeScale[2] == 1){
	   	  	timeCountTrue = (timeCount > 782);
	   	 	printf("timeCountTrue is %d\n",timeCountTrue);
		  }
	   	  else if(timeScale[1] == 1){
	   	  	timeCountTrue = (timeCount > 7813);
	   	  }
	   	  else if(timeScale[0] == 1){
			timeCountTrue = (timeCount > 78125);
	        	}
		timeToPlot = timeCount*deltaT;
		timeCount = timeCount + 1;
		printf("timeCount is %d\n",timeCount);
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
		   printf("timeScale[2] is %d\n",timeScale[2]);

	}
   }
   return 0;
}
