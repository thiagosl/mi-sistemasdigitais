module ex_wb_register_tb;

	reg write_instruction, write_instruction_2;
	reg [3:0] regA, regB, reg_hazard, reg_hazard_2;
	reg [23:0] code_instruction;
	wire [1:0] muxA_hazard_control, muxB_hazard_control;

	ex_wb_register DUT(clk, regs_bank_in, mux2_in, pc_in, regC_adress_in, write_inst_in, regs_bank_out, mux2_out, pc_out, regC_adress_out, write_inst_out);

	initial begin
		write_instruction = 1;
		write_instruction_2 = 0;
		regA = 9;
		regB = 12;
		reg_hazard = 5;
		reg_hazard_2 = 8;
		code_instruction = 620;
		#20;
		write_instruction = 0;
		write_instruction_2 = 1;
		regA = 6;
		regB = 3;
		reg_hazard = 13;
		reg_hazard_2 = 10;
		code_instruction = 470;
		#20;
		$finish;
	end
endmodule
