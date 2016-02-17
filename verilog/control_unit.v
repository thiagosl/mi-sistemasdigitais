module reg_bank (code, clk, mux1, mux2, mux3, mux4, mux5, mux6, reg_bank, data_mem, alu, reg_flags, comparator);

	input [23:0] code;
	input clk;
	output reg [1:0] mux1, mux5, mux6, reg_bank;
	output reg mux2, mux3, mux4, data_mem, comparator, reg_flags;
	output reg [5:0] alu;
	reg mux2_temp, mux3_temp, mux4_temp, data_mem_temp, reg_flags_temp;
	reg [5:0] alu_temp;
	reg [1:0] reg_bank_temp, reg_bank_temp2;
	
	always @(posedge clk) begin
		mux2 = mux2_temp;
		mux3 = mux3_temp;
		mux4 = mux4_temp;
		data_mem = data_mem_temp;
		reg_flags = reg_flags_temp;
		alu = alu_temp;
		reg_bank = reg_bank_temp;
		reg_bank_temp = reg_bank_temp2;
		case (code[23:22])
			2'b00: begin 
				mux1 = 2'b01;
				mux5 = 2'b00;
				mux6 = 2'b00;
				comparator = 1'b0;
				mux2_temp = 1'b1;
				mux3_temp = 1'b0;
				mux4_temp = 1'b0;
				data_mem_temp = 1'b0;
				reg_flags_temp = 1'b1;
				alu_temp = code[9:4];
				reg_bank_temp2 = 2'b00;
			end
			2'b01: begin 
				mux1 = 2'b11;
				mux5 = 2'b11;
				mux6 = 2'b00;
				comparator = 1'b0;
				mux2_temp = 1'b0;
				mux3_temp = 1'b0;
				mux4_temp = 1'b0;
				data_mem_temp = 1'b0;
				reg_flags_temp = 1'b0;
				alu_temp = 6'b111111;
				reg_bank_temp2 = code[13:12];
			end
			2'b10: begin
				mux1 = 2'b10;
				mux5 = 2'b01;
				mux6 = 2'b00;
				comparator = 1'b0;
				mux2_temp = 1'b1;
				mux3_temp = code[0];
				mux4_temp = 1'b1;
				data_mem_temp = code[0];
				reg_flags_temp = 1'b0;
				alu_temp = 6'b111111;
				reg_bank_temp2 = {2{code[0]}};
			end
			2'b11: begin
				mux1 = 2'b00;
				mux5 = 2'b11;
				comparator = ~code[12];
				mux2_temp = 1'b0;
				mux3_temp = 1'b0;
				mux4_temp = 1'b0;
				data_mem_temp = 1'b0;
				reg_flags_temp = 1'b0;
				alu_temp = 6'b111111;
				reg_bank_temp2 = 2'b11;
				if (code[14:12] == 3'b011 || code[14:12] == 3'b100) begin
					mux6 = 2'b10;
				end else begin
					mux6 = 2'b01;
				end
			end
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
