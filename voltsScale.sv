module voltsScale(input logic [2:0]voltsScale,
					  input logic [7:0]signalIn,
					  output logic [7:0]signalOut);
					
					
					
always_comb
begin
	case(voltsScale)
			3'b001 : signalOut = signalIn;
			3'b010 : begin
						if(signalIn > 12'd3) signalOut = 12'd3;
						else signalOut = signalIn;
						end
			3'b100 : begin
						if(signalIn < 12'd3) signalOut = 0;
						else signalOut = signalIn;
						end
			default : signalOut = signalIn;
	endcase
end

endmodule 
