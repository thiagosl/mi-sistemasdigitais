module processor (clk);

	input clk;
	wire [31:0] mux1_out, mux2_out, mux3_out, mux4_out, mux5_out, mux6_out, pc_out, add_4_out, reg_estagio0_out, reg_estagio1_out, signal_extend_out, adress_extend_out, regA, regB, alu_result, data_memory_out, operation;
	wire [5:0] alu_control, reg_flags_out, alu_flags;
	wire [2:0] regs_bank_control;
	wire [1:0] mux1_control, mux5_control, mux6_control;
	wire mux2_control, mux3_control, mux4_control, comparator_control, reg_flags_control, data_memory_control, comparator_out;
	
	pc pc0(mux6_out, clk, pc_out);
	add_4 add(pc_out, add_4_out);
	ram instruction_memory(1'b0, clk, pc_out, {32{1'b0}}, operation);
	reg_estagio reg_estagio0(mux6_out, reg_estagio0_out);
	control_unit control_unit0(operation[31:8], clk, mux1_control, mux2_control, mux3_control, mux4_control, mux5_control, mux6_control, reg_bank_control, data_memory_control, alu_control, reg_flags_control, comparator_control);
	signal_extend signal_extend0(operation[15:0], signal_extend_out);
	mux3_32b mux1(operation[15:12], operation[7:4], operation[3:0], mux1_control, mux1_out);
	mux3_32b mux5(operation[11:8], operation[7:4], operation[19:16], mux5_control, mux5_out);
	comparator comparator0(operation[19:16], reg_flags_out, comparator_control, comparator_out);
	reg_flags reg_flags0(alu_flags, reg_flags_control, reg_flags_out);
	reg_estagio reg_estagio1(signal_extend_out, reg_estagio1_out);
	adress_extend adress_extend0(operation[11:0], adress_extend_out);
	mux3_32b_PC mux6(add_4_out, adress_extend_out, regA, {mux6_control, comparator_out}, mux6_out);
	regs_bank regs_bank0(mux1_out, operation[3:0], mux5_out, clk, mux2_out, reg_estagio0_out, mux2_control, regA, regB);
	mux2_32b mux2(reg_estagio1_out, mux4_out, mux2_control, mux2_out);
	mux2_32b mux3(regA, regB, mux3_control, mux3_out);
	mux2_32b mux4(alu_result, data_memory_out, mux4_control, mux4_out);
	alu alu0(regA, regB, alu_control, alu_result, alu_flags);
	ram data_memory(data_memory_control, clk, mux3_out, regA, data_memory_out);

endmodule
