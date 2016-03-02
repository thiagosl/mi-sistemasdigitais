module processor (clk, reset, read_file, write_file, mux_ula_out);

	input clk, reset, read_file, write_file;
	wire [3:0] mux1_out, mux5_out;
	wire [31:0] mux2_out, mux4_out, mux6_out, pc_out, add_1_out, signal_extend_out, adress_extend_out, regA, regB, alu_result, data_memory_out, operation;
	wire [5:0] alu_control, reg_flags_out, alu_flags;
	wire [2:0] regs_bank_control;
	wire [1:0] mux1_control, mux5_control, mux6_control;
	wire mux2_control, mux4_control, comparator_control, reg_flags_control, data_memory_control, comparator_out;
	output [31:0] mux_ula_out;

	assign mux_ula_out = mux2_out;

	pc pc0(mux6_out, clk, reset, pc_out);
	add_1 add(pc_out, add_1_out);
	ram instruction_memory(read_file, 1'b0, 1'b0, clk, pc_out[9:0], {32{1'b0}}, operation);
	control_unit control_unit0(operation[31:8], mux1_control, mux2_control, mux4_control, mux5_control, mux6_control, regs_bank_control, data_memory_control, alu_control, reg_flags_control, comparator_control);
	signal_extend signal_extend0(operation[15:0], signal_extend_out);
	mux3_32b mux1(operation[15:12], operation[7:4], operation[3:0], mux1_control, mux1_out);
	mux3_32b mux5(operation[11:8], operation[7:4], operation[19:16], mux5_control, mux5_out);
	comparator comparator0(operation[19:16], reg_flags_out, comparator_control, comparator_out);
	reg_flags reg_flags0(alu_flags, reg_flags_control, reg_flags_out);
	adress_extend adress_extend0(operation[11:0], adress_extend_out);
	mux3_32b_PC mux6(add_1_out, adress_extend_out, regA, {mux6_control, comparator_out}, mux6_out);
	regs_bank regs_bank0(mux1_out, operation[3:0], mux5_out, clk, mux2_out, pc_out, regs_bank_control, regA, regB);
	mux2_32b mux2(signal_extend_out, mux4_out, mux2_control, mux2_out);
	mux2_32b mux4(alu_result, data_memory_out, mux4_control, mux4_out);
	alu_32b alu0(regA, regB, alu_control, alu_result, alu_flags);
	ram data_memory(read_file, write_file, data_memory_control, clk, regA[9:0], regB, data_memory_out);

endmodule

module processor_tb;

	reg clk, reset, read_file, write_file;
	wire [31:0] mux_ula_out;

	processor DUT(clk, reset, read_file, write_file, mux_ula_out);

	initial begin
		clk = 0;
		reset = 0;
		read_file = 1;
		write_file = 0;
		#10;
		read_file = 0;
		reset = 1;
		#10
		reset = 0;
		#200;
		write_file = 1;
		#10;
		$finish;
	end

	always #5 clk = ~clk;

endmodule
