module trig(input logic [7:0]scopeIn,trigLevel,
				input logic [5:0]timeScale,
				input logic clk,reset,
				output logic [7:0]scopeOut);


				
logic [7:0]scopeInTenX;

assign scopeInTenX = scopeIn;

logic [31:0] writeCount;
logic [31:0] timeScaleCount;

always_comb
begin
	case(timeScale)
			6'b000001 : timeScaleCount = 32'd100000;
			6'b000010 : timeScaleCount = 32'd10000;
			6'b000100 : timeScaleCount = 32'd1000;
			6'b001000 : timeScaleCount = 32'd100;
			6'b010000 : timeScaleCount = 32'd25;
			6'b100000 : timeScaleCount = 32'd15;
			default : timeScaleCount = 32'd200;
	endcase
end

always_ff@(posedge clk, posedge reset) begin
	if(reset) begin
		scopeOut = 0;
		writeCount <= 32'b0;
	end
	else begin
		if((scopeInTenX >= trigLevel)&&(writeCount==32'b0)) begin
			scopeOut <= scopeInTenX;
			writeCount <= writeCount + 1;
		end
		else if((writeCount != 32'd0)&& (writeCount < timeScaleCount)) begin
		writeCount <= writeCount + 1;
		scopeOut <= scopeInTenX;
		end
		else begin
			writeCount <= 0;
			scopeOut <= 0;
		end
	end
end
			
			

endmodule 