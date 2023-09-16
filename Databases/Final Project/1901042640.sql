-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 24, 2023 at 03:08 PM
-- Server version: 10.4.28-MariaDB
-- PHP Version: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `1901042640`
--

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `add_money_to_wallet` (IN `userid` INT, IN `moneyamount` INT)   BEGIN
UPDATE wallets SET wallets.wallet_money_amount = wallets.wallet_money_amount + moneyamount WHERE wallets.wallet_customer_id = userid;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_comment` (IN `userid` INT, IN `gameid` INT, IN `content` VARCHAR(30), OUT `result` INT)   BEGIN
  DECLARE number_of_games INT DEFAULT 0;
  SET autocommit = 0;

  START TRANSACTION;
  INSERT INTO comments (comments.comment_game_id, comments.comment_customer_id, comments.comment_content) VALUES(gameid, userid, content);
  SET number_of_games = (SELECT COUNT(*) FROM comments WHERE comments.comment_game_id = gameid AND comments.comment_customer_id = userid);
  
  IF (number_of_games > 1) THEN 
  ROLLBACK;
  SET result = 0;
  ELSE
    SET result = 1;
    COMMIT;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_game` (IN `userid` INT, IN `gamename` VARCHAR(30), IN `baseprice` INT, IN `discount` INT, IN `releaseyear` INT, IN `rating` INT, IN `categoryid` INT, OUT `result` INT)   BEGIN
  DECLARE number_of_games INT DEFAULT 0;
  SET autocommit = 0;

  START TRANSACTION;
  INSERT INTO games (games.game_name, games.game_base_price, games.game_discount_percentage, games.game_release_year, games.game_publisher_id, games.game_rating) VALUES(gamename, baseprice, discount, releaseyear, userid, rating);
  INSERT INTO category_and_games (category_and_games.category_id, category_and_games.game_id) VALUES(categoryid, last_insert_id());
  SET number_of_games = (SELECT COUNT(*) FROM games where games.game_name = gamename OR games.game_rating = rating);  
  IF (number_of_games > 1) THEN 
  ROLLBACK;
  SET result = 0;
  ELSE
    SET result = (SELECT games.game_id FROM games WHERE games.game_name = gamename);
  COMMIT;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_order` (IN `userid` INT, IN `gameid` INT, IN `address` VARCHAR(30), IN `gameprice` INT, OUT `result` INT)   BEGIN
  DECLARE number_of_games INT DEFAULT 0;
  DECLARE billid INT DEFAULT 0;
  DECLARE wallet_money_amount INT DEFAULT 0;
  SET autocommit = 0;

  START TRANSACTION;
  INSERT INTO orders (order_customer_id, order_game_id, order_address, order_date) VALUES(userid, gameid, address, CURDATE());
  SET number_of_games = (SELECT COUNT(*) FROM libraryitems WHERE libraryitems.game_id = gameid AND libraryitems.customer_id = userid);
  SET wallet_money_amount = (SELECT wallets.wallet_money_amount from wallets where wallets.wallet_customer_id = userid);
  
  IF (number_of_games > 1 OR wallet_money_amount - gameprice < 0) THEN 
  ROLLBACK;
  SET result = 0;
  ELSE
  	UPDATE wallets SET wallets.wallet_money_amount = wallet_money_amount - gameprice WHERE wallets.wallet_customer_id = userid;
    SET result = (SELECT orders.order_id FROM orders WHERE orders.order_customer_id = userid AND orders.order_game_id = gameid);
  COMMIT;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `delete_customer` (IN `userid` INT)   BEGIN
DELETE FROM bills WHERE bill_order_id in (SELECT DISTINCT order_id FROM orders where orders.order_customer_id = userid);
DELETE FROM notifications WHERE notifications.notification_order_id in (SELECT DISTINCT order_id FROM orders WHERE orders.order_customer_id = userid);
DELETE FROM orders WHERE orders.order_customer_id = userid;
DELETE FROM libraryitems WHERE libraryitems.customer_id = userid;
DELETE FROM customers WHERE customers.customer_id = userid;
DELETE FROM wallets WHERE wallets.wallet_customer_id = userid;
DELETE FROM comments WHERE comments.comment_customer_id = userid;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `delete_game_customer` (IN `userid` INT, IN `gameid` INT)   BEGIN
DELETE FROM libraryitems WHERE libraryitems.game_id = gameid AND libraryitems.customer_id = userid;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `filter_games_by_category` (IN `userid` INT, IN `categoryid` INT)   BEGIN
SELECT games.game_name, publishers.publisher_name, categories.category_name, games.game_release_year, games.game_rating, games.game_id from games join publishers on publishers.publisher_id = games.game_publisher_id 
JOIN category_and_games on category_and_games.game_id = games.game_id
JOIN categories on categories.category_id = categoryid AND category_and_games.category_id = categories.category_id
JOIN libraryitems on libraryitems.customer_id = userid AND libraryitems.game_id = games.game_id;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `get_wallet_money` (IN `userid` INT, OUT `result` INT)   BEGIN


SET result = (SELECT wallets.wallet_money_amount FROM wallets WHERE wallets.wallet_customer_id = userid);
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `register_customer` (IN `username` VARCHAR(39), IN `userpassword` VARCHAR(30), IN `useremail` VARCHAR(30), IN `userphone` INT, OUT `result` INT)   BEGIN
  DECLARE number_of_rows INT DEFAULT 0;
  DECLARE walletid INT DEFAULT 0;
  SET autocommit = 0;

  START TRANSACTION;
  INSERT INTO customers (customer_name, customer_password, customer_email, customer_phone) VALUES(username, userpassword, useremail, userphone);
  SET number_of_rows = (SELECT COUNT(*) FROM customers WHERE customers.customer_email = useremail);
  IF (number_of_rows > 1) THEN
  ROLLBACK;
  SET result = 0;
  ELSE
  SET result = (SELECT customers.customer_id FROM customers WHERE customers.customer_email = useremail);
  COMMIT;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `register_publisher` (IN `username` VARCHAR(30), IN `userpassword` VARCHAR(30), IN `useremail` VARCHAR(30), IN `userphone` INT, IN `userlocation` VARCHAR(30), OUT `result` INT)   BEGIN
  DECLARE number_of_rows INT DEFAULT 0;
  SET autocommit = 0;

  START TRANSACTION;
  INSERT INTO publishers (publisher_name, publisher_password, publisher_email, publisher_phone, publisher_location) VALUES(username, userpassword, useremail, userphone, userlocation);
  SET number_of_rows = (SELECT COUNT(*) FROM publishers WHERE publishers.publisher_email = useremail);
  IF (number_of_rows > 1) THEN
  ROLLBACK;
  SET result = 0;
  ELSE
  SET result = (SELECT publishers.publisher_id FROM publishers WHERE publishers.publisher_email = useremail);
  COMMIT;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_game` (IN `userid` INT, IN `gameid` INT, IN `baseprice` INT, IN `discount` INT, OUT `result` INT)   BEGIN
  DECLARE number_of_games INT DEFAULT 0;
  SET autocommit = 0;

  START TRANSACTION;
  UPDATE games SET games.game_base_price = baseprice, games.game_discount_percentage = discount WHERE games.game_id = gameid;
  SET number_of_games = (SELECT COUNT(*) FROM games where games.game_id = gameid AND games.game_publisher_id = userid);  
  IF (number_of_games <= 0 OR baseprice <= 0 OR discount < 0) THEN
  ROLLBACK;
  SET result = 0;
  ELSE
    SET result = (SELECT games.game_id FROM games WHERE games.game_id = gameid);
  COMMIT;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_comments` (IN `gameid` INT)   BEGIN
select customers.customer_name, comments.comment_content from comments
right join customers ON customers.customer_id = comments.comment_customer_id
join games on games.game_id = gameid AND comments.comment_game_id = games.game_id;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_library` (IN `userid` INT)   BEGIN
select games.game_name, publishers.publisher_name, categories.category_name, games.game_release_year, games.game_rating, games.game_id from games join libraryitems on libraryitems.game_id = games.game_id and libraryitems.customer_id = userid
join publishers on publishers.publisher_id = games.game_publisher_id
join category_and_games on category_and_games.game_id = games.game_id
join categories on categories.category_id = category_and_games.category_id;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_notifications` (IN `userid` INT)   BEGIN
SELECT notifications.notification_content, games.game_name from notifications
join orders ON orders.order_customer_id = userid AND notifications.notification_order_id = orders.order_id
join games on games.game_id = orders.order_game_id;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_orders` (IN `userid` INT)   BEGIN
select orders.order_id, customers.customer_name, games.game_name, games.game_net_price, orders.order_address, orders.order_date, bills.bill_id, bills.bill_address, bills.bill_due_date from orders 
join bills on orders.order_id = bills.bill_order_id and orders.order_customer_id = userid
join customers on customers.customer_id = orders.order_customer_id
join games on games.game_id = orders.order_game_id;
END$$

