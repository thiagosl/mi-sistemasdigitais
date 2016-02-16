module reg_flags (inp, control, out);

	input [5:0] inp;
	input control;
	output reg [5:0] out;
	
	always @(inp or control) begin
		if (control) begin
			out = inp;
		end
	end

endmodule


module reg_flags_tb;

	reg [5:0] inp;
	reg control;
	wire [5:0] out;

	reg_flags DUT(inp, control, out);

	initial begin
		control = 1'b1;
		inp = 6'b010101;
		#20;
		inp = 6'b111111;
		#20;
		control = 1'b0;
		inp = 6'b000000;
		#20;
		inp = 6'b101010;
		#20;
		$finish;
	end

endmodule
