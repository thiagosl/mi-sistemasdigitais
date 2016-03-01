module pc (inp, clk, reset, out);

	input [31:0] inp;
	input clk, reset;
	output reg [31:0] out;
	
	always @(posedge clk) begin
		out = inp;
	end
	always @(reset) begin
		if (reset == 1) begin
			out = 0;
		end
	end
endmodule


module pc_tb;

	reg [31:0] inp;
	reg clk, reset;
	wire [31:0] out;

	pc DUT(inp, clk, reset, out);

	initial begin
		clk = 0;
		reset = 0;
		inp = 32'b01010101010101010101010101010101;
		#17;
		inp = 32'b11111111111111111111111111111111;
		#20;
		reset = 1;
		#20;
		$finish;
	end

	always #5 clk = ~clk;

endmodule
