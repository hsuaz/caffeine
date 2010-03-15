<?php
/**
 * Example Module.
 * Heavily Commented.
 * Reference.
 */

/**
 * File Encoding, MUST be the first line of a file.
 */
declare(encoding='UTF-8');

/**
 * Namespace Declaration should be in the format of Ferrox\Caffeine\{ModuleName}
 * MUST be the first line of the file, after the declare statement
 */
namespace Ferrox\Caffeine\Example;


/**
 * Installer Class
 * This class will handle all the installation, and database upgrades
 * This file must be called _install.inc
 *
 * @TODO Not currently Implemented
 * @TODO Inject Database connection settings
 * @TODO Define configuration.ini format for settings
 *
 * This class must be named "Installer"
 */
class Installer {
  /**
   * Installs the module to the database
   * Actions required to install the database.
   *
   * This function is expected to install the latest version of the schema.
   *
   * @return boolean
   *  Success status of the install.
   */
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


  /**
   * In case schema updates are required, the installer module will be able
   * to run all updates defined in this class consecutively, based upon the
   * last update run.
   *
   * On initial install, the class will be inspected to determine the update
   * level, as it is assumed that the install() function reflects all updates.
   *
   * These update functions must be named "update_n" where n is an integer
   * with no gaps in order.
   *
   * Updates start at n=1
   *
   * @return boolean
   *  Success status of the install
   */
  public function update_1 () {

  }
}