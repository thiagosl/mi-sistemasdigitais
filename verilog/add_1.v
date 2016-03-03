module add_1 (inp, out);

	input [31:0] inp;
	output reg [31:0] out;

	always @(inp) begin
		out = inp + 1;
	end

endmodule
