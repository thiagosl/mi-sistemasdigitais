module pc (inp, clk, reset, out);

	input [31:0] inp;
	input clk, reset;
	output reg [31:0] out;
	
	always @(posedge clk) begin
		out = inp;
	end
	always @(reset) begin
		if (reset == 1) begin
			out = 0;
		end
	end
endmodule