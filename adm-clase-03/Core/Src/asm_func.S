.syntax unified
.thumb
.text

@ Simbolos exportados --> No olvidar de colocar los nombres de las funciones aquí
.global asm_svc
.global asm_sum
.global asm_zeros
.global asm_producto_escalar32

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_zeros (uint32_t * vector, uint32_t longitud);
@
@ Parametros recibidos:
@   r0: vector
@   r1: longitud
@
.thumb_func
    asm_zeros:
    	// Guardas de seguridad
    	cbz r0, exit
    	cbz r1, exit

    	mov r2, #0 // Cargo el valor 0 en r2

    	loop:
    		subs r1, #1 // Decremento en uno el "contador"
    		str r2, [r0, r1, LSL #2] // *(r0 + (r1<<2)) = r2
    		bne loop

    	exit:
	        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
	                    @ es la direccion, en el codigo del programa, de la
	                    @ instruccion "bl asm_sum" usada para saltar a
	                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
	                    @ el valor de retorno debe quedar en el registro r0.
	                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_producto_escalar32(uint32_t* vector_in, uint32_t* vector_out, uint32_t longitud, uint32_t escalar);
@
@ Parametros recibidos:
@   r0: vector_in
@   r1: vector_out
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_producto_escalar32:
    	// Guardas de seguridad. Tener mas en cuenta para divisiones por 0
    	cbz r0, exit_producto_escalar32
    	cbz r1, exit_producto_escalar32
    	cbz r2, exit_producto_escalar32
    	cbz r3, exit_producto_escalar32

    	push {r4, r5}

    	loop_producto_escalar32:
    		subs r2, #1 // Decremento en uno el "contador"

    		ldr r4, [r0, r2, LSL #2] // Guardo en r4 (auxiliar) *(r0 + (r2<<2)) = r4

    		//mul Rd, Rn, Rm -> Rd = Rn*Rm
    		mul r5, r4, r3 // r5 = r4*r3 (r3 ganancia)
    		str r5, [r1, r2, LSL #2] // *(r1 + (r2<<2)) = r6 (equivalente a decir que escribo el producto por un escalar en el vector de salida)

    		bne loop_producto_escalar32


		pop {r4, r5}
    	exit_producto_escalar32:
	        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
	                    @ es la direccion, en el codigo del programa, de la
	                    @ instruccion "bl asm_sum" usada para saltar a
	                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
	                    @ el valor de retorno debe quedar en el registro r0.
	                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}
