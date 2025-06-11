/*M!999999\- enable the sandbox mode */ 
-- MariaDB dump 10.19  Distrib 10.11.11-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: Olimpiadas
-- ------------------------------------------------------
-- Server version	10.11.11-MariaDB-0+deb12u1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `sports_results`
--

DROP TABLE IF EXISTS `sports_results`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `sports_results` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `match_date` varchar(20) DEFAULT NULL,
  `discipline` varchar(30) DEFAULT NULL,
  `team1` varchar(100) DEFAULT NULL,
  `team2` varchar(100) DEFAULT NULL,
  `result` varchar(20) DEFAULT NULL,
  `notes` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sports_results`
--

LOCK TABLES `sports_results` WRITE;
/*!40000 ALTER TABLE `sports_results` DISABLE KEYS */;
INSERT INTO `sports_results` VALUES
(1,'Date 1','Mixed Volleyball','Business Engineering','Communication','2 - 0',''),
(2,'Date 1','Mixed Volleyball','Law','Psychology','2 - 0',''),
(3,'Date 1','Mixed Volleyball','Software Engineering','Architecture','2 - 1',''),
(4,'Date 1','Mixed Volleyball','Business Management','Architecture/Industrial','2 - 0',''),
(5,'Date 1','Men\'s Futsal','Law','Architecture/Industrial','7 - 1',''),
(6,'Date 1','Men\'s Futsal','Business Management','Business Engineering','13 - 1',''),
(7,'Date 1','Men\'s Futsal','Software Engineering','Business Management','3 - 4',''),
(8,'Date 1','Men\'s Futsal','Psychology','Communication','3 - 0',''),
(9,'Date 2','Men\'s Basketball','Business Engineering','Business Management','-','Winner: Business Management'),
(10,'Date 2','Men\'s Basketball','Architecture','Psychology','-','Winner: Architecture'),
(11,'Date 2','Men\'s Basketball','Communication','Software Engineering','-','Winner: Software Engineering'),
(12,'Date 2','Men\'s Basketball','Law','Architecture/Industrial','-','Winner: Law'),
(13,'Date 2','Women\'s Futsal','Business Engineering','Business Management','-','Winner: Business Management'),
(14,'Date 2','Women\'s Futsal','Architecture/Industrial','Psychology','-','Winner: Psychology'),
(15,'Date 2','Women\'s Futsal','Communication','Law','-','Winner: Law'),
(16,'Date 3','Mixed Volleyball','Business Engineering','Software Engineering','0 - 2',''),
(17,'Date 3','Mixed Volleyball','Law','Architecture','0 - 2',''),
(18,'Date 3','Men\'s Futsal','Law','Business Management','-','Winner: Law'),
(19,'Date 3','Men\'s Futsal','Business Management','Psychology','-','Winner: Psychology'),
(20,'Date 4','Men\'s Basketball','Business Management','Software Engineering','25 - 14',''),
(21,'Date 4','Men\'s Basketball','Architecture/Industrial','Law','-','Winner: Law'),
(22,'Date 4','Women\'s Futsal','Business Management','Law','-','Winner: Law'),
(23,'Date 4','Women\'s Futsal','Business Management','Psychology','-','Winner: Psychology'),
(24,'Date 4','Women\'s Futsal','Psychology','Law','-','Winner: Law'),
(25,'Date 5','Mixed Volleyball','Business Management','Software Engineering','2 - 0','Champion: Business Management'),
(26,'Date 5','Women\'s Basketball','Architecture','Business Management','0 -','Champion: Business Management'),
(27,'Date 5','Men\'s Basketball','Law','Business Management','20 - 17','Champion: Law'),
(28,'Date 5','Men\'s Futsal','Psychology','Law','0 - 6','Champion: Law'),
(29,'Date 5','Women\'s Futsal','Psychology','Law','2 - 2','Draw'),
(30,'Finals','Women\'s Futsal','Psychology','Law','5 - 4','Champion: Psychology');
/*!40000 ALTER TABLE `sports_results` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-06-10 21:46:17
