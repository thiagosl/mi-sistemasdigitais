module id_fo_register (clk, mux2_in, mux3_in, mux4_in, mux5_in, mux6_in, reg_bank_in, data_mem_in, comparator_in,
			reg_flags_in, alu_in, data_in, mux2_out, mux3_out, mux4_out, mux5_out, mux6_out, reg_bank_out,
			data_mem_out, comparator_out, reg_flags_out, alu_out, data_out);
	input clk, mux2_in, mux3_in, mux4_in, data_mem_in, comparator_in, reg_flags_in;
	input [2:0] mux5_in, mux6_in, reg_bank_in;
	input [5:0] alu_in;
	input [19:0] data_in;
	output reg mux2_out, mux3_out, mux4_out, data_mem_out, comparator_out, reg_flags_out;
	output reg [2:0] mux5_out, mux6_out, reg_bank_out;
	output reg [5:0] alu_out;
	output reg [19:0] data_out;

	always @(posedge clk) begin
		mux2_out <= mux2_in;
		mux3_out <= mux3_in;
		mux4_out <= mux4_in;
		mux5_out <= mux5_in;
		mux6_out <= mux6_in;
		reg_bank_out <= reg_bank_in;
		data_mem_out <= data_mem_in;
		comparator_out <= comparator_in;
		reg_flags_out <= reg_flags_in;
		alu_out <= alu_in;
		data_out <= data_in;
	end
endmodule
