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

; int detectar_objeto(char* mapa, int cols, int fila, int col, char obj)
; RCX = mapa, RDX = cols, R8 = fila, R9 = col, [RSP+40] = obj
; Retorna 1 si el objeto está en esa celda, 0 si no

detectar_objeto:
    ; El 5to parámetro no cabe en registro, va en la pila
    ; En Windows x64: [RSP+40] después del prólogo
    movzx r10, byte [rsp+40]  ; r10 = obj (5to parámetro)

    ; Calcular índice: fila * cols + col
    mov  rax, r8         ; rax = fila
    imul rax, rdx        ; rax = fila * cols
    add  rax, r9         ; rax = fila * cols + col

    ; Obtener carácter en esa posición
    movzx r11, byte [rcx + rax]  ; r11 = mapa[índice]

    ; Comparar con el objeto buscado
    cmp  r11, r10
    je   .encontrado

    xor  rax, rax        ; no encontrado → 0
    ret

.encontrado:
    mov  rax, 1          ; encontrado → 1
    ret

; int contar_celdas_libres(char* mapa, int total)
; RCX = mapa, RDX = total
; Retorna en RAX el número de celdas libres ('.')

contar_celdas_libres:
    xor  rax, rax        ; contador = 0
    xor  r9,  r9         ; índice i = 0

.loop:
    cmp  r9, rdx         ; i >= total?
    jge  .fin            ; si sí, terminamos

    movzx r10, byte [rcx + r9]  ; r10 = mapa[i]
    cmp  r10, '.'        ; ¿es celda libre?
    jne  .siguiente      ; no → saltar

    inc  rax             ; sí → contador++

.siguiente:
    inc  r9              ; i++
    jmp  .loop

.fin:
    ret
; int calcular_puntaje(int monedas, int pasos, int niveles)
; RCX = monedas, RDX = pasos, R8 = niveles
; Fórmula: (monedas * 100) - (pasos * 2) + (niveles * 500)
; Retorna en RAX el puntaje final

calcular_puntaje:
    ; monedas * 100
    mov  rax, rcx        ; rax = monedas
    imul rax, 100        ; rax = monedas * 100

    ; pasos * 2
    mov  r9,  rdx        ; r9 = pasos
    imul r9,  2          ; r9 = pasos * 2

    ; restar pasos
    sub  rax, r9         ; rax = (monedas * 100) - (pasos * 2)

    ; niveles * 500
    mov  r10, r8         ; r10 = niveles
    imul r10, 500        ; r10 = niveles * 500

    ; sumar niveles
    add  rax, r10        ; rax = resultado final

    ; evitar puntaje negativo
    cmp  rax, 0
    jge  .fin
    xor  rax, rax        ; si es negativo, retorna 0

.fin:
    ret