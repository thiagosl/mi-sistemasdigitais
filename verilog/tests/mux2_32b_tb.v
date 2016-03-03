module mux2_32b_tb;

	reg [31:0] input0, input1;
	reg control;
	wire [31:0] out;

	mux2_32b DUT(input0, input1, control, out);

	initial begin
		input0 = 32'b01010101010101010101010101010101;
		input1 = 32'b00000000000000001111111111111111;
		control = 1'b0;
		#20;
		control = 1'b1;
		#20;
		control = 1'b0;
		#20;
		$finish;
	end

endmodule
