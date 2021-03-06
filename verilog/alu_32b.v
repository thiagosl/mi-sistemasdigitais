module alu_32b (operandA, operandB, operation, result, flags);

	input[31:0] operandA;
	input[31:0] operandB;
	input[5:0] operation;
	output reg[31:0] result;
	output reg[5:0] flags;

	reg[31:0] max = 32'b01111111111111111111111111111111;
	reg[31:0] min = 32'b10000000000000000000000000000000;

	always@(operandA or operandB or operation) begin
		case(operation)
			6'b000000:	result = operandA + operandB;	//ADD
			6'b000010:	result = operandA + operandB + 1;	//ADD INC
			6'b000011:	result = operandA + 1;	//INCA
			6'b000100:	result = operandA - operandB;	//SUB
			6'b000101:	result = operandA - operandB - 1;	//SUB DEC
			6'b000110:	result = operandA - 1;	//DECA
			6'b001001:	result = operandA;	//PASSB
			6'b001101:	result = operandA <<< 1;	//ASL
			6'b001110:	begin 
						result = operandA >>> 1;	//ASR
						result[31] = result[30];
					end
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
			6'b011110: 	result = {operandA[31:16], operandB[15:0]}; //LCL
			6'b011111:	result = {operandB[15:0], operandA[15:0]};  //LCH
			6'b100000:	result = operandB;  //LOADLIT
		endcase
		
		if (operation != 6'b001001 && operation != 6'b011110 && operation != 6'b011111 && operation != 6'b100000) begin
			flags[0] = 0;
			flags[5] = 0;
			if (result == 0) begin
				flags[4:1] = 4'b0101;	//NEGZERO, ZERO
			end else if (result[31] == 1) begin
				flags[4:1] = 4'b1110;	//TRUE, NEGZERO, NEG
			end else if (result[31] == 0) begin
				flags[4:1] = 4'b1000;	//TRUE
			end
		end
		
		if (operation > 9) begin
			if (operation == 13) begin
				if (operandA[31] != result[31]) begin
					flags[0] = 1;	//OVERFLOW
				end
			end
			if (operation == 28) begin
				if (operandA[31] == 1) begin
					flags[5] = 1;	//CARRY
				end
			end
		end
		else begin
			case (operation)
				0, 2:	begin
					if ((operandA[31] == operandB[31]) && (operandA[31] != result[31])) begin
						flags[0] = 1;	//OVERFLOW
					end
					if ((operandA[31] != operandB[31]) && (result[31] == 0)) begin
						flags[5] = 1;	//CARRY
					end
				end
				3: begin
					if (operandA == max) begin
						flags[0] = 1;	//OVERFLOW
					end
					if (operandA == 32'b11111111111111111111111111111111) begin
						flags[5] = 1;	//CARRY
					end
				end
				4: begin
					if ((operandA[31] != operandB[31]) && (operandA[31] != result[31])) begin
						flags[0] = 1;	//OVERFLOW
					end
					if ((operandA[31] == operandB[31]) && (result[31] == 0)) begin
						flags[5] = 1;	//CARRY
					end
				end
				5: begin
					if ((operandA[31] != operandB[31]) && (operandA[31] != result[31]))  begin
						flags[0] = 1;	//OVERFLOW
					end
					else if (((operandA == min) && (operandB == 0)) || ((operandB == min) && (operandA == 0))) begin
						flags[0] = 1;	//OVERFLOW
					end
					if ((operandA[31] == operandB[31]) && (result[31] == 0)) begin
						flags[5] = 1;	//CARRY
					end
				end
				6: begin
					if (operandA == min) begin
						flags[0] = 1;	//OVERFLOW
					end
					if (operandA == 1) begin
						flags[5] = 1;	//CARRY
					end
				end
			endcase
		end
	end
endmodule