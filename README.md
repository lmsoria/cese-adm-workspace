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

### Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código?
Al contrario que su predecesor (el set de instrucciones ARM), las instrucciones Thumb se encuentran codificadas de una forma más compacta (usando 16 bits). Esto significa que la representación binaria de estas instrucciones es más pequeña, por lo que se ocupa menos memoria (instrucciones más pequeñas resultan en código más denso ya que la misma funcionalidad puede ser codificada con menos bytes).

Por otro lado, el set de instrucciones Thumb suele usar menos registros para las operaciones, a comparación del set de instrucciones ARM completo. Esto impacta en tener que alocar menos memoria para guardar/manipular registros.

Finalmente, el set de instrucciones Thumb soporta ejecución condicional, permitiendo que se pueda ejecutar una instrucción solo si una condición específica se cumple. Esta característica reduce la necesidad de branchear al mismo tiempo que el código resultante es más compacto.

### Qué entiende por arquitectura load-store? Qué tipo de instrucciones no posee este tipo de arquitectura?
Una arquitectura del tipo `load-store` se refiere a la forma en la que las instrucciones son ejecutadas. En una arquitectura `load-store`, la mayoría de operaciones aritmético-lógicas se ejecutan exclusivamente en los registros del CPU (ie `r0-r12`). Es decir, las operaciones no acceden a los datos almacenados en memoria directamente, si no que previamente deben haber sido movidos a alguno de los registros de uso general.

Por ejemplo, si se desea sumar dos números, primero deben almacenarse sus valores desde la memoria a registros, luego sumar sus valores y finalmente guardar este resultado de nuevo en memoria si fuera necesario.

### Cómo es el mapa de memoria de la familia?
Dado que estamos hablando de una arquitectura de 32 bits, se pueden direccionar `2^32 - 1` direcciones (4Gib) En el caso de ARM, esta región de memoria se distribuye para
* Memoria de programa
* Memoria de datos
* Periféricos, tanto propietarios como comunes (ie NVIC).

![Cortex-M MemMap](Resources/cortex-m-memory-map.jpg "Mapa de memoria")

### ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
Un "shadowed stack pointer" permite que el stack de código privilegiado y no privilegiado se encuentren en diferentes regiones de memoria, lo que termina mejorando la estabilidad/confiabilidad del sistema.

![Cortex-M Stacks](Resources/arm-arch-core-execmode-2.svg "Separación de stacks")

Las tareas no provilegiadas usarán el `PSP` (`Process Stack Pointer`) mientras que las privilegiadas (usualmente el scheduler/kernel de un RTOS o el mismo código baremetal) usará el `MSP` (`Main Stack Pointer`). De este modo, cuando ocurran excepciones/interrupciones el procesador puede usar directamente el `MSP` sin tener que preocuparse sobre manejar el `PSP`.

### Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y cómo se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.

El Cortex-M es una arquitectura que ofrece dos modos de privilegio y operación principales: el modo de Privilegio Principal (Privileged) y el modo de No Privilegio (Unprivileged). Estos modos permiten controlar el acceso a ciertas funciones y recursos críticos del sistema.

En el modo privilegiado, el procesador tiene acceso completo a todos los recursos del sistema y a todas las instrucciones.Por otro lado, el código que se ejecuta en este modo generalmente es el kernel/scheduler del sistema operativo, por lo que es razonable que pueda acceder a instrucciones de configuración de interrupciones, funciones de administración de energía y registros de control especiales.

En contraparte, en el modo no privilegiado el procesador tiene un acceso limitado a los recursos y no puede ejecutar ciertas instrucciones. El código en este modo suele ser el código de las tareas de usuario, que no necesita acceso a recursos críticos.

![Cortex-M Stacks](Resources/operation-states-and-modes.png "Modos de operación")

Tanto el modo privilegiado como el no privilegiado son parte de lo que en la arquitectura se conoce como `Thread Mode`. El cambio entre un modo y otro se hace modificando un registro especial denominado `CONTROL`.

Además de este modo Thread, se encuentra el `Handler Mode`, el cual ocurre cuando una excepción se dispara. En este modo, el procesador siempre tiene un nivel de acceso privilegiado.

Una vez que el procesador entra en modo Thread no privilegiado, no puede regresar directamente al modo privilegiado. Para que esto suceda, debe primero ocurrir una excepción (por ejemplo la del SysTick o una SVC, Supervisor Call) de modo que el procesador vuelva a tener los privilegios necesarios como para modificar el registro `CONTROL`.

![Cortex-M Stacks](Resources/change-of-mode.png "Cambio de modo entre thread privilegiado y thread no privilegiado")
