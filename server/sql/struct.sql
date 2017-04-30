-- MySQL dump 10.13  Distrib 5.5.37, for Win32 (x86)
--
-- Host: qq.ssfly.club    Database: pj
-- ------------------------------------------------------
-- Server version	5.7.17-0ubuntu0.16.04.2

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
-- Table structure for table `course`
--

DROP TABLE IF EXISTS `course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `course` (
  `cid` char(6) NOT NULL,
  `cname` varchar(30) DEFAULT NULL,
  `current` int(11) DEFAULT NULL,
  `max` int(11) DEFAULT NULL,
  `tid` char(9) DEFAULT NULL,
  `ctime` char(3) DEFAULT NULL,
  `rid` char(4) DEFAULT NULL,
  `credit` int(11) DEFAULT NULL,
  PRIMARY KEY (`cid`),
  KEY `tid` (`tid`),
  KEY `rid` (`rid`),
  CONSTRAINT `course_ibfk_1` FOREIGN KEY (`tid`) REFERENCES `user` (`id`),
  CONSTRAINT `course_ibfk_2` FOREIGN KEY (`rid`) REFERENCES `room` (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `department`
--

DROP TABLE IF EXISTS `department`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `department` (
  `did` char(4) NOT NULL,
  `dname` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`did`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `room`
--

DROP TABLE IF EXISTS `room`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `room` (
  `rid` char(4) NOT NULL,
  `monday` char(11) DEFAULT '00000000000',
  `tuesday` char(11) DEFAULT '00000000000',
  `wednesday` char(11) DEFAULT '00000000000',
  `thursday` char(11) DEFAULT '00000000000',
  `friday` char(11) DEFAULT '00000000000',
  `saturday` char(11) DEFAULT '00000000000',
  `sunday` char(11) DEFAULT '00000000000',
  PRIMARY KEY (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sc`
--

DROP TABLE IF EXISTS `sc`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sc` (
  `sid` char(9) NOT NULL,
  `cid` char(6) NOT NULL,
  PRIMARY KEY (`sid`,`cid`),
  KEY `cid` (`cid`),
  CONSTRAINT `sc_ibfk_1` FOREIGN KEY (`sid`) REFERENCES `user` (`id`),
  CONSTRAINT `sc_ibfk_2` FOREIGN KEY (`cid`) REFERENCES `course` (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `id` char(9) NOT NULL,
  `name` varchar(30) DEFAULT NULL,
  `grade` int(11) DEFAULT NULL,
  `did` char(4) DEFAULT NULL,
  `pwd` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `did` (`did`),
  CONSTRAINT `user_ibfk_1` FOREIGN KEY (`did`) REFERENCES `department` (`did`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- ----------------------------
-- Records of room
-- ----------------------------
INSERT INTO `room` VALUES ('2001', '01111000000', '00000000000', '01111000000', '00000000000', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2002', '00011000000', '00000000000', '00000000000', '00000000000', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2003', '00000000000', '00000000000', '00001100000', '00000000000', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2004', '01111000000', '00000000000', '00000110000', '00000000000', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2101', '00000000000', '00000000000', '00000000000', '01100110000', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2102', '00000000000', '00000000000', '00000000011', '00000001100', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2103', '00000000000', '00000000000', '00000001100', '00000000000', '00000000000', '00000000000', '00000000000');
INSERT INTO `room` VALUES ('2104', '01100000000', '00000000000', '00000000011', '00000000000', '00000000000', '00000000000', '00000000000');
-- ----------------------------
-- Records of department
-- ----------------------------
INSERT INTO `department` VALUES ('0001', '计算机');
INSERT INTO `department` VALUES ('0002', '数学系');
INSERT INTO `department` VALUES ('0003', '软件工程');
INSERT INTO `department` VALUES ('0004', '微电子');
INSERT INTO `department` VALUES ('0005', '药学院');
INSERT INTO `department` VALUES ('0006', '中文系');
INSERT INTO `department` VALUES ('0007', '化学系');