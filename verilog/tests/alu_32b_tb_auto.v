module alu_32b_tb;
	
	reg[31:0] operandA, operandB;
	reg[5:0] operation;
	wire[31:0] result;
	wire[5:0] flags;
 	
 	integer i, qtd_validos;
 	integer arquivo;

	alu_32b DUT(operandA, operandB, operation, result, flags);
	
	reg[31:0] operandoA, operandoB;
	reg[5:0] operacao;
	wire[31:0] resultado;
	
	verificar verificar1(operandoA, operandoB, operacao, resultado);
			
	initial begin
		i=1;	
		qtd_validos = 0;

		arquivo = $fopen("teste_alu.out");
		$display("===========================================================================================================================");
		while(i < 100000) begin

			operandA = $random;
			operandB = $random;
			operation = $random;
			
			operandoA = operandA;
			operandoB = operandB;
			operacao = operation;
	
			#1000
			case(operation)
				
				//ADD
				6'b000000:																				
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ADD", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ADD INC
				6'b000010:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ADD INC", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end
				
				//INCA
				6'b000011:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: INCA", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end
				
				//SUB
				6'b000100:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: SUB", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//SUB DEC
				6'b000101:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: SUB DEC", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//DECA
				6'b000110:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: DECA", i);
					$fwrite(arquivo, "operandA = %b; operation = %b;", operandA, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//PASSB
				6'b001001:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: PASSB", i);
					$fwrite(arquivo, "operandA = %b; operation = %b;", operandA, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ASL
				6'b001101:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ASL", i);
					$fwrite(arquivo, "operandA = %b; operation = %b;", operandA, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ASR
				6'b001110:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ASR", i);
					$fwrite(arquivo, "operandA = %b; operation = %b;", operandA, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ZEROS
				6'b001111:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ZEROS", i);
					$fwrite(arquivo, "operation = %b;", operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ONES
				6'b010000:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ONES", i);
					$fwrite(arquivo, "operation = %b;", operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//PASSA
				6'b010001:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: PASSA", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end


				//PASSNOTA
				6'b010010:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: PASSNOTA", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//AND
				6'b010011:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: AND", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ANDNOTA
				6'b010100:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ANDNOTA", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//NAND
				6'b010101:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: NAND", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//OR
				6'b010110:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: OR", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//ORNOTA
				6'b010111:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: ORNOTA", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//NOR
				6'b011000:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: NOR", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//XOR
				6'b011001:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: XOR", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				///XORNOTA
				6'b011010:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: XORNOTA", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//XNOR
				6'b011011:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: XNOR", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//LSL
				6'b011100:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: LSL", i);
					$fwrite(arquivo, "operandA = %b; operation = %b;", operandA, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//LSR
				6'b011101:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: LSR", i);
					$fwrite(arquivo, "operandA = %b; operation = %b;", operandA, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//LCL
				6'b011110:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: LCL", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//LCH
				6'b011111:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: LCH", i);
					$fwrite(arquivo, "operandA = %b; operandB = %b; operation = %b;", operandA, operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end

				//LOADLIT
				6'b100000:
				begin
					qtd_validos = qtd_validos + 1;
					$fwrite(arquivo, "Iteration %d: LOADLIT", i);
					$fwrite(arquivo, "operandB = %b; operation = %b;", operandB, operation);
					$fwrite(arquivo, "result = %b; flags = %b;", result, flags);
					if(result==resultado) begin
						$fwrite(arquivo, "Verdadeiro");
					end
					else begin
						$fwrite(arquivo, "Falso");
					end
					$fwrite(arquivo, "===========================================================================================================================");
				end
			endcase
				
			i = i + 1;
		end

		$fwrite(arquivo, "Quantidade de Operacoes:         %d", i);
		$fwrite(arquivo, "Quantidade de Operacoes Validas: %d", qtd_validos);
		$fwrite(arquivo, "===========================================================================================================================");
		$flose(arquivo);
		$finish;
	end 

endmodule;
