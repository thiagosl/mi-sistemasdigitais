module processor (clk, reset, read_file, write_file);

	input clk, reset, read_file, write_file;
	wire [3:0] mux1_out, mux5_out, ex_mux5_out, wb_mux5_out;
	wire [31:0] mux4_out, mux6_out, muxA_out, muxB_out, pc_out, add_1_out, signal_extend_out, adress_extend_out, regA, regB, alu_result, data_memory_out, operation, ex_muxA_out, ex_muxB_out, ex_add_1_out, wb_mux4_out, wb_add_1_out;
	wire [5:0] alu_control, alu_flags, ex_alu_control;
	wire [1:0] mux1_control, mux5_control, mux6_control, muxA_control, muxB_control, regs_bank_control, ex_regs_bank_control, wb_regs_bank_control;
	wire mux4_control, comparator_control, data_memory_control, comparator_out, data_mem_control, write_inst, ex_mux4_control, ex_data_mem_control, ex_write_inst, wb_write_inst;

	pc pc0(mux6_out, clk, reset, pc_out);
	add_1 add(pc_out, add_1_out);
	ram instruction_memory(read_file, 1'b0, 1'b0, clk, pc_out[9:0], {32{1'b0}}, operation);
	mux3_32b_PC mux6(add_1_out, adress_extend_out, regA, {mux6_control, comparator_out}, mux6_out);
	mux4_4b mux1(operation[15:12], operation[7:4], operation[3:0], operation[19:16], mux1_control, mux1_out);
	mux4_4b mux5(operation[11:8], operation[7:4], operation[19:16], 4'b1111, mux5_control, mux5_out);
	comparator comparator0(operation[19:16], alu_flags, comparator_control, comparator_out);
	adress_extend adress_extend0(operation[11:0], adress_extend_out);
	hazard_controler hazard_controler0 (operation[31:8], mux1_out, operation[3:0], ex_mux5_out, wb_mux5_out, ex_write_inst, wb_write_inst, muxA_control, muxB_control);
	control_unit control_unit0(operation[31:8], mux1_control, mux4_control, mux5_control, mux6_control, regs_bank_control, data_mem_control, alu_control, comparator_control, write_inst);
	signal_extend signal_extend0(operation[15:0], signal_extend_out);
	regs_bank regs_bank0(mux1_out, operation[3:0], wb_mux5_out, clk, wb_mux4_out, wb_add_1_out, wb_regs_bank_control, regA, regB);
	mux3_32b muxA_hazard(regA, mux4_out, wb_mux4_out, muxA_control, muxA_out);
	mux4_32b muxB_hazard(regB, mux4_out, wb_mux4_out, signal_extend_out, muxB_control, muxB_out);
	id_ex_register id_ex_register0(clk, mux4_control, data_mem_control, alu_control, muxA_out, muxB_out, regs_bank_control, add_1_out, mux5_out, write_inst, ex_mux4_control, ex_data_mem_control, ex_alu_control, ex_muxA_out, ex_muxB_out, ex_regs_bank_control, ex_add_1_out, ex_mux5_out, ex_write_inst);
	mux2_32b mux4(alu_result, data_memory_out, ex_mux4_control, mux4_out);
	alu_32b alu0(ex_muxA_out, ex_muxB_out, ex_alu_control, alu_result, alu_flags);
	ram data_memory(read_file, write_file, ex_data_mem_control, clk, ex_muxA_out[9:0], ex_muxB_out, data_memory_out);
	ex_wb_register ex_wb_register0(clk, ex_regs_bank_control, mux4_out, ex_add_1_out, ex_mux5_out, ex_write_inst, wb_regs_bank_control, wb_mux4_out, wb_add_1_out, wb_mux5_out, wb_write_inst);
endmodule