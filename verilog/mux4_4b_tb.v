module mux4_4b_tb;

	reg [3:0] input0, input1, input2, input3;
	reg [1:0] control;
	wire [3:0] out;

	mux4_4b DUT(input0, input1, input2, input3, control, out);

	initial begin
		input0 = 4'b0101;
		input1 = 4'b0000;
		input2 = 4'b1111;
		input3 = 4'b0011;
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
