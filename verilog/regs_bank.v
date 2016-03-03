module regs_bank (inpA, inpB, inpC, clk, data, pc, control, outA, outB);

	input [3:0] inpA, inpB, inpC;
	input clk;
	input [2:0] control;
	input [31:0] data;
	input [31:0] pc;
	output reg [31:0] outA, outB;
	reg [31:0] regBank[15:0], temp;
	
	always @(posedge clk) begin
		case (control)
			3'b000: regBank[inpC] = data;
			3'b001: begin
				temp = regBank[inpC];
				regBank[inpC] = {temp[31:16], data[15:0]};
			end
			3'b010: begin
				temp = regBank[inpC];
				regBank[inpC] = {data[15:0], temp[15:0]};
			end
			3'b011: begin
				regBank[15] = pc;
			end
		endcase
	end
	always @(negedge clk) begin
		outA = regBank[inpA];
		outB = regBank[inpB];
	end

endmodule


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
