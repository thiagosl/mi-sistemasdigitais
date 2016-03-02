module control_unit (code, mux1, mux2, mux4, mux5, mux6, regs_bank, data_mem, alu, reg_flags, comparator);

	input [23:0] code;
	output reg [1:0] mux1, mux5, mux6;
	output reg [2:0] regs_bank;
	output reg mux2, mux4, data_mem, comparator, reg_flags;
	output reg [5:0] alu;
	
	always @(code) begin
		case (code[23:22])
			2'b00: begin
				mux1 = 2'b01;
				mux5 = 2'b00;
				mux6 = 2'b00;
				comparator = 1'b0;
				data_mem = 1'b0;
				alu = code[9:4];
				
				mux2 = 1'b1;
				mux4 = 1'b0;
				reg_flags = 1'b1;
				regs_bank = 3'b000;
			end
			2'b01: begin 
				mux1 = 2'b11;
				mux5 = 2'b11;
				mux6 = 2'b00;
				comparator = 1'b0;
				data_mem = 1'b0;
				alu = 6'b111111;
				
				mux2 = 1'b0;
				mux4 = 1'b0;
				reg_flags = 1'b0;
				regs_bank = {1'b0, code[13:12]};
			end
			2'b10: begin
				mux5 = 2'b01;
				mux6 = 2'b00;
				comparator = 1'b0;
				data_mem = code[0];
				alu = 6'b111111;

				mux2 = 1'b1;
				mux4 = 1'b1;
				reg_flags = 1'b0;
				regs_bank = {3{code[0]}};
				if (code[0] == 0) begin
					mux1 = 2'b10;
				end else begin
					mux1 = 2'b01;
				end
			end
			2'b11: begin
				mux1 = 2'b00;
				mux5 = 2'b11;
				comparator = ~code[12];
				data_mem = 1'b0;
				alu = 6'b111111;
				if (code[14:12] == 3'b011 || code[14:12] == 3'b100) begin
					mux6 = 2'b10;
				end else begin
					mux6 = 2'b01;
				end

				mux2 = 1'b0;
				mux4 = 1'b0;
				reg_flags = 1'b0;
				if (code[14:12] == 3'b011) begin
					regs_bank = 3'b011;
				end else begin
					regs_bank = 3'b111;
				end
			end
		endcase
	end
endmodule


module control_unit_tb;

	reg [23:0] code;
	wire [1:0] mux1, mux5, mux6;
	wire [2:0] regs_bank;
	wire mux2, mux4, data_mem, comparator, reg_flags;
	wire [5:0] alu;

	control_unit DUT(code, mux1, mux2, mux4, mux5, mux6, regs_bank, data_mem, alu, reg_flags, comparator);

	initial begin
		code = 24'b000000000000000100110000;
		#20;
		code = 24'b000000000000000010010000;
		#20;
		code = 24'b010000000000000000000000;
		#20;
		code = 24'b010000000001000000000000;
		#20;
		code = 24'b010000000010000000000000;
		#20;
		code = 24'b100000000000000000000000;
		#20;
		code = 24'b100000000000000000000001;
		#20;
		code = 24'b110000000000000000000000;
		#20;
		code = 24'b110000000001000000000000;
		#20;
		code = 24'b110000000010000000000000;
		#20;
		code = 24'b110000000011000000000000;
		#20;
		code = 24'b110000000100000000000000;
		#20;
		$finish;
	end

endmodule