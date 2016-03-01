module fo_ex_register (clk, mux4_in, mux5_in, mux6_in, reg_bank_in, data_mem_in, reg_flags_in, alu_in,
			mux4_out, mux5_out, mux6_out, reg_bank_out, data_mem_out, reg_flags_out, alu_out);
	input clk, mux4_in, data_mem_in, reg_flags_in;
	input [2:0] mux5_in, mux6_in, reg_bank_in;
	input [5:0] alu_in;
	output reg mux4_out, data_mem_out, reg_flags_out;
	output reg [2:0] mux5_out, mux6_out, reg_bank_out;
	output reg [5:0] alu_out;

	always @(posedge clk) begin
		mux4_out <= mux4_in;
		mux5_out <= mux5_in;
		mux6_out <= mux6_in;
		reg_bank_out <= reg_bank_in;
		data_mem_out <= data_mem_in;
		reg_flags_out <= reg_flags_in;
		alu_out <= alu_in;
	end
endmodule
