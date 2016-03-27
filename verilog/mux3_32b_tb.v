module mux3_32b_tb;

	reg [31:0] input0, input1, input2;
	reg [1:0] control;
	wire [31:0] out;

	mux3_32b DUT(input0, input1, input2, control, out);

	initial begin
		input0 = 32'b01010101010101010101010101010101;
		input1 = 32'b00000000000000001111111111111111;
		input2 = 32'b11111111111111110000000000000000;
		control = 2'b00;
		#20;
		control = 2'b01;
		#20;
		control = 2'b10;
		#20;
		control = 2'b00;
		#20;
		$finish;
	end

endmodule
