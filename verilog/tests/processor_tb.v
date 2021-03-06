module processor_tb;

	reg clk, reset, read_file, write_file;

	processor DUT(clk, reset, read_file, write_file);

	initial begin
		clk = 0;
		reset = 0;
		read_file = 1;
		write_file = 0;
		#2
		read_file = 0;
		reset = 1;
		#2
		reset = 0;
		#20000
		write_file = 1;
		#2
		$finish;
	end

	always #1 clk = ~clk;

endmodule
