INTEGRANTES:
Karlo Eduardo Ayala Salazar
Cinthia America Blanco Rodrigo
Dylan Edward Davila Grau

SIMULACION DE SUMA DE MATRICES NxN (Secuencial y Multithread)
Este proyecto implementa una simulación para sumar los elementos de matrices cuadradas de tamaños desde 1x1 hasta NxN. Se comparan dos enfoques: programación secuencial y programación con hilos (Threads), midiendo el tiempo de ejecución en cada caso.

TECNOLOGIAS USADAS
- Lenguaje: C++
- IDE sugerido: Dev-C++ o cualquier otro compilador C++
- GNUPlot (para graficar tiempos)
- Git y GitHub (control de versiones)

COMO EJECUTAR
Compilación:
Por ejemplo en c++:

```bash
g++ simulacion.cpp -o simulacion -std=c++11

Ejecución:
bash
./simulación

El programa pedirá el valor máximo de N y luego generará matrices aleatorias de tamaños 1x1 hasta NxN. Se ejecutan dos simulaciones:

- Suma secuencial
- Suma con hilos (std::thread)

Los tiempos se guardan en dos archivos:

- resultados_secuencial.txt
- resultados_threads.txt

GRÁFICO EN GNUPLOT: generará grafico_tiempos.png con la comparación
set title "Comparación de tiempos de ejecución"
set xlabel "Tamaño de la matriz (N)"
set ylabel "Tiempo (microsegundos)"
set grid
set terminal pngcairo size 800,600
set output 'grafico_tiempos.png'
plot 'resultados_secuencial.txt' using 1:2 title 'Secuencial' with lines, \
     'resultados_threads.txt' using 1:2 title 'Con Hilos' with lines

ESTRUCTURA DE ARCHIVOS
/LP3-2/
│
├── simulacion.cpp
├── resultados_secuencial.txt
├── resultados_threads.txt
├── grafico_tiempos.png
└── README.md

