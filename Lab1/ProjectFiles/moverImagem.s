		AREA    |.text|, CODE, READONLY, ALIGN=2

f_asm
        GLOBAL f_asm
		EXTERN ImageMemory
		EXTERN Direction

        THUMB

        push {r0-r10}
        ldr r0,=ImageMemory
        ldr r9,=Direction
        ldr r8, [r9,#0]
        cmp r8,#0
        beq MOVER_ESQUERDA
        cmp r8,#1
        beq MOVER_DIREITA
        cmp r8,#2
        beq MOVER_BAIXO
        cmp r8,#3
        beq MOVER_CIMA
        pop {r0-r10}
        bx lr       



MOVER_BAIXO
        mov r1, r0
        mov r7, #0

LOOP_PRINCIPAL_BAIXO
        add r4, r1, #127
        ldrb r3, [r4, #0]
            
DESLOCA_POSICOES_BAIXO
        sub r4, r4, #1
        ldrb r5, [r4, #0]
        strb r5, [r4, #1]
        cmp r4, r1
        bne DESLOCA_POSICOES_BAIXO
        strb r3, [r1, #0]
        
        add r1, #128
        add r7, #1
        cmp r7, #128
        bne LOOP_PRINCIPAL_BAIXO
        pop {r0-r10}
        bx lr
        
        
MOVER_CIMA
		mov r1, r0	;salva a posicao da imagen em r1
        mov r7, #0	; r7 é a linha

LOOP_PRINCIPAL_CIMA
        add r4, r1, #0	;R4 é a primeira posicao. Salvar para recarregar no final da linha
        ldrb r3, [r4, #0] ; R3 tem o valor da primeira posicao salvo
            
DESLOCA_POSICOES_CIMA
        add r4, r4, #1
        ldrb r5, [r4, #0]
        strb r5, [r4, #-1]
        add r6, r1, #127	;indica a posicao de memoria que corresponde ao final da linha atual
		cmp r4, r6		;verifica se r4 chegou ao final da linha
        bne DESLOCA_POSICOES_CIMA
        strb r3, [r1, #127]	; carrega o valor salvo na ultima coluna da linha
        
        add r1, #128
        add r7, #1
        cmp r7, #128
        bne LOOP_PRINCIPAL_CIMA
        pop {r0-r10}
        bx lr
        
		
MOVER_ESQUERDA
		mov r1, r0	;salva a posicao da imagen em r1
        mov r7, #0	; r7 é a linha

LOOP_PRINCIPAL_ESQUERDA
        add r4, r1, #0	;R4 é a primeira posicao. Salvar para recarregar no final da linha
        ldrb r3, [r4, #0] ; R3 tem o valor da primeira posicao salvo
            
DESLOCA_POSICOES_ESQUERDA
        add r4, r4, #128
        ldrb r5, [r4, #0]
        strb r5, [r4, #-128]
        add r6, r1, #(127*128)	;indica a posicao de memoria que corresponde ao final da linha atual
	cmp r4, r6		;verifica se r4 chegou ao final da linha
        bne DESLOCA_POSICOES_ESQUERDA
	mov r8, #(127*128)
        strb r3, [r1, r8]	; carrega o valor salvo na ultima coluna da linha
        
        add r1, #1
        add r7, #1
        cmp r7, #128
        bne LOOP_PRINCIPAL_ESQUERDA
        pop {r0-r10}
        bx lr
        
		
MOVER_DIREITA
		mov r1, r0	;salva a posicao da imagen em r1
        mov r7, #0	; r7 é a linha

LOOP_PRINCIPAL_DIREITA
        add r4, r1, #(127*128)	;R4 é a primeira posicao. Salvar para recarregar no final da linha
        ldrb r3, [r4, #0] ; R3 tem o valor da primeira posicao salvo
            
DESLOCA_POSICOES_DIREITA
        sub r4, r4, #128
        ldrb r5, [r4, #0]
        strb r5, [r4, #128]
	cmp r4, r1		;verifica se r4 chegou ao final da linha
        bne DESLOCA_POSICOES_DIREITA
        strb r3, [r1, #0]	; carrega o valor salvo na ultima coluna da linha
        
        add r1, #1
        add r7, #1
        cmp r7, #128
        bne LOOP_PRINCIPAL_DIREITA
        pop {r0-r10}
        bx lr
 
        END