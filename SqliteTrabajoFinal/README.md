# Proyecto: Base de Datos de Tours en Arequipa

Este proyecto consiste en una base de datos construida en **SQLite3** que almacena información sobre destinos turísticos alrededor de Arequipa. El objetivo es representar diversos lugares, su tipo de tour, ubicación y si son populares o no.

## Archivo principal

- `tours_arequipa.db`: Base de datos en formato SQLite.
- `backup_tours.sql`: Respaldo completo de la base de datos en formato `.sql`.

## Tabla: places

La base de datos contiene una única tabla llamada `places`, con los siguientes campos:

| Campo       | Tipo     | Descripción                               |
|-------------|----------|-------------------------------------------|
| id          | INTEGER  | ID único del lugar                        |
| name        | TEXT     | Nombre del lugar turístico                |
| description | TEXT     | Breve descripción del lugar               |
| tour_type   | TEXT     | Tipo de tour (Cultural, Aventura, etc.)   |
| x           | INTEGER  | Coordenada X (para representación visual) |
| y           | INTEGER  | Coordenada Y                              |
| price       | REAL     | Precio en soles (S/.)                     |
| popular     | INTEGER  | 1 si es popular, 0 si no                  |

## 🛠️ Cómo abrir la base de datos

Puedes abrir el archivo `.db` desde tu terminal con:

```bash
sqlite3 tours_arequipa.db


Y luego puedes usar consultas como:

sql
Copiar
Editar
SELECT * FROM places;

INTEGRANTES
Cinthia Blanco
Karlo Ayala Salazar
Dylan Davylá Grau