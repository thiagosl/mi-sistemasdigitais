module alu_32b_tb ();
	reg[31:0] operandA, operandB;
	reg[5:0] operation;
	wire[31:0] result;

	alu_32b DUT(operandA, operandB, operation, result);

	initial begin
		operandA = 32'b00000000000000000000000000000010;
		operandB = 32'b00000000000000000000000000000010;
		operation = 6'b000000;
		#20
		operandA = 32'b00000000000000000000000000000010;
		operation = 6'b000011;
		#20
		operandA = 32'b00000000000000000000000000000011;
		operation = 6'b000011;
		#20
		$finish;
	end
endmodule
