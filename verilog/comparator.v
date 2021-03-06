module comparator (condicao, flags, control, salto);

	input [3:0] condicao;
	input [5:0] flags;
	input control;
	output reg salto;
	
	always @(condicao or flags or control) begin
		if (control == 1) begin
			case (condicao)
				4'b0001: salto = flags[0];
				4'b0010: salto = flags[1];
				4'b0011: salto = flags[2];
				4'b0100: salto = flags[3];
				4'b0101: salto = flags[4];
				4'b0110: salto = flags[5];
			endcase
		end else if (control == 0) begin
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