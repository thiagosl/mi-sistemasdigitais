module control_unit (code, mux1, mux4, mux5, mux6, regs_bank, data_mem, alu, comparator, write_inst);

	input [23:0] code;
	output reg [1:0] mux1, mux5, mux6, regs_bank;
	output reg mux4, data_mem, comparator, write_inst;
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
				mux4 = 1'b0;
				regs_bank = 2'b00;
			end
			2'b01: begin 
				mux1 = 2'b11;
				mux5 = 2'b10;
				mux6 = 2'b00;
				comparator = 1'b0;
				data_mem = 1'b0;
				if (code[14:12] == 3'b000) begin
					alu = 6'b100000;
				end else begin
					alu = {5'b01111, code[13]};
				end
				mux4 = 1'b0;
				regs_bank = 2'b00;
			end
			2'b10: begin
				mux5 = 2'b01;
				mux6 = 2'b00;
				comparator = 1'b0;
				data_mem = code[0];
				alu = 6'b100000;
				mux4 = 1'b1;
				regs_bank = {2{code[0]}};
				if (code[0] == 0) begin
					mux1 = 2'b10;
				end else begin
					mux1 = 2'b01;
				end
			end
			2'b11: begin
				mux1 = 2'b00;
				mux5 = 2'b11;
				comparator = code[12];
				data_mem = 1'b0;
				alu = 6'b100000;
				if (code[14:12] == 3'b011 || code[14:12] == 3'b100) begin
					mux6 = 2'b10;
				end else if (code[14:12] == 3'b000) begin
					mux6 = 2'b11;
				end else begin
					mux6 = 2'b01;
				end
				mux4 = 1'b0;
				if (code[14:12] == 3'b011) begin
					regs_bank = 3'b01;
				end else begin
					regs_bank = 2'b11;
				end
			end
		endcase

		if (code[23:12] == 12'b110000000011) begin
			write_inst = 1;
		end else if ((code[23] == 0) || (code[23:0] == 24'b100000000000000000000000)) begin
			write_inst = 1;
		end else begin
			write_inst = 0;
		end
	end
endmodule