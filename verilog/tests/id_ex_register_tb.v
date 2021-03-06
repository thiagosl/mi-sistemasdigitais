module id_ex_register_tb;

	reg clk, mux4_in, data_mem_in, write_inst_in;
	reg [1:0] regs_bank_in;
	reg [3:0] regC_adress_in;
	reg [5:0] alu_in;
	reg [31:0] regA_in, regB_in, pc_in;
	wire mux4_out, data_mem_out, write_inst_out;
	wire [1:0] regs_bank_out;
	wire [3:0] regC_adress_out;
	wire [5:0] alu_out;
	wire [31:0] regA_out, regB_out, pc_out;

	id_ex_register DUT(clk, mux4_in, data_mem_in, alu_in, regA_in, regB_in, regs_bank_in, pc_in, regC_adress_in, write_inst_in, mux4_out, data_mem_out, alu_out, regA_out, regB_out, regs_bank_out, pc_out, regC_adress_out, write_inst_out);

	initial begin
		clk = 0;
		mux4_in = 1;
		data_mem_in = 0;
		write_inst_in = 1;
		regs_bank_in = 2;
		regC_adress_in = 9;
		alu_in = 25;
		regA_in = 100;
		regB_in = 240;
		pc_in = 620;
		#20;
		mux4_in = 0;
		data_mem_in = 1;
		write_inst_in = 0;
		regs_bank_in = 3;
		regC_adress_in = 6;
		alu_in = 20;
		regA_in = 450;
		regB_in = 170;
		pc_in = 380;
		#20;
		$finish;
	end

	always #5 clk = ~clk;

endmodule
