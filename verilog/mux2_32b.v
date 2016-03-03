module mux2_32b (input0, input1, control, out);

	input [31:0] input0, input1;
	input control;
	output reg [31:0] out;
	
	always @(control or input0 or input1) begin
		if(control) begin
			out = input1;
		end else begin
			out = input0;
		end
	end

endmodule