--
-- Functions
--
CREATE DEFINER=`root`@`localhost` FUNCTION `variable` () RETURNS INT(11) DETERMINISTIC NO SQL return @variable$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `bills`
--

CREATE TABLE `bills` (
  `bill_id` int(11) NOT NULL,
  `bill_address` varchar(30) NOT NULL,
  `bill_due_date` date NOT NULL,
  `bill_order_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `bills`
--

INSERT INTO `bills` (`bill_id`, `bill_address`, `bill_due_date`, `bill_order_id`) VALUES
(23, 'hfghgfh', '2025-06-11', 30),
(26, 'Goztepe', '2025-06-11', 33),
(27, 'Istanbul', '2025-06-11', 34),
(28, 'Ordu', '2025-06-12', 35),
(29, 'Ordu', '2025-06-12', 36),
(39, 'Kocaeli', '2025-06-15', 46),
(40, 'Lima', '2025-06-15', 47),
(48, 'Goztepe', '2025-06-24', 55),
(49, 'Goztepe', '2025-06-24', 56),
(50, 'Goztepe', '2025-06-24', 57),
(51, 'Bostanci', '2025-06-24', 58),
(53, 'Bostanci', '2025-06-24', 60),
(54, 'Bostanci', '2025-06-24', 61);

-- --------------------------------------------------------

--
-- Table structure for table `categories`
--

CREATE TABLE `categories` (
  `category_id` int(11) NOT NULL,
  `category_rating` int(11) NOT NULL,
  `category_name` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `categories`
--

INSERT INTO `categories` (`category_id`, `category_rating`, `category_name`) VALUES
(1, 2, 'FPS'),
(2, 1, 'Shooter'),
(3, 5, 'Adventure'),
(4, 4, 'RPG');

-- --------------------------------------------------------

--
-- Table structure for table `category_and_games`
--

CREATE TABLE `category_and_games` (
  `game_id` int(11) NOT NULL,
  `category_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `category_and_games`
--

INSERT INTO `category_and_games` (`game_id`, `category_id`) VALUES
(1, 3),
(1, 1),
(2, 3),
(3, 1),
(3, 2),
(4, 4),
(4, 3),
(5, 4),
(6, 2),
(12, 3),
(14, 91),
(15, 4),
(16, 2);

-- --------------------------------------------------------

--
-- Table structure for table `comments`
--

CREATE TABLE `comments` (
  `comment_game_id` int(11) NOT NULL,
  `comment_customer_id` int(11) NOT NULL,
  `comment_content` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `comments`
--

INSERT INTO `comments` (`comment_game_id`, `comment_customer_id`, `comment_content`) VALUES
(3, 1, 'Great game'),
(1, 1, 'Baya iyii'),
(4, 1, 'Almayan pisman .d'),
(2, 1, 'guzell'),
(5, 1, 'very nice'),
(6, 1, 'dfghdh'),
(6, 64, 'Guzel'),
(3, 64, 'Cok guzel'),
(16, 64, 'Cok guzel'),
(16, 66, 'Muazzam'),
(3, 66, 'Mehh'),
(5, 66, 'Kesin alinn');

-- --------------------------------------------------------

--
-- Table structure for table `customers`
--

CREATE TABLE `customers` (
  `customer_id` int(11) NOT NULL,
  `customer_name` varchar(30) NOT NULL,
  `customer_password` varchar(30) NOT NULL,
  `customer_email` varchar(30) NOT NULL,
  `customer_phone` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `customers`
--

INSERT INTO `customers` (`customer_id`, `customer_name`, `customer_password`, `customer_email`, `customer_phone`) VALUES
(1, 'Emre Sezer', '12345', 'emresezer', 123123),
(64, 'Bilal Miller', '42069', 'bilalmiller', 34534),
(65, 'Beyza Sezer', '1234', 'beyzasezer', 45345),
(66, 'Efe Okur', '12345', 'efeokur', 456456);

--
-- Triggers `customers`
--
DELIMITER $$
CREATE TRIGGER `create_wallet` AFTER INSERT ON `customers` FOR EACH ROW BEGIN
  INSERT INTO wallets (wallet_money_amount, wallet_customer_id) VALUES (100, NEW.customer_id);
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `games`
--

CREATE TABLE `games` (
  `game_id` int(11) NOT NULL,
  `game_name` varchar(30) NOT NULL,
  `game_base_price` int(11) NOT NULL,
  `game_discount_percentage` int(11) NOT NULL,
  `game_net_price` int(11) NOT NULL,
  `game_release_year` int(11) NOT NULL,
  `game_publisher_id` int(11) NOT NULL,
  `game_rating` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `games`
--

INSERT INTO `games` (`game_id`, `game_name`, `game_base_price`, `game_discount_percentage`, `game_net_price`, `game_release_year`, `game_publisher_id`, `game_rating`) VALUES
(1, 'DOOM', 50, 0, 50, 1991, 3, 14),
(2, 'Life is Strange', 120, 10, 108, 2013, 2, 7),
(3, 'Call of Duty', 200, 25, 150, 2001, 4, 35),
(4, 'Mount and Blade', 150, 50, 75, 2005, 1, 12),
(5, 'Valheim', 100, 10, 90, 2020, 1, 8),
(6, 'Beyond Two Souls', 125, 0, 125, 2009, 3, 20),
(12, 'Tomb Raider', 300, 10, 270, 2023, 6, 76),
(14, 'Word Universe', 300, 10, 270, 100, 7, 2022),
(15, 'Joy Blast', 200, 20, 160, 2018, 7, 101),
(16, 'Fun Run', 200, 50, 100, 2023, 8, 10);

--
-- Triggers `games`
--
DELIMITER $$
CREATE TRIGGER `update_gamme_net_price` BEFORE INSERT ON `games` FOR EACH ROW BEGIN
SET NEW.game_net_price = ((100 - NEW.game_discount_percentage) / 100 * NEW.game_base_price);
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `update_price_update` BEFORE UPDATE ON `games` FOR EACH ROW BEGIN
SET NEW.game_net_price = ((100 - NEW.game_discount_percentage) / 100 * NEW.game_base_price);
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `libraryitems`
--

CREATE TABLE `libraryitems` (
  `game_id` int(11) NOT NULL,
  `customer_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `libraryitems`
--

INSERT INTO `libraryitems` (`game_id`, `customer_id`) VALUES
(1, 1),
(2, 1),
(5, 1),
(3, 1),
(4, 1),
(6, 1),
(12, 1),
(6, 64),
(3, 64),
(16, 64),
(16, 66),
(3, 66),
(5, 66);

-- --------------------------------------------------------

--
-- Table structure for table `notifications`
--

CREATE TABLE `notifications` (
  `notification_order_id` int(11) NOT NULL,
  `notification_content` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `notifications`
--

INSERT INTO `notifications` (`notification_order_id`, `notification_content`) VALUES
(46, 'Have fun!'),
(47, 'Lets play!'),
(30, 'Thank you for your order!'),
(33, 'Thank you for your order!'),
(34, 'Thank you for your order!'),
(35, 'Thank you for your order!'),
(36, 'Thank you for your order!'),
(55, 'Have a great time!'),
(56, 'Lets play!'),
(57, 'Have fun!'),
(58, 'Have a great time!'),
(60, 'Have fun!'),
(61, 'Have a great time!');

-- --------------------------------------------------------

--
-- Table structure for table `notification_messages`
--

CREATE TABLE `notification_messages` (
  `notification_message_id` int(11) NOT NULL,
  `notification_message_content` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `notification_messages`
--

INSERT INTO `notification_messages` (`notification_message_id`, `notification_message_content`) VALUES
(1, 'Have fun!'),
(2, 'Have a great time!'),
(3, 'Lets play!'),
(4, 'Thank you for your order!');

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE `orders` (
  `order_id` int(11) NOT NULL,
  `order_customer_id` int(11) NOT NULL,
  `order_game_id` int(11) NOT NULL,
  `order_address` varchar(30) NOT NULL,
  `order_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `orders`
--

INSERT INTO `orders` (`order_id`, `order_customer_id`, `order_game_id`, `order_address`, `order_date`) VALUES
(30, 1, 1, 'hfghgfh', '2023-06-11'),
(33, 1, 2, 'Goztepe', '2023-06-11'),
(34, 1, 5, 'Istanbul', '2023-06-11'),
(35, 1, 3, 'Ordu', '2023-06-12'),
(36, 1, 4, 'Ordu', '2023-06-12'),
(46, 1, 6, 'Kocaeli', '2023-06-15'),
(47, 1, 12, 'Lima', '2023-06-15'),
(55, 64, 6, 'Goztepe', '2023-06-24'),
(56, 64, 3, 'Goztepe', '2023-06-24'),
(57, 64, 16, 'Goztepe', '2023-06-24'),
(58, 66, 16, 'Bostanci', '2023-06-24'),
(60, 66, 3, 'Bostanci', '2023-06-24'),
(61, 66, 5, 'Bostanci', '2023-06-24');

--
-- Triggers `orders`
--
DELIMITER $$
CREATE TRIGGER `create_bill` AFTER INSERT ON `orders` FOR EACH ROW BEGIN
  INSERT INTO bills (bill_address, bill_due_date, bill_order_id) VALUES (NEW.order_address, DATE_ADD(NEW.order_date, INTERVAL 2 YEAR), NEW.order_id);
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `create_library_item` AFTER INSERT ON `orders` FOR EACH ROW BEGIN
  INSERT INTO libraryitems (game_id, customer_id) VALUES (NEW.order_game_id, NEW.order_customer_id);
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `create_notification` AFTER INSERT ON `orders` FOR EACH ROW BEGIN
  DECLARE messageid INT DEFAULT 0;
  DECLARE messagecontent VARCHAR(30);
  SET messageid = ((NEW.order_id) % 3) + 1;
  SET messagecontent = (SELECT notification_messages.notification_message_content FROM notification_messages WHERE notification_messages.notification_message_id = messageid);
  INSERT INTO notifications (notifications.notification_order_id, notifications.notification_content) VALUES (NEW.order_id, messagecontent);
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `publishers`
--

CREATE TABLE `publishers` (
  `publisher_id` int(11) NOT NULL,
  `publisher_name` varchar(30) NOT NULL,
  `publisher_password` varchar(30) NOT NULL,
  `publisher_phone` int(11) NOT NULL,
  `publisher_email` varchar(30) NOT NULL,
  `publisher_location` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `publishers`
--

INSERT INTO `publishers` (`publisher_id`, `publisher_name`, `publisher_password`, `publisher_phone`, `publisher_email`, `publisher_location`) VALUES
(1, 'Bethesda', '12345', 1234567, 'bethesda@gmail.com', 'San Francisco'),
(2, 'EA', '66666', 5353535, 'ea@gmail.com', 'New York'),
(3, 'Ubisoft', '097897', 56565656, 'ubisoft@gmail.com', 'Paris'),
(4, 'Valve', '123456', 420420, 'valve@gmail.com', 'Phoenix'),
(6, 'NYPE', '31331', 45346, 'nype@gmail.com', 'Istanbul'),
(7, 'Libra', '1234', 5645, 'libra', 'Goztepe'),
(8, 'Ahmet Game Company', '123', 434343, 'ahmet', 'Goztepe');

-- --------------------------------------------------------

--
-- Stand-in structure for view `sort_games_by_name`
-- (See below for the actual view)
--
CREATE TABLE `sort_games_by_name` (
`game_id` int(11)
,`game_name` varchar(30)
,`game_base_price` int(11)
,`game_discount_percentage` int(11)
,`game_net_price` int(11)
,`game_release_year` int(11)
,`publisher_name` varchar(30)
,`game_rating` int(11)
,`category_name` varchar(30)
);

-- --------------------------------------------------------

--
-- Stand-in structure for view `sort_games_by_price`
-- (See below for the actual view)
--
CREATE TABLE `sort_games_by_price` (
`game_id` int(11)
,`game_name` varchar(30)
,`game_base_price` int(11)
,`game_discount_percentage` int(11)
,`game_net_price` int(11)
,`game_release_year` int(11)
,`publisher_name` varchar(30)
,`game_rating` int(11)
,`category_name` varchar(30)
);

-- --------------------------------------------------------

--
-- Stand-in structure for view `sort_games_by_rating`
-- (See below for the actual view)
--
CREATE TABLE `sort_games_by_rating` (
`game_id` int(11)
,`game_name` varchar(30)
,`game_base_price` int(11)
,`game_discount_percentage` int(11)
,`game_net_price` int(11)
,`game_release_year` int(11)
,`publisher_name` varchar(30)
,`game_rating` int(11)
,`category_name` varchar(30)
);

-- --------------------------------------------------------

--
-- Stand-in structure for view `view_all_categories`
-- (See below for the actual view)
--
CREATE TABLE `view_all_categories` (
`category_id` int(11)
,`category_rating` int(11)
,`category_name` varchar(30)
);

-- --------------------------------------------------------

--
-- Stand-in structure for view `view_all_games`
-- (See below for the actual view)
--
CREATE TABLE `view_all_games` (
`game_id` int(11)
,`game_name` varchar(30)
,`game_base_price` int(11)
,`game_discount_percentage` int(11)
,`game_net_price` int(11)
,`game_release_year` int(11)
,`publisher_name` varchar(30)
,`game_rating` int(11)
,`category_name` varchar(30)
);

-- --------------------------------------------------------

--
-- Stand-in structure for view `view_wallet`
-- (See below for the actual view)
--
CREATE TABLE `view_wallet` (
`game_name` varchar(30)
,`publisher_name` varchar(30)
,`category_name` varchar(30)
,`game_release_year` int(11)
,`game_rating` int(11)
);

-- --------------------------------------------------------

--
-- Table structure for table `wallets`
--

CREATE TABLE `wallets` (
  `wallet_money_amount` int(11) NOT NULL DEFAULT 0,
  `wallet_customer_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `wallets`
--

INSERT INTO `wallets` (`wallet_money_amount`, `wallet_customer_id`) VALUES
(947, 1),
(845, 64),
(100, 65),
(460, 66);

-- --------------------------------------------------------

--
-- Structure for view `sort_games_by_name`
--
DROP TABLE IF EXISTS `sort_games_by_name`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `sort_games_by_name`  AS SELECT `games`.`game_id` AS `game_id`, `games`.`game_name` AS `game_name`, `games`.`game_base_price` AS `game_base_price`, `games`.`game_discount_percentage` AS `game_discount_percentage`, `games`.`game_net_price` AS `game_net_price`, `games`.`game_release_year` AS `game_release_year`, `publishers`.`publisher_name` AS `publisher_name`, `games`.`game_rating` AS `game_rating`, `categories`.`category_name` AS `category_name` FROM (((`games` left join `publishers` on(`games`.`game_publisher_id` = `publishers`.`publisher_id`)) join `category_and_games` on(`category_and_games`.`game_id` = `games`.`game_id`)) join `categories` on(`categories`.`category_id` = `category_and_games`.`category_id`)) ORDER BY `games`.`game_name` ASC ;

-- --------------------------------------------------------

--
-- Structure for view `sort_games_by_price`
--
DROP TABLE IF EXISTS `sort_games_by_price`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `sort_games_by_price`  AS SELECT `games`.`game_id` AS `game_id`, `games`.`game_name` AS `game_name`, `games`.`game_base_price` AS `game_base_price`, `games`.`game_discount_percentage` AS `game_discount_percentage`, `games`.`game_net_price` AS `game_net_price`, `games`.`game_release_year` AS `game_release_year`, `publishers`.`publisher_name` AS `publisher_name`, `games`.`game_rating` AS `game_rating`, `categories`.`category_name` AS `category_name` FROM (((`games` left join `publishers` on(`games`.`game_publisher_id` = `publishers`.`publisher_id`)) join `category_and_games` on(`category_and_games`.`game_id` = `games`.`game_id`)) join `categories` on(`categories`.`category_id` = `category_and_games`.`category_id`)) ORDER BY `games`.`game_net_price` ASC ;

-- --------------------------------------------------------

--
-- Structure for view `sort_games_by_rating`
--
DROP TABLE IF EXISTS `sort_games_by_rating`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `sort_games_by_rating`  AS SELECT `games`.`game_id` AS `game_id`, `games`.`game_name` AS `game_name`, `games`.`game_base_price` AS `game_base_price`, `games`.`game_discount_percentage` AS `game_discount_percentage`, `games`.`game_net_price` AS `game_net_price`, `games`.`game_release_year` AS `game_release_year`, `publishers`.`publisher_name` AS `publisher_name`, `games`.`game_rating` AS `game_rating`, `categories`.`category_name` AS `category_name` FROM (((`games` left join `publishers` on(`games`.`game_publisher_id` = `publishers`.`publisher_id`)) join `category_and_games` on(`category_and_games`.`game_id` = `games`.`game_id`)) join `categories` on(`categories`.`category_id` = `category_and_games`.`category_id`)) ORDER BY `games`.`game_rating` ASC ;

-- --------------------------------------------------------

--
-- Structure for view `view_all_categories`
--
DROP TABLE IF EXISTS `view_all_categories`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `view_all_categories`  AS SELECT `categories`.`category_id` AS `category_id`, `categories`.`category_rating` AS `category_rating`, `categories`.`category_name` AS `category_name` FROM `categories` WHERE 1 ;

-- --------------------------------------------------------

--
-- Structure for view `view_all_games`
--
DROP TABLE IF EXISTS `view_all_games`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `view_all_games`  AS SELECT `games`.`game_id` AS `game_id`, `games`.`game_name` AS `game_name`, `games`.`game_base_price` AS `game_base_price`, `games`.`game_discount_percentage` AS `game_discount_percentage`, `games`.`game_net_price` AS `game_net_price`, `games`.`game_release_year` AS `game_release_year`, `publishers`.`publisher_name` AS `publisher_name`, `games`.`game_rating` AS `game_rating`, `categories`.`category_name` AS `category_name` FROM (((`games` left join `publishers` on(`games`.`game_publisher_id` = `publishers`.`publisher_id`)) join `category_and_games` on(`category_and_games`.`game_id` = `games`.`game_id`)) join `categories` on(`categories`.`category_id` = `category_and_games`.`category_id`)) ORDER BY `games`.`game_id` ASC ;

-- --------------------------------------------------------

--
-- Structure for view `view_wallet`
--
DROP TABLE IF EXISTS `view_wallet`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `view_wallet`  AS SELECT `games`.`game_name` AS `game_name`, `publishers`.`publisher_name` AS `publisher_name`, `categories`.`category_name` AS `category_name`, `games`.`game_release_year` AS `game_release_year`, `games`.`game_rating` AS `game_rating` FROM ((((`games` join `libraryitems` on(`libraryitems`.`customer_id` = `variable`() and `libraryitems`.`game_id` = `games`.`game_id`)) join `publishers` on(`publishers`.`publisher_id` = `games`.`game_publisher_id`)) join `category_and_games` on(`category_and_games`.`game_id` = `games`.`game_id`)) join `categories` on(`categories`.`category_id` = `category_and_games`.`category_id`)) ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `bills`
--
ALTER TABLE `bills`
  ADD PRIMARY KEY (`bill_id`);

--
-- Indexes for table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`category_id`);

--
-- Indexes for table `customers`
--
ALTER TABLE `customers`
  ADD PRIMARY KEY (`customer_id`);

--
-- Indexes for table `games`
--
ALTER TABLE `games`
  ADD PRIMARY KEY (`game_id`);

--
-- Indexes for table `notification_messages`
--
ALTER TABLE `notification_messages`
  ADD PRIMARY KEY (`notification_message_id`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`order_id`);

--
-- Indexes for table `publishers`
--
ALTER TABLE `publishers`
  ADD PRIMARY KEY (`publisher_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `bills`
--
ALTER TABLE `bills`
  MODIFY `bill_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=55;

--
-- AUTO_INCREMENT for table `categories`
--
ALTER TABLE `categories`
  MODIFY `category_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `customers`
--
ALTER TABLE `customers`
  MODIFY `customer_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=67;

--
-- AUTO_INCREMENT for table `games`
--
ALTER TABLE `games`
  MODIFY `game_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;

--
-- AUTO_INCREMENT for table `notification_messages`
--
ALTER TABLE `notification_messages`
  MODIFY `notification_message_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `orders`
--
ALTER TABLE `orders`
  MODIFY `order_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=62;

--
-- AUTO_INCREMENT for table `publishers`
--
ALTER TABLE `publishers`
  MODIFY `publisher_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
