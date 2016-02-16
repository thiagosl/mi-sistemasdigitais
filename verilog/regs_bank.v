module reg_flags (inpA, inpB, inpC, control, data, outA, outB);

	input [3:0] inpA, inpB, inpC;
	input [1:0] control;
	input [31:0] data;
	output reg [31:0] outA, outB, outC;
	reg [31:0] regBank[31:0];
	
	always @(inp or control) begin
		if (control) begin
			out = inp;
		end
	end

endmodule


module reg_flags_tb;

	reg [6:0] inp;
	reg control;
	wire [6:0] out;

	reg_flags DUT(inp, control, out);

	initial begin
		control = 1;
		inp = 6'b010101;
		#20;
		inp = 6'b111111;
		#20;
		control = 0;
		inp = 6'b000000;
		#20;
		inp = 6'b101010;
		#20;
		$finish;
	end

endmodule
