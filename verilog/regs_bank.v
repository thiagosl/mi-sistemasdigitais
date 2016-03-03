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