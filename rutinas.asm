;Rutinas .asm

section .text
global contar_caracteres
global validar_movimiento
global calcular_puntaje
global detectar_objeto
global contar_celdas_libres

;int contar_caracteres(char* mapa, int total, char c)
;RCX = mapa, RDX = total, R8 = carcter
;Retorna en RAX el número de veces que aparece c

contar_caracteres:
    xor rax, rax    ;contador=0
    xor r9, r9      ;indice i=0

.loop:
    cmp r9, rdx     ;i >= total?
    jge .fin        ;si, termina loop

    movzx r10, byte [rcx + r9]  ;r10 = mapa[i]
    cmp r10, r8                 ;caracter buscado?
    jne .siguiente              ;no, saltar

    inc rax                     ;si, contador++

.siguiente:
    inc r9                      
    jmp .loop

.fin:
    ret


;int validar_movimiento(char* mapa, int cols, int fila, int col)
;RCX = mapa, RDX = cols, R8 = fila, R9 = col
;ret 1 si movimiento = valido, 0 si no

validar_movimiento:
    mov rax, r8         ;rax = fila
    imul rax, rdx       ;rax = fila * cols
    add rax, r9         ;rax = fila * cols + col

    ;Obtener carácter en esa posición
    movzx r10, byte [rcx + rax]     ;r10 = mapa[indice]

    ;verificar pared
    cmp r10, '#'
    je .bloqueado   ;si es pared, invalido

    mov rax, 1
    ret

.bloqueado
    xor rax, rax    ;retorna 0
    ret