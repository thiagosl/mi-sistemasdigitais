.module bubblesort
.pseg
MAIN:
		lcl r0, LOWBYTE ARR1
		lch r0, HIGHBYTE ARR1 ;r0 tem o endereço base do array
		load r1, r0 ;r1 passa a armazenar o tamanho do array
		loadlit r15, 4
		add r0, r0, r15 ;faz r0 apontar uma palavra adiante, não considera o tamanho do array durante a ordenação
		zeros r2 ;i = 0
		zeros r6
		passa r4, r2; j = i
LOOP:
		sub r3, r2, r1 ;r3 não é utilizado
		jt.neg LOOP2
		j FIM
INC:
		inca r2, r2 ;i++
		passa r4, r2 ;j = i
		j LOOP
LOOP2:
		sub r5, r4, r6
		jt.zero INC
		;dentro do for 2
		deca r7, r4;r7 = j - 1
		add r7, r7, r0 ;endereço (j-1)+base
		add r11, r4, r0 ;endereço j+base
		load r9, r7 ;carrega valor que está na posição (j-1)+base
		load r10, r11 ;carrega valor que está na posição j+base
		sub r12, r9, r10
		jf.neg TROCA
DEC:
		deca r4, r4 ;j--
		j LOOP2
TROCA:
		store r9, r11 ;grava valor armazenado em j-1 em j
		store r10, r7 ;grava valor armazenado em j em j-1
		j DEC
FIM:
		j FIM
.dseg
ARR1:
	.word 10
		.word 3
		.word 2
		.word 10
		.word 9
		.word 1
		.word 4
		.word 7
		.word 8
		.word 5
		.word 6
.end