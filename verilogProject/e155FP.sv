//-----------------------------------------------------
// Design Name : E155 Final Project
// File Name   : e155FP
// Function    : Verilog File for Osciolloscope Project
//-----------------------------------------------------

/*
  Created: 9/1/2017
  Name: Isabel Martos-Repath and Alex Echeverria
  Email: imartosrepath@g.hmc.edu aecheverria@g.hmc.edu
  Purpose: This is the top level module designed to run osciolloscope
  functionality on FPGA
*/
module  e155FP   (
input logic clk, // the 40 MHz clock
input logic MISO,
output logic MOSI,
output logic CS,
output logic sclk, //clock for ADC
output logic [7:0]led ,// the 8 lights on the LED bar=
output logic oscopeOut //signal sent to the 
);


/*Logic to setup slow clock for the ADC*/
reg [32:0] counter;
assign sclk = counter[23]; // <------ data to change


always @ (posedge clk) begin
	 counter <= counter + 1;
end

endmodule 


