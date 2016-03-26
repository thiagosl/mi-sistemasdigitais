module mux4_4b (input0, input1, input2, input3, control, out);

	input [3:0] input0, input1, input2, input3;
	input [1:0] control;
	output reg [3:0] out;
	
	always @(control or input0 or input1 or input2 or input3) begin
		case(control)
			2'b00: out = input0;
			2'b01: out = input1;
			2'b10: out = input2;
			2'b11: out = input3;
		endcase
	end
endmodule
