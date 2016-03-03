module adress_extend (inp, out);

	input [11:0] inp;
	output reg [31:0] out;
	
	always @(inp) begin
		out = {20'b00000000000000000000, inp};
	end

endmodule