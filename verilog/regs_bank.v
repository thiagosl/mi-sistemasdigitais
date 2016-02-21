module reg_bank (inpA, inpB, inpC, clk, data, pc, control, outA, outB);

	input [3:0] inpA, inpB, inpC;
	input clk;
	input [1:0] control;
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
		case (control)
			2'b01: outA = regBank[inpA];
			2'b01: outB = regBank[inpB];
		endcase
	end

endmodule


module reg_bank_tb;

	reg [3:0] inpA, inpB, inpC;
	reg clk;
	reg [1:0] control;
	reg [31:0] data;
	wire [31:0] outA, outB;

	reg_bank DUT(inpA, inpB, inpC, clk, control, data, outA, outB);

	initial begin
		inpA = 4'b0100;
		inpB = 4'b0100;
		inpC = 4'b0100;
		data = 32'b01010101010101010101010101010101;
		control = 2'b01;
		#20;
		data = 32'b11111111111111111111111111111111;
		control = 2'b10;
		#20;
		control = 2'b00;
		#20;
		control = 2'b11;
		#20;
		control = 2'b00;
		$finish;
	end

	always #5 clk = ~clk;

endmodule
