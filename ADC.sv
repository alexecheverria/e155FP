module ADC(input logic ADC_sclk,reset, ADC_MISO,
			  output logic ADC_MOSI,
			  output logic [11:0]outputcode,
			  output logic newNumber);

   logic conv; //needs to go low to start collecting from ADC
	logic [3:0]count; //counter for negative edges of sclk
	logic en;
	logic [11:0]datatmp;
	
	//The clock counter starts at 0, so clock is from 0 to 15 instead of 1-16
	always_ff@(posedge ADC_sclk, posedge reset)
		begin	
			if(reset) count <= 0;
			else	count <= count + 1;
		end
		
	//Assert the CONV signal
	always_ff@(negedge ADC_sclk, posedge reset)
		begin
			if(reset) conv <= 0;
			else begin
				if((count==4'd14)||(count==4'd15)) conv <= 0;
				else conv <= 1;
			end
		end
		
	//Read the serial data into a 12-bit register 
	
	always_ff@(negedge ADC_sclk, posedge reset)
		begin
			if(reset) outputcode <= 0;
			else begin
				datatmp <= {datatmp[10:0],ADC_MISO};
				if(count == 4'd14) 
				begin
					newNumber <= 1;
					outputcode <=datatmp;
				end
				else newNumber <= 0;
			end
		end
		
	assign ADC_MOSI = conv;
	/*
	
	typedef enum logic [1:0] {S0,S1,S2} statetype; //logic for state machine
	statetype state, nextstate;
	
	
	//establishes counter for FSM
	always_ff@(negedge ADC_sclk, posedge reset)
		if(reset) slowcount <= 1;
		else begin
			if(!conv) slowcount <= 1;
			else slowcount = slowcount + 5'b1;
		end
		
	//state register
	always_ff@(posedge ADC_sclk, posedge reset)
		if(reset) state <= S0;
		else state <= nextstate;
		
	//FSM for the SPI timing diagram
	always_comb
		case(state)
			S0: if(conv&&(slowcount == 1)) nextstate = S1;
				 else nextstate = S0;
			S1: if(slowcount==5'b01000) nextstate = S0;
				 else nextstate = S1;
			default: nextstate = S0;
		endcase
				 
	//output logic
	assign en = (state==S1); //enable to read DATA
	assign conv = !((slowcount==5'b01000)); //once slowcount is 15, drive conv low
	assign newNumber = (slowcount == 5'b01000); //assign newNumber as high when slowcount reaches 13
	
	
	//this shift register is where ADC will be shifting bits into
	always_ff@(negedge ADC_sclk, posedge reset)
		if(reset) outputcode<= 0;
		else if(en)
			begin
				outputcode <= outputcode << 1;
				outputcode[0] <= ADC_MISO;
			  end
		else outputcode <= outputcode;
	
	//signal from master FPGA to slave ADC is conv
	assign ADC_MOSI = conv;
	*/
endmodule 
/*case(count)
					1: outputcode[11] <= ADC_MISO;
					2: outputcode[10] <= ADC_MISO;
					3: outputcode[9] <= ADC_MISO;
					4: outputcode[8] <= ADC_MISO;
					5: outputcode[7] <= ADC_MISO;
					6: outputcode[6] <= ADC_MISO;
					7: outputcode[5] <= ADC_MISO;
					8: outputcode[4] <= ADC_MISO;
					9: outputcode[3] <= ADC_MISO;
					10: outputcode[2] <= ADC_MISO;
					11: outputcode[1] <= ADC_MISO;
					12: outputcode[0] <= ADC_MISO;
					13: ADC_MOSI <= outputcode;
					14: ADC_MOSI <= outputcode;
				endcase*/

//the slave will have its MOSI as conv, which when written low causes the ADC
//to send output codes back over MISO