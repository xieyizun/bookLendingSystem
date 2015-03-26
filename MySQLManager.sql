SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT;
SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS;
SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION;
SET NAMES latin1;
SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=1;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
	`userID` INT(15) NOT NULL AUTO_INCREMENT,
	`userName` VARCHAR(25) NOT NULL,
	`userPassword` VARCHAR(25) NOT NULL,
	`userEmail` VARCHAR(25) NOT NULL,
	`userPhone` VARCHAR(20) NOT NULL,
	`userAddress` VARCHAR(100) NOT NULL,
	`money` NUMERIC(20,2) NOT NULL,
	PRIMARY KEY (`userID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_german1_ci;

DROP TABLE IF EXISTS `book`;
CREATE TABLE `book` (
	`bookID` INT(15) NOT NULL AUTO_INCREMENT,
	`bookName` VARCHAR(55) NOT NULL,
	`ownerName` VARCHAR(25) NOT NULL,
	`price` NUMERIC(10,2) NOT NULL,
	`state` BOOL NOT NULL default 1,
	`ts` TIMESTAMP NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
	PRIMARY KEY (`bookID`),
	KEY (`bookName`),

	`ownerID` INT(15) NOT NULL,
	KEY (`ownerID`),
	CONSTRAINT `book_ibfk_1` FOREIGN KEY (`ownerID`) REFERENCES `user` (`userID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_german1_ci;

DROP TABLE IF EXISTS `record`;
CREATE TABLE `record` (
	`recordID` INT(15) NOT NULL AUTO_INCREMENT,
	`bookName` VARCHAR(55) NOT NULL,
	`bookID` INT(15) NOT NULL,
	`lenderID` INT(15),
	`borrowerID` INT(15),
	`price` NUMERIC(10,2) NOT NULL,
	`borrowTime` INT(15) NOT NULL,
	`completed` BOOL NOT NULL default 0,
	PRIMARY KEY (`recordID`),
	
	KEY (`lenderID`),
	KEY (`borrowerID`),
	CONSTRAINT `belong_to_book` FOREIGN KEY (`bookID`) REFERENCES `book` (`bookID`) ON DELETE CASCADE,
	CONSTRAINT `belong_to_lender` FOREIGN KEY (`lenderID`) REFERENCES `user` (`userID`) ON DELETE SET NULL,
	CONSTRAINT `belong_to_borrower` FOREIGN KEY (`borrowerID`) REFERENCES `user` (`userID`) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_german1_ci;


LOCK TABLES `user` WRITE;
INSERT INTO `user` VALUES (1, 'root', 'jkl', '1@qq.com', '123456', 'guangzhou', 100.0), (2, 'xieyizun', 'jkl', '2@qq.com', '111111', 'beijing', 100.0);
UNLOCK TABLES;

LOCK TABLES `book` WRITE;
INSERT INTO `book` VALUES (1, 'mysql 5', 'xieyizun', 10.5, 1, '2015-3-01 10:51:10', 2);
UNLOCK TABLES;

LOCK TABLES `record` WRITE;
INSERT INTO `record` VALUES (1,'mysql 5', 1, 2, 1, 10.5, 30, 0);
UNLOCK TABLES;
