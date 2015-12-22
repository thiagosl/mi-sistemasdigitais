.module binarysearch
.pseg
		;Algoritmo de Busca Binária iterativa aplicado a um vetor
		;Os dois primeiros elementos do vetor são respectivamente o número buscado e o tamanho do vetor
		;Estes dois valores não ficarão ordenados ao final do processo, somente os valores que de fato fazem parte do vetor de dados
MAIN:
		lcl r0, LOWBYTE ARR1
		lch r0, HIGHBYTE ARR1 ;carrega endereço base do vetor
		
		load r1, r0 ;r1 tem o elemento que será buscado
		inca r0, r0 ;r0 aponta para a proxima palavra
		load r2, r0 ;r2 tem o tamanho do array
		inca r0, r0 ;r0 aponta para a proxima palavra
		
		zeros r3 ;r3 tem o limite inferior
		deca r4, r2 ;r4 tem o limite superior
		zeros r5 ;registrador da variável "meio"
		loadlit r6, -1 ;resultado, -1 quer dizer que não foi encontrado
LOOP:
		sub r8, r3, r4 ;r8 não é utilizado
		jf.negzero FIM ;se o limite inferior for maior que o limite superior, termina
		add r9, r3, r4 ;soma os limites superior e inferior
		lsr r5, r9 ;faz um shift a direita (dividi por 2 e no caso de número impar também faz o floor do resultado)
		add r10, r5, r0 ;adiciona o endereço base do vetor ao valor da variável meio (grava em r10 o endereço em que a posição "meio" do vetor está)
		load r12, r10 ;carrega em r12 o valor que está na posição "meio" do vetor
		sub r13, r1, r12 ;r13 não é utilizado
		jt.zero IF1 ;se o elemento que está sendo buscado for igual ao valor que estava na posição "meio", pula para IF1
		sub r13, r1, r12 ;r13 não é utilizado
		jt.neg IF2 ;se o elemento que está sendo buscado for menor que o valor que estava na posição "meio", pula para IF2
		inca r3, r5 ;se o elemento for maior do que o valor que estava na posição "meio", faz limite inferior receber (meio+1)
		j LOOP ;volta ao loop
IF1:
		passa r6, r5 ;passa a posição em que esta o elemento buscado para o registrador que armazena o resultado
		j FIM ;termina
IF2:
		deca r4, r5 ;limite superior recebe (meio-1)
		j LOOP ;volta ao loop
FIM:
		j FIM ;ao terminar o resultado está no registrador r6
.dseg
ARR1:
	.word -21
	.word 10
		.word -21
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