module reg_estagio (inp, clk, out);

	input [31:0] inp;
	input clk;
	output reg [31:0] out;
	
	always @(posedge clk) begin
		out = inp;
	end

endmodule


module reg_estagio_tb;

	reg [31:0] inp;
	reg clk;
	wire [31:0] out;

	reg_estagio DUT(inp, out);

	initial begin
		
		inp = 32'b01010101010101010101010101010101;
		#17;
		inp = 32'b11111111111111111111111111111111;
		#20;
		$finish;
	end
	
	always #5 clk = ~clk;

endmodule
