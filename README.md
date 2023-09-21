# cese-adm-workspace
Prácticas de la asignatura Arquitectura de Microprocesadores, dictada en el marco de la Carrera de Especialización en Sistemas Embebidos de la FIUBA

## Preguntas Orientadoras

### Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características
ARM presenta la siguiente familia de microcontroladores:
* `Cortex-A` (**A**pplication): Son dispositivos pensados para aplicaciones de alta performance, que normalmente corren sistemas operativos de propósito general (ie android/linux). Soportan instrucciones tanto de 32 como de 64 bits. Debido a esto es usual encontrarlos en dispositivos de consumo masivo como smartphones.
* `Cortex-R` (**R**eal-time): Estos están pensados para usarse en aplicaciones críticas (por ejemplo, en la industria automotriz). Su arquitectura es similar a la del cortex-A, pero incorporan mas herramientas de control de memoria y manejo de excepciones. Normalmente son dispositivos son dual-core que permiten correr en modo *lockstep*, en el que el mismo set de instrucciones corre en paralelo. Esta redundancia permite detectar y corregir errores, y hace el sistema más tolerante a fallos.
* `Cortex-M` (**M**icrocontroller): Son procesadores de 32 bits optimizados para correr aplicaciones específicas/dedicadas (es decir, no de uso general) en sistemas embebidos compaactos.

## Cortex-M

### Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y M4

* **Cortex-M0(+)**: Su arquitectura es la **ARMv6-M** (Von Neumann). El Cortex-M0+ es un set optimizado del Cortex-M0 original. Está pensado para aplicaciones relativamente sencillas de bajo consumo y bajo costo. Su pipeline de instrucciones es de 2 etapas.
* **Cortex-M3**: Su arquitectura es la **ARMV7-M** (Harvard). El pipeline es de 3 etapas, y la incorporación de un `Systick Timer` lo hace apto para correr RTOS.
* **Cortex-M4(F)**: Su Arquitectura es la **ARMv7E-M** (Harvard). Conceptualente, el Cortex-M4F es un M3 con el agregado de instrucciones de DSP y una unidad de punto flotante (FPU) de simple precisión.
* **Cortex-M7(F)**: Es la arquitectura más potente del segmento. Posee un pipeline de 6 etapas con *branch prediction*, y su FPU es capaz de trabajar tanto en simple como doble precision.

### Por qué se dice que el set the instrucciones Thumb permite mayor densidad de código?
Al contrario que su predecesor (el set de instrucciones ARM), las instrucciones Thumb se encuentran codificadas de una forma más compacta (usando 16 bits). Esto significa que la representación binaria de estas instrucciones es más pequeña, por lo que se ocupa menos memoria (instrucciones más pequeñas resultan en código más denso ya que la misma funcionalidad puede ser codificada con menos bytes).

Por otro lado, el set de instrucciones Thumb suele usar menos registros para las operaciones, a comparación del set de instrucciones ARM completo. Esto impacta en tener que alocar menos memoria para guardar/manipular registros.

Finalmente, el set de instrucciones Thumb soporta ejecución condicional, permitiendo que se pueda ejecutar una instrucción solo si una condición específica se cumple. Esta característica reduce la necesidad de branchear al mismo tiempo que el código resultante es más compacto.