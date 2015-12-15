.module fibonacci
.pseg
		;Algoritmo da Sequencia Fibonacci iterativo
MAIN:
		lcl r0, LOWBYTE ARR1
		lch r0, HIGHBYTE ARR1
		load r0, r0 ;carrega numero n para realizar o fibonacci
		loadlit r14, 1 ;f(1) = 1
		loadlit r15, 1 ;f(2) = 1
		loadlit r1, 2 ;i = 2
LOOP:
		sub r2, r1, r0 ;verifica se i < n
		jf.neg FIM ;se i for maior ou igual a n, termina
		add r3, r14, r15 ;se i for menor que n, adiciona os dois numeros correspondentes ao fibonacci de i-1 e i-2 e coloca em r3
		passa r14, r15 ;f(i-2) = f(i-1)
		passa r15, r3 ;f(i-1) = soma em r3
		inca r1 ;i++
		j LOOP
FIM:
		j FIM ;ao final do processamento o resultado estara armazenado no registrador r15
.dseg
ARR1:
	.word 4
.end