module add_4 (inp, out);

	input [31:0] inp;
	output reg [31:0] out;

	always @(inp) begin
		out = inp + 4;
	end

endmodule


module add_4_tb;

	reg [31:0] inp;
	wire [31:0] out;

	add_4 DUT(inp, out);

	initial begin
		inp = 32'b00000000000000000000000000000001;
		#20;
		inp = 32'b11111111111111111111111111111111;
		#20;
		$finish;
	end

endmodule
