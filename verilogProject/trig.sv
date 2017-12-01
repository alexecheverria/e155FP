module trig(input logic [15:0]scopeIn,trigLevel,
				input logic [5:0]timeScale,
				input logic clk,reset,
				output logic full,empty,rdPi
				output logic [15:0]scopeOut);

				
logic wr;

assign scopeIn10X = scopeIn*10;

logic [31:0] writeCount;
logic [31:0] timeScaleCount;

always_comb
begin
	case(timeScale)
			6'b000001 : timeScaleCount = 32'd1250000;
			6'b000010 : timeScaleCount = 32'd125000;
			6'b000100 : timeScaleCount = 32'd12500;
			6'b001000 : timeScaleCount = 32'd1250;
			6'b010000 : timeScaleCount = 32'd125;
			6'b100000 : timeScaleCount = 32'd16;
			default : timeScaleCount = 32'd10;
	endcase
end

always_ff@(posedge clk, posedge reset) begin
	if(reset) begin
		wr <= 0;
		writeCount <= 32'b0;
	end
	else begin
		if((scopeIn10X >= trigLevel)&&(writeCount==32'b0)) begin
			wr <= 1;
			writeCount <= writeCount + 1;
		end
		else if((writeCount != 32'd0)&& (writeCount < timeScaleCount)) begin
		writeCount <= writeCount + 1;
		end
		else begin
			writeCount <= 0;
			wr <= 0;
		end
	end
end
			

sync_fifo buffer(scopeOut,full,empty,scopeIn10X,clk,reset,wr,rdPi);


endmodule 

