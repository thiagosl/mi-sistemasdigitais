module mux3_32b_PC_tb;

	reg [31:0] input0, input1, input2;
	reg [2:0] control;
	wire [31:0] out;

	mux3_32b_PC DUT(input0, input1, input2, control, out);

	initial begin
		input0 = 32'b00000000000000000000000000000000;
		input1 = 32'b01010101010101010101010101010101;
		input2 = 32'b11111111111111110000000000000000;
		control = 3'b000;
		#10;
		control = 3'b001;
		#10;
		control = 3'b010;
		#10;
		control = 3'b011;
		#10;
		control = 3'b100;
		#10;
		control = 3'b101;
		#10;
		control = 3'b000;
		#10;
		$finish;
	end

endmodule
