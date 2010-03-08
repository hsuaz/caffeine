<?php

class Installer {
  public function install () {
    $sql = "
    CREATE TABLE `news` (
      `news_id` int(11) NOT NULL AUTO_INCREMENT,
      `title` varchar(50) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
      `locale` varchar(8) CHARACTER SET ascii NOT NULL,
      `published` datetime NOT NULL,
      `author` int(11) NOT NULL,
      `content` mediumtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
      `status` smallint(6) NOT NULL DEFAULT '1',
      PRIMARY KEY (`news_id`),
      KEY `published_index` (`published`)
    ) ENGINE=InnoDB DEFAULT CHARSET=latin1
    ";
  }
}