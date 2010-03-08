<?php

class Installer {
  public function install () {
    $sql = '
    CREATE TABLE users (
      user_id INT NOT NULL AUTO_INCREMENT,
      username VARCHAR(50) CHARSET UTF8 COLLATE UTF8_UNICODE_CI NOT NULL,
      password VARCHAR(64) CHARSET ASCII COLLATE ASCII_GENERAL_CI NOT NULL,
      salt VARCHAR(32) CHARSET ASCII COLLATE ASCII_GENERAL_CI NOT NULL,
      status TINYINT UNSIGNED NOT NULL,
      PRIMARY KEY (user_id),
      UNIQUE INDEX users_username (username)
    ) ENGINE INNODB;
    ';
  }
}