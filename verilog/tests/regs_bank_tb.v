module regs_bank_tb;

	reg [3:0] inpA, inpB, inpC;
	reg clk;
	reg [2:0] control;
	reg [31:0] data;
	reg [31:0] pc;
	wire [31:0] outA, outB;

	regs_bank DUT(inpA, inpB, inpC, clk, data, pc, control, outA, outB);

	initial begin
		clk = 1'b0;
		inpA = 4'b1111;
		inpB = 4'b1111;
		inpC = 4'b1111;
		data = 32'b01010101010101010101010101010101;
		control = 3'b000;
		#15;
		data = 32'b11111111111111111111111111111111;
		control = 3'b001;
		#17;
		control = 3'b010;
		#10;
		control = 3'b011;
		pc = 32'b00110011001100110011001100110011;
		#20
		control = 3'b111;
		#20
		$finish;
	end

	always #5 clk = ~clk;

endmodule