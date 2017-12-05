module pi(input logic sclk, reset, //from Pi master
				input logic pi_MOSI,
				output logic pi_MISO,
				input logic [7:0]d, //voltage to send
				output logic [7:0]q); //voltage sent
				
	logic [2:0]counter; //need to be able to count up to 8
	logic qdelayed;
	
	always_ff@(negedge sclk, posedge reset)
		if(reset) counter <= 0;
		else counter <= counter + 3'b1;
		
	always_ff@(posedge sclk)
		q <= (counter == 0)? {d[6:0], pi_MOSI}: {q[6:0], pi_MOSI};
		
	always_ff@(negedge sclk)
		qdelayed = q[7];
	
	assign pi_MISO = (counter == 0) ? d[7] : qdelayed;
endmodule



