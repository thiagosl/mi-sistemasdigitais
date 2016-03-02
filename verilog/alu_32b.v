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
		
		if (operation != 6'b001001) begin
			flags[0] = 0;
			flags[5] = 0;
			if (result == 0) begin
				flags[4:1] = 4'b0101;	//ZERO, NEGZERO
			end
			if (result < 0) begin
				flags[4:1] = 4'b1110;	//NEG, NEGZERO, TRUE
			end
			if (result > 0) begin
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
			flags[5] = 0;	//CARRY
			case (operation)
				0, 2:	begin
					if ((operandA[31] == operandB[31]) && (operandA[31] != result[31])) begin
						flags[0] = 1;	//OVERFLOW
					end
				end
				3: begin
					if (operandA == max) begin
						flags[0] = 1;	//OVERFLOW
					end
				end
				4: begin
					if ((operandA[31] != operandB[31]) && (operandA[31] != result[31])) begin
						flags[0] = 1;	//OVERFLOW
					end
				end
				5: begin
					if ((operandA[31] != operandB[31]) && (operandA[31] != result[31]))  begin
						flags[0] = 1;	//OVERFLOW
					end
					else if (((operandA == min) && (operandB == 0)) || ((operandB == min) && (operandA == 0))) begin
						flags[0] = 1;	//OVERFLOW
					end
				end
				6: begin
					if (operandA == min) begin
						flags[0] = 1;	//OVERFLOW
					end
				end
			endcase
		end
	end
endmodule

module alu_32b_tb ();
	reg[31:0] operandA, operandB;
	reg[5:0] operation;
	wire[31:0] result;
	wire[5:0] flags;

	alu_32b DUT(operandA, operandB, operation, result, flags);

	initial begin
		//ADD TEST
		operandA = 4;
		operandB = 4;
		operation = 0;
		#20
		operandA = 4;
		operandB = -2;
		operation = 0;
		#20
		operandA = -2;
		operandB = -2;
		operation = 0;
		#20
		operandA = 4;
		operandB = -4;
		operation = 0;
		#20
		operandA = 4;
		operandB = 0;
		operation = 0;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 1;
		operation = 0;
		#20
		
		//ADD INC TEST
		operandA = 4;
		operandB = 4;
		operation = 2;
		#20
		operandA = 4;
		operandB = -2;
		operation = 2;
		#20
		operandA = -2;
		operandB = -1;
		operation = 2;
		#20
		operandA = 4;
		operandB = -4;
		operation = 2;
		#20
		operandA = 4;
		operandB = 0;
		operation = 2;
		#20
		operandA = 32'b01111111111111111111111111111110;
		operandB = 1;
		operation = 2;
		#20
		
		//INCA TEST
		operandA = 4;
		operation = 3;
		#20
		operandA = -1;
		operation = 3;
		#20
		operandA = -2;
		operation = 3;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operation = 3;
		#20
		
		//SUB TEST
		operandA = 5;
		operandB = 4;
		operation = 4;
		#20
		operandA = 4;
		operandB = -2;
		operation = 4;
		#20
		operandA = -2;
		operandB = -2;
		operation = 4;
		#20
		operandA = 4;
		operandB = -5;
		operation = 4;
		#20
		operandA = 4;
		operandB = 0;
		operation = 4;
		#20
		operandA = 32'b10000000000000000000000000000000;
		operandB = 1;
		operation = 4;
		#20
		
		//SUB DEC TEST
		operandA = 4;
		operandB = 4;
		operation = 5;
		#20
		operandA = 4;
		operandB = -2;
		operation = 5;
		#20
		operandA = -2;
		operandB = -1;
		operation = 5;
		#20
		operandA = 4;
		operandB = -4;
		operation = 5;
		#20
		operandA = 4;
		operandB = 0;
		operation = 5;
		#20
		operandA = 32'b10000000000000000000000000000001;
		operandB = 1;
		operation = 5;
		#20
		
		//DECA TEST
		operandA = 4;
		operation = 6;
		#20
		operandA = 1;
		operation = 6;
		#20
		operandA = -2;
		operation = 6;
		#20
		operandA = 32'b10000000000000000000000000000000;
		operation = 6;
		#20
		
		//PASSB TEST
		operandA = 4;
		operation = 9;
		#20
		operandA = -1;
		operation = 9;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operation = 9;
		#20
		operandA = 0;
		operation = 9;
		#20
		
		//ASL TEST
		operandA = 4;
		operation = 13;
		#20
		operandA = -4;
		operation = 13;
		#20
		operandA = 32'b10000000000000000000000000000010;
		operation = 13;
		#20
		
		//ASR TEST
		operandA = 4;
		operation = 14;
		#20
		operandA = -4;
		operation = 14;
		#20
		operandA = 32'b10000000000000000000000000000010;
		operation = 14;
		#20
		
		//ZEROS TEST
		operation = 15;
		#20
		
		//ONES TEST
		operation = 16;
		#20
		
		//PASSA TEST
		operandA = 8;
		operation = 17;
		#20
		operandA = 0;
		operation = 17;
		#20
		operandA = -1;
		operation = 17;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operation = 17;
		#20
		
		//PASSNOTA TEST
		operandA = 8;
		operation = 18;
		#20
		operandA = 0;
		operation = 18;
		#20
		operandA = -1;
		operation = 18;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operation = 18;
		#20
		
		//AND TEST
		operandA = 8;
		operandB = -4;
		operation = 19;
		#20
		operandA = 0;
		operandB = -4;
		operation = 19;
		#20
		operandA = -1;
		operandB = -4;
		operation = 19;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 19;
		#20
		
		//ANDNOTA TEST
		operandA = 8;
		operandB = -4;
		operation = 20;
		#20
		operandA = 0;
		operandB = -4;
		operation = 20;
		#20
		operandA = -1;
		operandB = -4;
		operation = 20;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 20;
		#20
		
		//NAND TEST
		operandA = 8;
		operandB = -4;
		operation = 21;
		#20
		operandA = 0;
		operandB = -4;
		operation = 21;
		#20
		operandA = -1;
		operandB = -4;
		operation = 21;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 21;
		#20
		
		//OR TEST
		operandA = 8;
		operandB = -4;
		operation = 22;
		#20
		operandA = 0;
		operandB = -4;
		operation = 22;
		#20
		operandA = -1;
		operandB = -4;
		operation = 22;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 22;
		#20
		
		//ORNOTA TEST
		operandA = 8;
		operandB = -4;
		operation = 23;
		#20
		operandA = 0;
		operandB = -4;
		operation = 23;
		#20
		operandA = -1;
		operandB = -4;
		operation = 23;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 23;
		#20
		
		//NOR TEST
		operandA = 8;
		operandB = -4;
		operation = 24;
		#20
		operandA = 0;
		operandB = -4;
		operation = 24;
		#20
		operandA = -1;
		operandB = -4;
		operation = 24;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 24;
		#20
		
		//XOR TEST
		operandA = 8;
		operandB = -4;
		operation = 25;
		#20
		operandA = 0;
		operandB = -4;
		operation = 25;
		#20
		operandA = -1;
		operandB = -4;
		operation = 25;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 25;
		#20
		
		//XORNOTA TEST
		operandA = 8;
		operandB = -4;
		operation = 26;
		#20
		operandA = 0;
		operandB = -4;
		operation = 26;
		#20
		operandA = -1;
		operandB = -4;
		operation = 26;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 26;
		#20
		
		//XNOR TEST
		operandA = 8;
		operandB = -4;
		operation = 27;
		#20
		operandA = 0;
		operandB = -4;
		operation = 27;
		#20
		operandA = -1;
		operandB = -4;
		operation = 27;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operandB = 4;
		operation = 27;
		#20
		
		//LSL TEST
		operandA = 8;
		operation = 28;
		#20
		operandA = 0;
		operation = 28;
		#20
		operandA = -1;
		operation = 28;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operation = 28;
		#20
		operandA = 32'b01111111111111111111111100111111;
		operation = 28;
		#20
		
		//LSR TEST
		operandA = 8;
		operation = 29;
		#20
		operandA = 0;
		operation = 29;
		#20
		operandA = -1;
		operation = 29;
		#20
		operandA = 32'b01111111111111111111111111111111;
		operation = 29;
		#20
		operandA = 32'b01111111111111111111111100111111;
		operation = 29;
		#20
		$finish;
	end
endmodule