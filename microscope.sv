//////////////////////////////////////////////
// E155 Final Project: MicrOscope
// I. Martos-Repath & A. Echeverria
// First written on 18 November 2017
// Updated 20 November 2017
//////////////////////////////////////////////

/////////////////////////
// Pin Assignments!
// Since we are using a different ADC from what is on the MuddPi
// (an MCP3002), we need to assign some of the pins on the MuddPi
// to correspond with our ADC pins. Can also find this on the schematic.
// clk (ADC Pin 7) = ADC_sclk (FPGA Pin 30)
// Data (ADC Pin 6) = ADC_MISO (FPGA Pin 31)
// Conv (ADC Pin 5) = ADC_MOSI (FPGA Pin 32)
// FPGA pins...
// clk (from onboard oscillator) is Pin 88
// output codes on Pin 34
// reset on Pin 38
/////////////////////////

/////////////////////////
// ADC_FPGA_SPImaster module
// Function: FPGA (acting as master) gets output codes from ADC (acting as slave)
/////////////////////////
module microscope(input logic clk, reset, ADC_MISO,					//for testing SPI, called microscope, rename to ADC_FPGA_SPImaster
			output logic ADC_sclk, ADC_MOSI, outputcode);
	
	logic conv; //needs to go low to start collecting from ADC
	logic [4:0]slowcount; //counter for negative edges of sclk
	logic [31:0]counter; //counter for clock divider to make sclk
	
	typedef enum logic [1:0] {S0,S1,S2,S3} statetype; //logic for state machine
	statetype state, nextstate;
	
	//logic to generate sclk to pass to ADC slave
	always_ff@(posedge clk, posedge reset)
		if(reset) counter <= 32'b0;
		else begin
		counter <= counter + 1;
		end
	assign ADC_sclk = counter[4];
	
	//establishes counter for FSM
	always_ff@(negedge ADC_sclk)
		if(conv) slowcount <= 1;
		else slowcount = slowcount + 5'b1;
		
	//state register
	always_ff@(negedge ADC_sclk, posedge reset)
		if(reset) state <= S0;
		else state <= nextstate;
		
	//FSM for the SPI timing diagram
	always_comb
		case(state)
			S0: if(~conv) nextstate = S1;
				 else nextstate = S0;
			S1: if(slowcount==5'b00010) nextstate = S2;
				 else nextstate = S1;
			S2: if(slowcount==5'b01110) nextstate = S3;
				 else nextstate = S2;
			S3: if(slowcount==5'b10000) nextstate = S0;
				 else nextstate = S3;
			default: nextstate = S0;
		endcase
				 
	//output logic
	assign en = (state==S2); //enable to read DATA
	assign conv = (slowcount==5'b10000); //once slowcount is 16, drive conv high again
	
	//create a shift register
	logic [11:0]shiftReg;
	
	//this shift register is where ADC will be shifting bits into
	always_ff@(posedge ADC_sclk, posedge reset)
		if(reset) shiftReg<= 0;
		else if(en)
			begin
				outputcode <= shiftReg[11];
				shiftReg <= shiftReg << 1;
				shiftReg[0] <= ADC_MISO;
			  end
		else shiftReg <= shiftReg;
	
	//signal from master FPGA to slave ADC is conv
	assign ADC_MOSI = conv;

endmodule

//the slave will have its MOSI as conv, which when written low causes the ADC
//to send output codes back over MISO