.module bubblesort
.pseg
		;Algoritmo de ordenação Bubble Sort aplicado a um vetor
		;O primeiro elemento do vetor é o tamanho do mesmo e não estará ordenado ao final do processo
		;Somente estarão ordenados os valores que de fato fazem parte do vetor de dados
MAIN:
		lcl r0, LOWBYTE ARR1
		lch r0, HIGHBYTE ARR1 ;r0 tem o endereço base do array
		load r1, r0 ;r1 passa a armazenar o tamanho do array
		inca r0, r0 ;faz r0 apontar uma palavra adiante, não considera o tamanho do array durante a ordenação
		zeros r2 ;i = 0
		zeros r6
LOOP:
		passa r4, r2; j = i
		sub r3, r2, r1 ;r3 não é utilizado
		jt.neg LOOP2 ;se i for menor que o tamanho do array vai para o LOOP2
		j FIM ;se i for maior igual que o tamanho do array termina a ordenação
INC:
		inca r2, r2 ;i++
		j LOOP ;volta para o LOOP1
LOOP2:
		sub r5, r4, r6 ;r5 não é utilizado
		jt.zero INC ;se j for igual a zero, pula para INC
		;se j for maior que zero, continua
		deca r7, r4 ;r7 = j - 1
		add r7, r7, r0 ;endereço (j-1)+base
		add r11, r4, r0 ;endereço j+base
		load r9, r7 ;carrega valor que está na posição (j-1)+base
		load r10, r11 ;carrega valor que está na posição j+base
		sub r12, r9, r10 ;r12 não é utilizado
		jf.neg TROCA ;se valor da posição (j-1) for maior que o da posição j do vetor, pula para TROCA
		;se não, continua
DEC:
		deca r4, r4 ;j--
		j LOOP2 ;volta para o LOOP2
TROCA:
		store r11, r9 ;grava valor que estava na posição (j-1) na posição j do array
		store r7, r10 ;grava valor que estava na posição j na posição (j-1) do array
		j DEC
FIM:
		j FIM
.dseg
ARR1:
	.word 10
		.word   -1
            	.word   6
                .word   3
                .word   -2
                .word   4
                .word   0
                .word   -3
                .word   5
                .word   1
                .word   2
	.end
