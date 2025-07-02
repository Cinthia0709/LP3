#  Aplicación de Rutas Turísticas

Este proyecto es una aplicación de escritorio desarrollada para gestionar rutas entre lugares turísticos mediante grafos. Permite la creación de nodos (lugares), la creación de rutas entre ellos, y la visualización de caminos mediante algoritmos como **camino mínimo**, **búsqueda en profundidad (DFS)** y **búsqueda en anchura (BFS)**.

##  Funcionalidades

- **Crear Nodo:** Añade un nuevo punto turístico al grafo.
- **Modificar Nodo:** Edita el nombre o posición de un nodo existente.
- **Eliminar Nodo:** Elimina un nodo y sus conexiones.
- **Crear Ruta:** Conecta dos nodos con una ruta, indicando su peso/distancia.
- **Visualizar Caminos:**
  - **Camino mínimo:** Utiliza algoritmos como Dijkstra para mostrar la ruta más corta.
  - **Recorrido en Profundidad (DFS):** Explora el grafo en profundidad desde un nodo origen.
  - **Recorrido en Anchura (BFS):** Explora el grafo por niveles desde un nodo origen.

##  Capturas de Pantalla

| Vista | Descripción |
|------|-------------|
| ![Vista Principal](./7452e84f-dfe8-42a2-86b3-3051dfc5d715.jpg) | Visualización general del grafo con nodos y rutas. |
| ![Vista Lateral](./d7dbd9ae-813f-4e1a-933a-a564a2215f87.jpg) | Menú lateral con botones para crear y modificar nodos. |
| ![Botones](./7aba5c77-218c-4687-b5bc-72e2ca03714b.jpg) | Detalle de botones disponibles para la gestión de nodos. |

Los nodos representan lugares como:
- `SantuarioDeSantaC`
- `PlazaDeArmas`
- `PlazaDeYanahuara`

##  Integrantes del Proyecto y Aportes

| Integrante                             | Aporte principal                                                      | Porcentaje de trabajo |
|----------------------------------------|------------------------------------------------------------------------|------------------------|
| **Cinthia América Blanco Rodrigo**     | Lógica de algoritmos de caminos, estructura del grafo, interfaz.      | 33%                    |
| **Karlo Ayala Eduardo Salazar**        | Diseño de interfaz, lógica de creación y modificación de nodos.       | 33%                    |
| **Dylan Edward Davila Grau**           | Implementación de algoritmos DFS, BFS y pruebas de funcionamiento.    | 34%                    |


##  Tecnologías Utilizadas

- C++ 
- Algoritmos de grafos: Dijkstra, DFS, BFS
- Qt

##  Estado del Proyecto

 Módulos de creación y modificación de nodos funcionando  
 Visualización básica de rutas  
 Integración completa del camino mínimo y mejoras gráficas

## 📝 Notas Adicionales

- Esta aplicación fue desarrollada como proyecto académico.
- Las rutas representan conexiones entre puntos turísticos de Arequipa.



