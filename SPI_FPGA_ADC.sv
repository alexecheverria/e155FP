//////////////////////////////////////////////
// E155 Final Project: MicrOscope
// I. Martos-Repath & A. Echeverria
// 18 November 2017
// Implementation of SPI b/w ADC (p/n ADS7818) and FPGA (on MuddPi)
//////////////////////////////////////////////

/////////////////////////
// Pin Assignments!
// Since we are using a different ADC from what is on the MuddPi
// (an MCP3002), we need to assign some of the pins on the MuddPi
// to correspond with our ADC pins.
// clk (ADC Pin 7) = ADC_sclk (FPGA Pin 30)
// Data (ADC Pin 6) = ADC_MISO (FPGA Pin 31) //Does this need to be tied to a serial input data pin? Where are those?
// Conv (ADC Pin 5) = ADC_MOSI (FPGA Pin 32)
/////////////////////////

/////////////////////////
// ADC_FPGA_SPImaster module
// FPGA (acting as master) gets output codes from ADC (acting as slave)
/////////////////////////
module ADC_FPGA_SPImaster(input logic clk, reset, ADC_MISO,
			output logic ADC_sclk, ADC_MOSI, [11:0] outputcode);

	logic [n:0] counter; //how big does counter need to be?
	always_ff@(posedge clk, posedge reset)
		if(reset) counter <= 0;
		else counter <= counter + n’b1;
	assign ADC_sclk = counter[44];

	//conv on ADC needs to be brought low; is attached to ADC_MOSI
	always_ff@(posedge clk, posedge reset)
		if(reset) outputcode <= 12’b0; 
		else //does conv need to be 0? do something else to output code

	assign ADC_MOSI = ??

endmodule

// delete at end: clocking info https://eewiki.net/pages/viewpage.action?pageId=4096096

/////////////////////////
// ADC_FPGA_SPIslave module
// upon request, ADC slave sends output codes to FPGA master
/////////////////////////
module ADC_FPGA_SPIslave(input ADC_sclk, ADC_MOSI, reset, [11:0]d
			output logic ADC_MISO, [11:0]q);
		//d is data to send, q is data received (by FPGA master?)

	logic [3:0] count;
	logic qdelay;

	//when counter reaches 12, all bits ready to transmit
	always_ff@(negedge ADC_sclk,posed reset)
		if(reset) count = 0;
		else count = count + 4’b1;

	//creates a shift register with d at the start, then shifts MOSI into bottom
	always_ff@(posedge ADC_sclk) //check for correct clock edge
		q <= (count == 0) ? {d[10:0],ADC_MOSI}:{q[11:0],ADC_MOSI};

	//load d at start (when count is 0)
	always_ff@(negedge ADC_sclk)
		qdelay = q[11];
		assign ADC_MISO = (count==0) ? d[11]:qdelay;
		// now output code ought to be in ADC_MISO

endmodule
