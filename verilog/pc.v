module pc (inp, out);

	input [31:0] inp;
	output reg [31:0] out;
	
	always @(inp) begin
		out = inp;
	end

endmodule


module pc_tb;

	reg [31:0] inp;
	wire [31:0] out;

	pc DUT(inp, out);

	initial begin
		inp = 32'b01010101010101010101010101010101;
		#20;
		inp = 32'b11111111111111111111111111111111;
		#20;
		$finish;
	end

endmodule
