module ex_wb_register_tb;

	reg clk, write_inst_in;
	reg [1:0] regs_bank_in;
	reg [3:0] regC_adress_in;
	reg [31:0] mux2_in, pc_in;
	wire write_inst_out;
	wire [1:0] regs_bank_out;
	wire [3:0] regC_adress_out;
	wire [31:0] mux2_out, pc_out;

	ex_wb_register DUT(clk, regs_bank_in, mux2_in, pc_in, regC_adress_in, write_inst_in, regs_bank_out, mux2_out, pc_out, regC_adress_out, write_inst_out);

	initial begin
		clk = 0;
		write_inst_in = 1;
		regs_bank_in = 2;
		regC_adress_in = 9;
		mux2_in = 200;
		pc_in = 620;
		#20;
		write_inst_in = 0;
		regs_bank_in = 3;
		regC_adress_in = 12;
		mux2_in = 160;
		pc_in = 450;
		#20;
		$finish;
	end

	always #5 clk = ~clk;

endmodule
