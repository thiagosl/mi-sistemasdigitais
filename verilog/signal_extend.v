module signal_extend (inp, out);

	input [15:0] inp;
	output reg [31:0] out;
	
	always @(inp) begin
		case(inp[15])
			1'b0: out = {16'b0000000000000000, inp};
			1'b1: out = {16'b1111111111111111, inp};
		endcase
	end
endmodule