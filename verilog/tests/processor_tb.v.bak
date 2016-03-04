module processor_tb;

	reg clk, reset, read_file, write_file;
	wire [31:0] mux_ula_out;

	processor DUT(clk, reset, read_file, write_file, mux_ula_out);

	initial begin
		clk = 0;
		reset = 0;
		read_file = 1;
		write_file = 0;
		#10;
		read_file = 0;
		reset = 1;
		#10
		reset = 0;
		#200;
		write_file = 1;
		#10;
		$finish;
	end

	always #5 clk = ~clk;

endmodule
