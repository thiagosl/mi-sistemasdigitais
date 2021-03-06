module hazard_controler (code_instruction, regA, regB, reg_hazard, reg_hazard_2, write_instruction, write_instruction_2, muxA_hazard_control, muxB_hazard_control);
	input write_instruction, write_instruction_2;
	input [3:0] regA, regB, reg_hazard, reg_hazard_2;
	input [23:0] code_instruction;
	output reg [1:0] muxA_hazard_control, muxB_hazard_control;

	always @(code_instruction or regA or regB or reg_hazard or reg_hazard_2 or write_instruction or write_instruction_2) begin
		muxA_hazard_control = 0;
		muxB_hazard_control = 0;
		if (write_instruction && reg_hazard == regA) begin
			muxA_hazard_control = 2'b01;
		end else if (write_instruction_2 && reg_hazard_2 == regA) begin
			muxA_hazard_control = 2'b10;
		end
		if (write_instruction && reg_hazard == regB) begin
			muxB_hazard_control = 2'b01;
		end else if (write_instruction_2 && reg_hazard_2 == regB) begin
			muxB_hazard_control = 2'b10;
		end if (code_instruction[23:22] == 2'b01) begin
			muxB_hazard_control = 2'b11;
		end
	end
endmodule
