.module conflitos
.pseg
		;Algoritmo que faz multiplicacoes sucessivas por 3
MAIN:
		lcl r7, LOWBYTE ARR1
		lch r7, HIGHBYTE ARR1
		
		load r0, r7 ;carrega o n
		inca r7, r7
		
		loadlit r3, 2 ;numero=2
		loadlit r2, 0 ;i=0
LOOP:
		sub r4, r2, r0 ;verifica se i < n
		jf.neg FIM ;se i for maior ou igual a n, termina
		passa r6, r3 ;aux=num
		add r3, r3, r3 ;num=2*num
		add r3, r6, r3 ;num=num+aux
		store r7, r3
		inca r2, r2 ;i++
		j LOOP
FIM:
		j FIM ;ao final, o resultado estara no registrador r5
.dseg
ARR1:
	.word 5 ;quantidade n de iteracoes
.end