#include <stdio.h>


void updatePins(int timeScale[], int newNumber,int dataPi,int evenOrOdd){
	   //TimeScale Pins
	   pinMode(4,INPUT); 
       pinMode(17,INPUT); 
       pinMode(27,INPUT); 
       pinMode(22,INPUT); 
       pinMode(5,INPUT); 
       pinMode(6,INPUT);
        
       pinMode(24,INPUT); //newNumber
       pinMode(18,INPUT); //dataPi
       pinMode(23,INPUT); //evenOrOdd

	   timeScale[0] = digitalRead(4);
	   timeScale[1] = digitalRead(17);
	   timeScale[2] = digitalRead(27);
	   timeScale[4] = digitalRead(22);
	   timeScale[5] = digitalRead(5);
	   timeScale[6] = digitalRead(6);


       newNumber = digitalRead(24);
       dataPi = digitalRead(18);
       evenOrOdd = digitalRead(23);

}


 int  main(){
   


   /*Intialize PIO*/
   pioInit();
   

   /*Time Scale Logic*/
   bool timeScale[6]; // [x volts/div, y volts div, 1 volt/div
 

   bool evenOrOdd;
   bool nextBit;
   bool dataPi;
   bool newNumber;


   float dataToPlot;
   float timeToPlot;
   float conversionFactor = 5/4096;

   int counter = 12;
   int timeCount = 0;
   bool timeCountTrue = 0;


   float delta t0.0000128;

   while(true){
   	   
   	 
   	   //log data in .data
   	  while(timeCountTrue){
   	  		
	   	  		  //get data from FPGA
	   	   for(counter = 12; counter > 0, counter = counter-1){
	   	   		updatePins(timeScale,voltsScale,newNumber,dataPi,evenOrOdd);
	   	   		//Turn serial data to number
	   	   		dataToPlot = (2**(counter+1)*dataPi)*conversionFactor;
	   			nextBit = evenOrOdd
	   			//Wait for next bit
	   			while(nextBit == evenOrOdd){
	   				//update new pins
	   				updatePins(timeScale,newNumber,dataPi,evenOrOdd);
	   			}
	   	  }
	   }

   	  	  //Check timeScale
	   	  if(timeScale[6] == 1){
	   	  	timeCountTrue = (timeCount == 78125);
	   	  }
	   	  else if(timeScale[5] == 1){
	   	  	timeCountTrue = !(timeCount == 78125);
	   	  }
	   	  else if(timeScale[4] == 1){
	   	  	timeCountTrue = !(timeCount == 7813);
	   	  }
	   	  else if(timeScale[3] == 1){
	   	  	timeCountTrue = !(timeCount == 792);
	   	  }
	   	  else if(timeScale[2] == 1){
	   	  	timeCountTrue = !(timeCount == 78);
	   	  }
	   	  else if(timeScale[1] == 1){
	   	  	timeCountTrue = !(timeCount == 8);
	   	  }
	   	  else if(timeScale[0] == 1){
			timeCountTrue = !(timeCount == 1);

			timeToPlot = timeCount*deltaT;
			timeCount = timeCount + 1;
			FILE * fp;
			/* open the file for writing*/
			fp = fopen ("values.data","a");
			fprintf (fp, "%f %f\n",timeToPlot,dataToPlot);
			/* close the file*/  
			fclose (fp);
   	  
   }



   return 0;
}