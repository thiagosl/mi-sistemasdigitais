module add_1_tb;

	reg [31:0] inp;
	wire [31:0] out;

	add_1 DUT(inp, out);

	initial begin
		inp = 32'b00000000000000000000000000000001;
		#20;
		inp = 32'b11111111111111111111111111111111;
		#20;
		$finish;
	end

endmodule
