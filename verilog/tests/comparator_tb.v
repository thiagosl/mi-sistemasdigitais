module comparator_tb;

	reg [3:0] condicao;
	reg [5:0] flags;
	reg control;
	wire salto;

	comparator DUT(condicao, flags, control, salto);

	initial begin
		control = 1'b1;
		flags = 6'b010101;
		condicao = 4'b0000;
		#10;
		condicao = 4'b0001;
		#10;
		condicao = 4'b0010;
		#10;
		condicao = 4'b0011;
		#10;
		condicao = 4'b0100;
		#10;
		condicao = 4'b0101;
		#10;
		flags = 6'b111000;
		condicao = 4'b0000;
		#10;
		condicao = 4'b0001;
		#10;
		condicao = 4'b0010;
		#10;
		condicao = 4'b0011;
		#10;
		condicao = 4'b0100;
		#10;
		condicao = 4'b0101;
		#10;
		control = 1'b0;
		#10
		$finish;
	end

endmodule
