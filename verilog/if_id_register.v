module if_id_register(clk, op_code_in, data_in, op_code_out, data_out);
	input clk;
	input [23:0] op_code_in;
	input [19:0] data_in;
	output reg [23:0] op_code_out;
	output reg [19:0] data_out;

	always @(posedge clk) begin
		op_code_out <= op_code_in;
		data_out <= data_in;
	end
endmodule