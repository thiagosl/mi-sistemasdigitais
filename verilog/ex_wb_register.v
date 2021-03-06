module ex_wb_register (clk, regs_bank_in, mux2_in, pc_in, regC_adress_in, write_inst_in, regs_bank_out, mux2_out, pc_out, regC_adress_out, write_inst_out);
	input clk, write_inst_in;
	input [1:0] regs_bank_in;
	input [3:0] regC_adress_in;
	input [31:0] mux2_in, pc_in;
	output reg write_inst_out;
	output reg [1:0] regs_bank_out;
	output reg [3:0] regC_adress_out;
	output reg [31:0] mux2_out, pc_out;

	always @(posedge clk) begin
		mux2_out <= mux2_in;
		regs_bank_out <= regs_bank_in;
		pc_out <= pc_in;
		regC_adress_out <= regC_adress_in;
		write_inst_out <= write_inst_in;
	end
endmodule