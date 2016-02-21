module alu_32b (operandA, operandB, operation, result, flags);

	input [31:0] operandA;
	input [31:0] operandB;
	input [5:0] operation;
	output reg [31:0] result;
	output reg [5:0] flags;

	always@(operation or operandA or operandB)
	begin
		case(operation)
			6'b000000:	result = operandA + operandB;	//ADD
			6'b000010:	result = operandA + operandB + 1;	//ADD INC
			6'b000011:	result = operandA + 1;	//INCA
			6'b000100:	result = operandA - operandB;	//SUB
			6'b000101:	result = operandA - operandB - 1;	//SUB DEC
			6'b000110:	result = operandA - 1;	//DECA
			6'b001001:	result = operandA;	//PASSB
			6'b001101:	result = operandA <<< 1;	//ASL
			6'b001110:	result = operandA >>> 1;	//ASR
			6'b001111:	result = 0;	//ZEROS
			6'b010000:	result = 1;	//ONES
			6'b010001:	result = operandA;	//PASSA
			6'b010010:	result = ~operandA;	//PASSNOTA
			6'b010011:	result = operandA & operandB;	//AND
			6'b010100:	result = (~operandA) & operandB;	//ANDNOTA
			6'b010101:	result = ~(operandA & operandB);	//NAND
			6'b010110:	result = operandA | operandB;	//OR
			6'b010111:	result = (~operandA) | operandB;	//ORNOTA
			6'b011000:	result = ~(operandA | operandB);	//NOR
			6'b011001:	result = operandA ^ operandB;	//XOR
			6'b011010:	result = (~operandA) ^ operandB;	//XORNOTA
			6'b011011:	result = ~(operandA ^ operandB);	//XNOR
			6'b011100:	result = operandA << 1;	//LSL
			6'b011101:	result = operandA >> 1;	//LSR
		endcase
	end
endmodule