module adress_extend (inp, out);

	input [11:0] inp;
	output reg [31:0] out;
	
	always @(inp) begin
		out = {20'b00000000000000000000, inp};
	end

endmodule


module adress_extend_tb;

	reg [11:0] inp;
	wire [31:0] out;

	adress_extend DUT(inp, out);

	initial begin
		inp = 12'b010101010101;
		#20;
		inp = 12'b101010101010;
		#20;
		inp = 12'b011111111111;
		#20;
		inp = 12'b100000000000;
		#20;
		$finish;
	end

endmodule
