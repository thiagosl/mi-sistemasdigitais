module signal_extend_tb;

	reg [15:0] inp;
	wire [31:0] out;

	signal_extend DUT(inp, out);

	initial begin
		inp = 16'b0101010101010101;
		#20;
		inp = 16'b1010101010101010;
		#20;
		inp = 16'b0111111111111111;
		#20;
		inp = 16'b1000000000000000;
		#20;
		$finish;
	end

endmodule
