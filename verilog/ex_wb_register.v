module ex_wb_register (clk, mux5_in, mux6_in, reg_bank_in, mux5_out, mux6_out, reg_bank_out);
	input clk;
	input [2:0] mux5_in, mux6_in, reg_bank_in;
	output reg [2:0] mux5_out, mux6_out, reg_bank_out;

	always @(posedge clk) begin
		mux5_out <= mux5_in;
		mux6_out <= mux6_in;
		reg_bank_out <= reg_bank_in;
	end
endmodule