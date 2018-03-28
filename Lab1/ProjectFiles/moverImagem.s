		AREA    |.text|, CODE, READONLY, ALIGN=2

f_asm
        GLOBAL f_asm
		EXTERN ImageMemory
		EXTERN Direction

        THUMB

        ldr r0,=ImageMemory
		ldr r9,=Direction
		ldr r8, [r9,#0]
		cmp r8,#0
		beq MOVER_ESQUERDA
		cmp r8,#1
		beq MOVER_DIREITA
		cmp r8,#2
		beq MOVER_CIMA
		cmp r8,#3
		beq MOVER_BAIXO
		
        bx lr		



MOVER_BAIXO
		mov r1, r0
		mov r7, #0

LOOP_PRINCIPAL_BAIXO
		add r4, r1, #127
		ldr r3, [r4, #0]
		
		;r4 = índice da posição atual		
DESLOCA_POSICOES_BAIXO
		sub r4, r4, #1
		ldr r5, [r4, #0]
		str r5, [r4, #1]
		cmp r4, r1
		bne DESLOCA_POSICOES_BAIXO
		str r3, [r1, #0]
		
		add r1, #128
		add r7, #1
		cmp r7, #128
		bne LOOP_PRINCIPAL_BAIXO
 
        bx lr
		
		
		
MOVER_CIMA
		mov r1, r0
		mov r7, #0

LOOP_PRINCIPAL_CIMA
		add r4, r1, #0
		ldr r3, [r4, #0]
		add r2, r1, #127
		
		;r4 = índice da posição atual		
DESLOCA_POSICOES_CIMA
		add r4, r4, #1
		ldr r5, [r4, #1]
		str r5, [r4, #0]
		cmp r4, r2
		bne DESLOCA_POSICOES_CIMA
		str r3, [r2, #0]
		
		add r1, #128
		add r7, #1
		cmp r7, #128
		bne LOOP_PRINCIPAL_CIMA
 
        bx lr
		
		
		
MOVER_ESQUERDA
		mov r1, r0
		mov r7, #0

LOOP_PRINCIPAL_ESQUERDA
		add r4, r1, #127
		ldr r3, [r4, #0]
		
		;r4 = índice da posição atual		
DESLOCA_POSICOES_ESQUERDA
		sub r4, r4, #1
		ldr r5, [r4, #0]
		str r5, [r4, #1]
		cmp r4, r1
		bne DESLOCA_POSICOES_ESQUERDA
		str r3, [r1, #0]
		
		add r1, #128
		add r7, #1
		cmp r7, #128
		bne LOOP_PRINCIPAL_ESQUERDA
 
        bx lr
		
		
		
MOVER_DIREITA
		mov r1, r0
		mov r7, #0

LOOP_PRINCIPAL_DIREITA
		add r4, r1, #127
		ldr r3, [r4, #0]
		
		;r4 = índice da posição atual		
DESLOCA_POSICOES_DIREITA
		sub r4, r4, #1
		ldr r5, [r4, #0]
		str r5, [r4, #1]
		cmp r4, r1
		bne DESLOCA_POSICOES_DIREITA
		str r3, [r1, #0]
		
		add r1, #128
		add r7, #1
		cmp r7, #128
		bne LOOP_PRINCIPAL_DIREITA
 
        bx lr
 
        END