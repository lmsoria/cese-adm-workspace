#pragma once

#include <stdint.h>


void asm_svc (void);

uint32_t asm_sum (uint32_t firstOperand, uint32_t secondOperand);

// 1) Realizar una función que inicialice un vector con ceros.
void asm_zeros (uint32_t * vector, uint32_t longitud);

// 2) Realizar una función que realice el producto de un vector y un escalar (por ejemplo, podría servir
// para cambiar el nivel de amplitud de una señal)
void asm_producto_escalar32(uint32_t* vector_in, uint32_t* vector_out, uint32_t longitud, uint32_t escalar);

// 3) Adapte la función del ejercicio 2 para realizar operaciones sobre vectores de 16 bits
void asm_producto_escalar16(uint16_t* vector_in, uint16_t* vector_out, uint16_t longitud, uint16_t escalar);

// 4) Adapte la función del ejercicio 3 para saturar el resultado del producto a 12 bits
void asm_producto_escalar12(uint16_t* vector_in, uint16_t* vector_out, uint16_t longitud, uint16_t escalar);

// 5) Realice una función que implemente un filtro de ventana móvil de 10 valores sobre un vector de muestras
void asm_filtro_ventana10(uint16_t* vector_in, uint16_t* vector_out, uint32_t longitud_vector_in);

// 6) Realizar una función que reciba un vector de números signados de 32 bits y los “empaquete” en
// otro vector de 16 bits. La función deberá adecuar los valores de entrada a la nueva precisión
void asm_pack32_to_16(int32_t* vector_in, int16_t* vector_out, uint32_t longitud);

// 7) Realizar una función que reciba un vector de números signados de 32 bits y devuelva la posición
// del máximo del vector.
int32_t asm_max(int32_t* vector_in, uint32_t longitud);

// 8) Realizar una función que reciba un vector de muestras signadas de 32 bits y lo decime
// descartando una cada N muestras.
void asm_downsample_N(int32_t* vector_in, int32_t* vector_out, uint32_t longitud, uint32_t N);

// 9) Realizar una función que reciba un vector de muestras no signadas de 16 bits e invierta su orden.
void asm_invertir(uint16_t* vector, uint32_t longitud);


void asm_mean(uint16_t* x, uint16_t* y, uint16_t* z, uint16_t N);

void asm_mean_simd(uint16_t* x, uint16_t* y, uint16_t* z, uint16_t N);
