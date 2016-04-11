module control_unit_tb;

	reg [23:0] code;
	wire [1:0] mux1, mux5, mux6;
	wire [2:0] regs_bank;
	wire mux4, data_mem, comparator, write_inst;
	wire [5:0] alu;

	integer i, qtd_validos;
	integer arquivo;

	control_unit DUT(code, mux1, mux5, mux6, regs_bank, mux4, data_mem, alu, comparator, write_inst);

	initial begin
		i=1;
		qtd_validos = 0;
		arquivo = $fopen("teste_control_unit.out");

		while (i < 100000) begin
			
			code = $random;
			
			#1000
			case (code[23:22])
				2'b00: begin							//Logico-Aritmetica
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d:", i);
					$fwrite(arquivo, "code = %b;", code);
					$fwrite(arquivo, "mux1 = %b; mux5 = %b; mux6 = %b; regs_bank = %d; mux4 = %b; data_mem = %b; comparator = %b; write_inst = %b; alu = %b;", mux1, mux5, mux6, regs_bank,mux4, data_mem, comparator, write_inst, alu);
					$fwrite(arquivo, "===========================================================================================================================");
				end
				2'b01: begin							//Carga de Constante
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d:", i);
					$fwrite(arquivo, "code = %b;", code);
					$fwrite(arquivo, "mux1 = %b; mux5 = %b; mux6 = %b; regs_bank = %d; mux4 = %b; data_mem = %b; comparator = %b; write_inst = %b; alu = %b;", mux1, mux5, mux6, regs_bank,mux4, data_mem, comparator, write_inst, alu);
					$fwrite(arquivo, "===========================================================================================================================");
				end
				2'b10: begin							//Acesso a Memoria
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d:", i);
					$fwrite(arquivo, "code = %b;", code);
					$fwrite(arquivo, "mux1 = %b; mux5 = %b; mux6 = %b; regs_bank = %d; mux4 = %b; data_mem = %b; comparator = %b; write_inst = %b; alu = %b;", mux1, mux5, mux6, regs_bank,mux4, data_mem, comparator, write_inst, alu);
					$fwrite(arquivo, "===========================================================================================================================");
				end
				2'b11: begin							//Transferencia de Controle
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d:", i);
					$fwrite(arquivo, "code = %b;", code);
					$fwrite(arquivo, "mux1 = %b; mux5 = %b; mux6 = %b; regs_bank = %d; mux4 = %b; data_mem = %b; comparator = %b; write_inst = %b; alu = %b;", mux1, mux5, mux6, regs_bank,mux4, data_mem, comparator, write_inst, alu);
					$fwrite(arquivo, "===========================================================================================================================");
				end
			endcase
			
			i = i + 1;
		end
		
		$fwrite(arquivo, "Quantidade de Operacoes:         %d", i);
		$fwrite(arquivo, "Quantidade de Operacoes Validas: %d", qtd_validos);
		$flose(arquivo);
		$finish;
	end

endmodule;
