module mux2_32b (input0, input1, control, out);

	input [31:0] input0, input1;
	input control;
	output reg [31:0] out;
	
	always @(control or input0 or input1) begin
		if(control) begin
			out = input1;
		end else begin
			out = input0;
		end
	end

endmodule

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
