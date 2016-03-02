.module teste
.pseg
	loadlit r3, ARR1
	zeros r1
	ones r2
	add r0, r1, r2
	store r3, r0
HALT: j HALT
.dseg
ARR1:
        .word  -3
.end