PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE places (
  id INTEGER PRIMARY KEY,
  name TEXT,
  description TEXT,
  tour_type TEXT,
  x INTEGER,
  y INTEGER,
  price REAL,
  popular INTEGER
);
INSERT INTO places VALUES(1,'Monasterio de Santa Catalina','Convento colonial muy visitado','Cultural',1,2,25.0,1);
INSERT INTO places VALUES(2,'Can del Colca','Segundo can ms profundo del mundo','Aventura',3,4,90.0,1);
INSERT INTO places VALUES(3,'Mirador de Yanahuara','Vista panormica de los volcanes','Panormico',2,1,0.0,1);
INSERT INTO places VALUES(4,'Molino de Sabanda','Antigua construccin colonial','Histrico',4,3,10.0,0);
INSERT INTO places VALUES(5,'Volcn Misti','Ascenso al volcn emblemtico','Trekking',5,5,120.0,1);
INSERT INTO places VALUES(6,'Ruta del Sillar','Canteras de sillar blanco','Geolgico',3,2,35.0,0);
INSERT INTO places VALUES(7,'Plaza de Armas','Centro histrico de Arequipa','Cultural',0,0,0.0,1);
INSERT INTO places VALUES(8,'Cuevas de Sumbay','Arte rupestre preincaico','Arqueolgico',4,5,20.0,0);
COMMIT;
