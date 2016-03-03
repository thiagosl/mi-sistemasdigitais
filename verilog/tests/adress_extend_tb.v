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

