//////////////////////////////////////////////
// E155 Final Project: MicrOscope
// I. Martos-Repath & A. Echeverria
// First written on 18 November 2017
// Updated 4 December 2017
//////////////////////////////////////////////

/////////////////////////
// ADC_FPGA_SPImaster module
// Function: FPGA (acting as master) gets output codes from ADC (acting as slave)
/////////////////////////
module microscope(input logic clk, reset, ADC0_MISO,ADC1_MISO,pi_sclk,pi_MOSI,
						input logic [5:0]timeScale,
						input logic [2:0]voltsScale,
						input logic [1:0]filterSelect,
						output logic [7:0]led,
						output logic ADC0_sclk, ADC0_MOSI,ADC1_sclk, ADC1_MOSI,pi_MISO,empty);

	logic [7:0]scopeOut,signalOutWide;
	logic [7:0]untriggeredSignal;
	logic [7:0]q,d;
	logic [11:0]scopeIn;
	logic newNumber;
	logic ADC_sclk;
	logic [11:0]trigLevel;
	
	
	logic [31:0]counter; //counter for clock divider to make sclk
	//logic to generate sclk to pass to ADC slave
	always_ff@(posedge clk, posedge reset) begin
		if(reset) counter <= 32'b0;
		else begin
		counter <= counter + 1;
		end
	end
	assign led = trigLevel[11:4];
	assign ADC_sclk = counter[4];
	assign ADC0_sclk  = ADC_sclk;
	assign ADC1_sclk  = ADC_sclk;
	
	ADC triggerLevel(ADC_sclk,reset,ADC0_MISO,ADC0_MOSI,trigLevel);
	ADC channel1(ADC_sclk,reset,ADC1_MISO,ADC1_MOSI,scopeIn,newNumber);
	filter lowOrNot(newNumber,reset,scopeIn[11:4],filterSelect,untriggeredSignal);
	trig trigger(untriggeredSignal,trigLevel[11:4],timeScale,ADC_sclk,reset,scopeOut);
	voltsScale scaleOutput(voltsScale,scopeOut,signalOutWide);
	
	pi(pi_sclk,reset,pi_MOSI,pi_MISO,d,q);
	
	//pi(pi_sclk,reset,pi_MOSI,pi_MISO,8'b11000100,q);
	
	logic full;
	logic writeEn_in;
	assign writeEn_in = 1;
	
	   aFifo buffer(d, empty, pi_MOSI, pi_sclk, signalOutWide,full, writeEn_in,newNumber,reset);

endmodule
