module mux3_32b (input0, input1, input2, control, out);

	input [31:0] input0, input1, input2;
	input [1:0] control;
	output reg [31:0] out;
	
	always @(control or input0 or input1 or input2) begin
		case(control)
			2'b00: out = input0;
			2'b01: out = input1;
			2'b10: out = input2;
		endcase
	end
endmodule