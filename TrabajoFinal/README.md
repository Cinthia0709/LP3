# Proyecto: Tours en Arequipa

Este proyecto consiste en una base de datos creada en **MariaDB** y una interfaz gráfica sencilla construida con **Qt**, cuyo propósito es mostrar información de tours turísticos alrededor de la ciudad de Arequipa.

## Descripción

La base de datos contiene una tabla llamada `Places` que almacena información de distintos destinos turísticos, incluyendo su tipo, ubicación (X, Y), precio y si es un destino popular o no.

La interfaz permite visualizar los lugares turísticos registrados en la base de datos, organizados por tipo, popularidad o precio.

## Tecnologías utilizadas

- **MariaDB**: Para la gestión y almacenamiento de datos.
- **Qt (C++/Qt Widgets)**: Para la creación de la interfaz gráfica de usuario.
- **WSL / Ubuntu / Debian**: Ambiente de desarrollo para ejecutar MariaDB y trabajar con Qt.

## Estructura de la tabla

La tabla `Places` tiene la siguiente estructura:

| Campo       | Tipo        | Descripción                                |
|-------------|-------------|--------------------------------------------|
| ID          | INT         | Identificador único del lugar              |
| Nombre      | VARCHAR     | Nombre del lugar turístico                 |
| Descripción | TEXT        | Breve reseña del lugar                     |
| Tipo        | VARCHAR     | Tipo de tour (Cultural, Aventura, etc.)    |
| X           | INT         | Coordenada X (posición en el mapa)         |
| Y           | INT         | Coordenada Y                               |
| Precio      | DECIMAL     | Precio del tour en soles                   |
| Popular     | BOOLEAN     | 1 si es popular, 0 si no                    |

## Ejecución

1. Importar el archivo `places.sql` en MariaDB:

```bash
mysql -u tu_usuario -p tours_arequipa < places.sql


Ejecutar la aplicación Qt desde Qt Creator o compilándola con qmake y make.

Integrantes:
Cinthia Blanco Rodrigo
Karlo Ayala Salazar
Dylan Davila Grau
