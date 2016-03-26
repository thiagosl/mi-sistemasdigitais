module id_ex_register (clk, mux4_in, data_mem_in, alu_in, regA_in, regB_in, regs_bank_in, pc_in, regC_adress_in, write_inst_in, mux4_out, data_mem_out, alu_out, regA_out, regB_out, regs_bank_out, pc_out, regC_adress_out, write_inst_out);

	input clk, mux4_in, data_mem_in, write_inst_in;
	input [1:0] regs_bank_in;
	input [3:0] regC_adress_in;
	input [5:0] alu_in;
	input [31:0] regA_in, regB_in, pc_in;
	output reg mux4_out, data_mem_out, write_inst_out;
	output reg [1:0] regs_bank_out;
	output reg [3:0] regC_adress_out;
	output reg [5:0] alu_out;
	output reg [31:0] regA_out, regB_out, pc_out;

	always @(posedge clk) begin
		mux4_out <= mux4_in;
		data_mem_out <= data_mem_in;
		alu_out <= alu_in;
		regA_out <= regA_in;
		regB_out <= regB_in;
		regs_bank_out <= regs_bank_in;
		pc_out <= pc_in;
		regC_adress_out <= regC_adress_in;
		write_inst_out <= write_inst_in;
	end
endmodule

