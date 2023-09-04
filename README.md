# cese-adm-workspace
Prácticas de la asignatura Arquitectura de Microprocesadores, dictada en el marco de la Carrera de Especialización en Sistemas Embebidos de la FIUBA

## Preguntas Orientadoras

### Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características
ARM presenta la siguiente familia de microcontroladores:
* `Cortex-A` (**A**pplication): Son dispositivos pensados para aplicaciones de alta performance, que normalmente corren sistemas operativos de propósito general (ie android/linux). Soportan instrucciones tanto de 32 como de 64 bits. Debido a esto es usual encontrarlos en dispositivos de consumo masivo como smartphones.
* `Cortex-R` (**R**eal-time): Estos están pensados para usarse en aplicaciones críticas (por ejemplo, en la industria automotriz). Su arquitectura es similar a la del cortex-A, pero incorporan mas herramientas de control de memoria y manejo de excepciones. Normalmente son dispositivos son dual-core que permiten correr en modo *lockstep*, en el que el mismo set de instrucciones corre en paralelo. Esta redundancia permite detectar y corregir errores, y hace el sistema más tolerante a fallos.
* `Cortex-M` (**M**icrocontroller): Son procesadores de 32 bits optimizados para correr aplicaciones específicas/dedicadas (es decir, no de uso general) en sistemas embebidos compaactos.