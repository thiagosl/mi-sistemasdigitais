module mux3_4b_tb;

	reg [3:0] input0, input1, input2;
	reg [1:0] control;
	wire [3:0] out;

	mux3_4b DUT(input0, input1, input2, control, out);

	initial begin
		input0 = 4'b0000;
		input1 = 4'b0101;
		input2 = 4'b1111;
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
