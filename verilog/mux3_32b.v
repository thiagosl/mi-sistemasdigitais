module mux3_32b (input0, input1, input2, control, out);

	input [3:0] input0, input1, input2;
	input [1:0] control;
	output reg [3:0] out;
	
	always @(control or input0 or input1 or input2) begin
		case(control)
			2'b00: out = input0;
			2'b01: out = input1;
			2'b10: out = input2;
		endcase
	end

endmodule


module mux3_32b_tb;

	reg [3:0] input0, input1, input2;
	reg [1:0] control;
	wire [3:0] out;

	mux3_32b DUT(input0, input1, input2, control, out);

	initial begin
		input0 = 32'b0000;
		input1 = 32'b0101;
		input2 = 32'b1111;
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
