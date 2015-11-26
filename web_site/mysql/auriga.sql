-- MySQL dump 10.13  Distrib 5.1.73, for redhat-linux-gnu (x86_64)
--
-- Host: 127.12.95.2    Database: auriga
-- ------------------------------------------------------
-- Server version	5.5.45

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `errors`
--

DROP TABLE IF EXISTS `errors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `errors` (
  `id` int(10) unsigned NOT NULL,
  `uuid_perft` char(36) COLLATE utf8_bin NOT NULL,
  `uuid_task` char(36) COLLATE utf8_bin NOT NULL,
  `fen` char(100) COLLATE utf8_bin NOT NULL,
  `depth` int(11) DEFAULT NULL,
  `creation_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `heartbeat` tinyint(1) DEFAULT NULL,
  `tot` bigint(20) unsigned DEFAULT NULL,
  `engine` varchar(30) COLLATE utf8_bin DEFAULT NULL,
  `author` varchar(30) COLLATE utf8_bin DEFAULT NULL,
  `minutes` int(11) DEFAULT NULL,
  `country` char(2) COLLATE utf8_bin DEFAULT NULL,
  `anomaly` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ip_country_timezone`
--

DROP TABLE IF EXISTS `ip_country_timezone`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ip_country_timezone` (
  `network` varchar(11) COLLATE utf8_bin NOT NULL,
  `country_iso_code` char(2) COLLATE utf8_bin NOT NULL,
  `time_zone` tinyint(4) NOT NULL,
  PRIMARY KEY (`network`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `perft`
--

DROP TABLE IF EXISTS `perft`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `perft` (
  `uuid_perft` char(36) COLLATE utf8_bin NOT NULL,
  `fen` varchar(100) COLLATE utf8_bin NOT NULL,
  `depth` int(11) NOT NULL,
  `tasks` int(11) NOT NULL,
  `creation_date` timestamp NULL DEFAULT NULL,
  `tot` varchar(40) COLLATE utf8_bin DEFAULT NULL,
  `perc_completed` tinyint(4) DEFAULT NULL,
  `minutes` int(11) DEFAULT NULL,
  `engines` int(11) DEFAULT NULL,
  `confirmed` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`uuid_perft`),
  UNIQUE KEY `fen` (`fen`,`depth`,`tasks`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `perft_engines`
--

DROP TABLE IF EXISTS `perft_engines`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `perft_engines` (
  `uuid_perft` char(36) COLLATE utf8_bin NOT NULL,
  `engine` varchar(30) COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`uuid_perft`,`engine`),
  CONSTRAINT `tadids_ibfk_1` FOREIGN KEY (`uuid_perft`) REFERENCES `perft` (`uuid_perft`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `perft_tasks`
--

DROP TABLE IF EXISTS `perft_tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `perft_tasks` (
  `uuid_perft` char(36) COLLATE utf8_bin NOT NULL,
  `uuid_task` char(36) COLLATE utf8_bin NOT NULL,
  `fens` int(11) DEFAULT NULL,
  `engine` int(11) DEFAULT NULL,
  `perc_completed` tinyint(4) DEFAULT NULL,
  `creation_date` timestamp NULL DEFAULT NULL,
  `tot` bigint(20) unsigned DEFAULT NULL,
  `minutes` int(11) DEFAULT NULL,
  PRIMARY KEY (`uuid_perft`,`uuid_task`),
  UNIQUE KEY `uuid_task` (`uuid_task`),
  CONSTRAINT `ptiaisks_i0f3_2` FOREIGN KEY (`uuid_perft`) REFERENCES `perft` (`uuid_perft`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `personal_uuid`
--

DROP TABLE IF EXISTS `personal_uuid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `personal_uuid` (
  `uuid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `task_fens`
--

DROP TABLE IF EXISTS `task_fens`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_fens` (
  `uuid_task` char(36) COLLATE utf8_bin NOT NULL,
  `fen` varchar(100) COLLATE utf8_bin NOT NULL,
  `depth` int(11) DEFAULT NULL,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `task_fs_fen_IDX` (`fen`),
  KEY `ptasksds_bf3_2` (`uuid_task`),
  CONSTRAINT `iptaxdds_bf3_2` FOREIGN KEY (`uuid_task`) REFERENCES `perft_tasks` (`uuid_task`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=428964 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tasks`
--

DROP TABLE IF EXISTS `tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tasks` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uuid_perft` char(36) COLLATE utf8_bin NOT NULL,
  `uuid_task` char(36) COLLATE utf8_bin NOT NULL,
  `fen` char(100) COLLATE utf8_bin NOT NULL,
  `depth` int(11) DEFAULT NULL,
  `creation_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `heartbeat` tinyint(1) DEFAULT NULL,
  `tot` bigint(20) unsigned DEFAULT NULL,
  `engine` varchar(30) COLLATE utf8_bin DEFAULT NULL,
  `author` varchar(30) COLLATE utf8_bin DEFAULT NULL,
  `minutes` int(11) DEFAULT NULL,
  `country` char(2) COLLATE utf8_bin DEFAULT NULL,
  `anomaly` int(11) DEFAULT NULL,
  `personal_uuid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `uuid_task` (`uuid_task`),
  KEY `tadds_ibifk_1` (`uuid_perft`),
  KEY `tadfk_1` (`personal_uuid`),
  CONSTRAINT `taddds_ibfk_1` FOREIGN KEY (`uuid_perft`) REFERENCES `perft` (`uuid_perft`) ON DELETE CASCADE,
  CONSTRAINT `tadfk_1` FOREIGN KEY (`personal_uuid`) REFERENCES `personal_uuid` (`uuid`)
) ENGINE=InnoDB AUTO_INCREMENT=10941 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-11-26 13:15:54
