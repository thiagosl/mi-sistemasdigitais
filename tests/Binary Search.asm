.module bubblesort
.pseg
MAIN:
		lcl r0, LOWBYTE ARR1
		lch r0, HIGHBYTE ARR1
		loadlit r15, 4
		
		load r1, r0 ;r1 tem o elemento que será buscado
		add r0, r0, r15 ;r0 aponta para a proxima palavra
		load r2, r0 ;r2 tem o tamanho do array
		add r0, r0, r15 ;r0 aponta para a proxima palavra
		
		zeros r3 ;r3 tem o limite inferior
		deca r4, r2 ;r4 tem o limite superior
		zeros r5 ;registrador da variável "meio"
		loadlit r6, -1 ;resultado, -1 quer dizer que não foi encontrado
LOOP:
		sub r8, r3, r4
		jf.negzero FIM
		add r9, r3, r4
		lsr r5, r9
		add r10, r5, r0
		load r12, r10
		sub r13, r1, r12
		jt.zero IF1
		sub r13, r1, r12
		jt.neg IF2
		inca r3, r5
		j LOOP
IF1:
		passa r6, r5
		j FIM
IF2:
		deca r4, r5
		j LOOP
FIM:
		j FIM
.dseg
ARR1:
	.word 7
	.word 10
		.word 2
		.word 4
		.word 5
		.word 7
		.word 8
		.word 9
		.word 11
		.word 14
		.word 15
		.word 17
.end