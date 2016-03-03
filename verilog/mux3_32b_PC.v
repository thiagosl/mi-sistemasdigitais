module mux3_32b_PC (input0, input1, input2, control, out);

	input [31:0] input0, input1, input2;
	input [2:0] control;
	output reg [31:0] out;
	
	always @(control or input0 or input1 or input2) begin
		case(control[2:1])
			2'b00: out = input0;
			2'b10: out = input2;
			2'b01: begin
				if (control[0] == 0) begin
					out = input0;
				end else begin
					out = input1;
				end
			end
			2'b11: out = input1;
		endcase
	end

endmodule


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
