module filter(input logic clk, reset, 
				  input logic [7:0]signalIn,
				  input logic [1:0]filterChoice,
				  output logic [7:0]signalOut);

logic [7:0]signalLowFilt;
logic [7:0]signalHighFilt; 


always_comb
begin
	case(filterChoice)
			2'b00 : signalOut = signalIn;
			2'b10 : signalOut = signalLowFilt;
			2'b01 : signalOut = signalHighFilt;
			default : signalOut = signalIn;
	endcase
end




//Low Pass Filter 
logic [63:0]alphaL;
assign alphaL = 0.1; //Gives us corner frequency of 40027.70716
logic [7:0]signalLowFiltPrevious;

always_ff@(posedge clk, posedge reset) begin
		if(reset) signalLowFilt <= 0;
		else	begin
			signalLowFilt <= signalLowFiltPrevious+(alphaL*(signalIn-signalLowFiltPrevious));
			signalLowFiltPrevious <= signalLowFilt;
		end
	end

//High Pass filter
logic [63:0]alphaH;
assign alphaH =	0.9; //Gives us corner frequency of 40027.70716
logic [7:0]signalHighFiltPrevious;
logic [7:0]signalInPrevious;


always_ff@(posedge clk, posedge reset) begin
		if(reset) begin
			signalHighFilt <= 0;
			signalInPrevious <= 0;
		end
		else begin
			signalHighFilt <= alphaH*(signalHighFiltPrevious+signalIn-signalInPrevious);
			signalInPrevious <= signalIn;
			signalHighFiltPrevious <= signalHighFilt;
		end
end


endmodule 