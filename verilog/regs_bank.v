module regs_bank (inpA, inpB, inpC, clk, data, pc, control, outA, outB);

	input [3:0] inpA, inpB, inpC;
	input clk;
	input [1:0] control;
	input [31:0] data, pc;
	output reg [31:0] outA, outB;
	reg [31:0] regBank[15:0];
	
	always @(posedge clk) begin
		case (control)
			2'b00: regBank[inpC] = data;
			2'b01: regBank[15] = pc;
		endcase
	end
	always @(inpA or inpB) begin
		outA = regBank[inpA];
		outB = regBank[inpB];
	end

endmodule