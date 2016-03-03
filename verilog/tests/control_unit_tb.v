module control_unit_tb;

	reg [23:0] code;
	wire [1:0] mux1, mux5, mux6;
	wire [2:0] regs_bank;
	wire mux2, mux4, data_mem, comparator, reg_flags;
	wire [5:0] alu;

	control_unit DUT(code, mux1, mux2, mux4, mux5, mux6, regs_bank, data_mem, alu, reg_flags, comparator);

	initial begin
		code = 24'b000000000000000100110000;
		#20;
		code = 24'b000000000000000010010000;
		#20;
		code = 24'b010000000000000000000000;
		#20;
		code = 24'b010000000001000000000000;
		#20;
		code = 24'b010000000010000000000000;
		#20;
		code = 24'b100000000000000000000000;
		#20;
		code = 24'b100000000000000000000001;
		#20;
		code = 24'b110000000000000000000000;
		#20;
		code = 24'b110000000001000000000000;
		#20;
		code = 24'b110000000010000000000000;
		#20;
		code = 24'b110000000011000000000000;
		#20;
		code = 24'b110000000100000000000000;
		#20;
		$finish;
	end

endmodule