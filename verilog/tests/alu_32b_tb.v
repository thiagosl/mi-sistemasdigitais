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

		//LCL TEST
		operandA = 5;
		operandB = -1;
		operation = 30;
		#20

		//LCH TEST
		operandA = 7;
		operandB = -1;
		operation = 31;
		#20

		//LCL TEST
		operandA = 15;
		operandB = -1;
		operation = 32;
		#20

		$finish;
	end
endmodule
