module reg_flags (inp, control, clk, out);

	input [5:0] inp;
	input control, clk;
	output reg [5:0] out;
	
	always @(posedge clk) begin
		if (control) begin
			out = inp;
		end
	end

endmodule