.syntax unified
.thumb
.text

@ Simbolos exportados --> No olvidar de colocar los nombres de las funciones aquí
.global asm_svc
.global asm_sum
.global asm_zeros
.global asm_producto_escalar32
.global asm_producto_escalar16
.global asm_producto_escalar12
.global asm_max

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
	        bx lr

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
    	push {r4}

    	loop_producto_escalar32:
    		subs r2, #1 // Decremento en uno el "contador"

    		ldr r4, [r0, r2, LSL #2] // Guardo en r4 (auxiliar) *(r0 + (r2<<2)) = r4

    		//mul Rd, Rn, Rm -> Rd = Rn*Rm
    		mul r4, r4, r3 // r4 = r4*r3 (r3 ganancia)
    		str r4, [r1, r2, LSL #2] // *(r1 + (r2<<2)) = r4 (equivalente a decir que escribo el producto por un escalar en el vector de salida)

    		bne loop_producto_escalar32

		pop {r4}
    	exit_producto_escalar32:
	        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@    void asm_producto_escalar16(uint16_t* vector_in, uint16_t* vector_out, uint16_t longitud, uint16_t escalar);
@
@ Parametros recibidos:
@   r0: vector_in
@   r1: vector_out
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_producto_escalar16:
    	push {r4}

    	loop_producto_escalar16:
    		subs r2, #1 // Decremento en uno el "contador"

    		ldrh r4, [r0, r2, LSL #1] // Guardo en r4 (auxiliar) *(r0 + (r2<<1)) = r4

    		//mul Rd, Rn, Rm -> Rd = Rn*Rm
    		mul r4, r4, r3 // r4 = r4*r3 (r3 ganancia)
    		strh r4, [r1, r2, LSL #1] // *(r1 + (r2<<2)) = r4 (equivalente a decir que escribo el producto por un escalar en el vector de salida)

    		bne loop_producto_escalar16


		pop {r4}
    	exit_producto_escalar16:
	        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@    void asm_producto_escalar16(uint16_t* vector_in, uint16_t* vector_out, uint16_t longitud, uint16_t escalar);
@
@ Parametros recibidos:
@   r0: vector_in
@   r1: vector_out
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_producto_escalar12:
    	push {r4}

    	loop_producto_escalar12:
    		subs r2, #1 // Decremento en uno el "contador"

    		ldrh r4, [r0, r2, LSL #1] // Guardo en r4 (auxiliar) *(r0 + (r2<<2)) = r4

    		//mul Rd, Rn, Rm -> Rd = Rn*Rm
    		mul r4, r4, r3 // r4 = r4*r3 (r3 ganancia)
    		usat16 r4, #12, r4 // Saturo en 12 bits [0-4095]
    		strh r4, [r1, r2, LSL #1] // *(r1 + (r2<<2)) = r4 (equivalente a decir que escribo el producto por un escalar en el vector de salida)

    		bne loop_producto_escalar12

		pop {r4}
    	exit_producto_escalar12:
	        bx lr

 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_max(int32_t* vector_in, uint32_t longitud);
@
@ Parametros recibidos:
@   r0: vector_in
@   r1: longitud
@ Valor de retorno:
@   r0: maximo elemento del array
@
.thumb_func
    asm_max:
    	// En r2 voy a guardar el maximo. Arranco inicializandolo con el ultimo elemento del array
    	subs r1, #1 // Decremento en uno el "contador"
    	ldr r2, [r0, r1, LSL #2] // *(r0 + (r1<<2)) = r2

    	loop_max:
    		subs r1, #1 // Decremento en uno el "contador"
    		ldr r3, [r0, r1, LSL #2] // *(r0 + (r1<<2)) = r3 (uso r3 para recorrer el array)

    		cmp r3, r2
    		it gt
			movgt r2, r3
			cbz r1, exit_max
    		b loop_max

    	exit_max:
    		mov r0, r2
	        bx lr
