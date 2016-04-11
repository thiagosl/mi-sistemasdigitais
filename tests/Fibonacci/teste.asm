.module fibonacci
.pseg
		;Algoritmo da Sequência Fibonacci iterativo
MAIN:
		lcl r0, LOWBYTE ARR1
		lch r0, HIGHBYTE ARR1
		inca r4, r0 ;proximo endereço de memória
		load r0, r0 ;carrega número n para realizar o fibonacci
		
		loadlit r14, 1 ;f(1) = 1
		store r4, r14
		inca r4, r4
		
		loadlit r7, 1 ;f(2) = 1
		store r4, r7
		inca r4, r4
		
		loadlit r1, 2 ;i = 2
LOOP:
		sub r2, r1, r0 ;verifica se i < n
		jf.neg FIM ;se i for maior ou igual a n, termina
		add r3, r14, r7 ;se i for menor que n, adiciona os dois numeros correspondentes ao fibonacci de i-1 e i-2 e coloca em r3
		passa r14, r7 ;f(i-2) = f(i-1)
		passa r7, r3 ;f(i-1) = soma em r3
		store r4, r7
		inca r4, r4
		inca r1, r1 ;i++
		j LOOP
FIM:
		j FIM ;ao final do processamento o resultado estará armazenado no registrador r7
.dseg
ARR1:
	.word 8
.end
