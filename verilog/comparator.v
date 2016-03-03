
module comparator (condicao, flags, control, salto);

	input [3:0] condicao;
	input [5:0] flags;
	input control;
	output reg salto;
	
	always @(condicao or flags or control) begin
		if (control) begin
			case (condicao)
				4'b0001: salto = flags[0];
				4'b0010: salto = flags[1];
				4'b0011: salto = flags[2];
				4'b0100: salto = flags[3];
				4'b0101: salto = flags[4];
				4'b0110: salto = flags[5];
			endcase
		end else begin
			case (condicao)
				4'b0001: salto = ~flags[0];
				4'b0010: salto = ~flags[1];
				4'b0011: salto = ~flags[2];
				4'b0100: salto = ~flags[3];
				4'b0101: salto = ~flags[4];
				4'b0110: salto = ~flags[5];
			endcase
		end
	end

endmodule


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
