module reg_flags (inp, control, clk, out);

	input [5:0] inp;
	input control, clk;
	output reg [5:0] out;
	
	always @(posedge clk) begin
		if (control) begin
			out = inp;
		end
	end

endmodule


module reg_flags_tb;

	reg [5:0] inp;
	reg control, clk;
	wire [5:0] out;

	reg_flags DUT(inp, control, clk, out);

	initial begin
		clk = 0;
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

	always #5 clk = ~clk;

endmodule
