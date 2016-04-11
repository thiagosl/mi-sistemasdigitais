module mux4_32b_tb;

	reg [31:0] input0, input1, input2, input3;
	reg [1:0] control;
	wire [31:0] out;

	mux4_32b DUT(input0, input1, input2, input3, control, out);

	initial begin
		input0 = 32'b01010101010101010101010101010101;
		input1 = 32'b00000000000000001111111111111111;
		input2 = 32'b11111111111111110000000000000000;
		input3 = 32'b11111111111111111111111111111111;
		control = 2'b00;
		#20;
		control = 2'b01;
		#20;
		control = 2'b10;
		#20;
		control = 2'b11;
		#20;
		control = 2'b00;
		#20;
		$finish;
	end

endmodule
