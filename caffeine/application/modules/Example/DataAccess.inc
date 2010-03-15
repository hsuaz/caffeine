<?php
/**
 * Concrete Implementation of IDataAccess
 */
declare(encoding=UTF8);
namespace Ferrox\Caffeine\Example;
use
  Ferrox\Caffeine\System\DataAccess\IDataTransferObject;


/**
 * MySql Adaptor
 * @author IceWolf
 */
class DataAccess
  extends
    \Ferrox\Caffeine\System\Mapper\Mapper
  implements
    \Ferrox\Caffeine\User\IDataAccess
{
  /**
   * Table Name
   * @var stringn
   */
  protected $_tableName = 'users';


  /**
   * Primary Key
   */
  protected $_tablePrimaryKey = 'user_id';


  /**
   * Populates the DTO(array) from a database Row
   * Handles renaming and normalizing the keys
   */
  protected function _populateDataTransferObject ($databaseRow) {
    // Normalize the database results
    return new \Ferrox\Caffeine\System\DataAccess\DataTransferObject(
      array(
        'id' => $databaseRow['user_id'],
        'username' => $databaseRow['username'],
        'passwordHash' => $databaseRow['password'],
        'passwordSalt' => $databaseRow['salt'],
        'status' => $databaseRow['status'],
      )
    );
  }


  /**
   * Given a DTO, returns an array in a format suitable for the database.
   * @return array
   */
  private function _getDtoArray ($dto, $defaults=array()) {
    $data = array();
    if (isset($dto->username)) {
      $data['username'] = $dto->username;
    }

    if (isset($dto->password)) {
      $data['password'] = \md5($dto->password);
    }
    elseif (isset($dto->passwordHash)) {
      $data['password'] = $dto->passwordHash;
    }

    if (isset($dto->status)) {
      $data['status'] = $dto->status;
    }

    if (isset($dto->id)) {
      $data['user_id'] = $dto->id;
    }

    if (isset($dto->passwordSalt)) {
      $data['salt'] = $dto->passwordSalt;
    }

    // Assign Defaults
    $data += $defaults;
    return $data;
  }


  /**
   * Datamapper for the create function
   * Returns an array to insert, given the $dto
   * @param IDataTransferObject $dto
   * @return array
   */
  protected function _insert (IDataTransferObject $dto) {
    if (!isset ($dto->username)) {
      return FALSE;
    }
    $defaults = array(
      'password' => \md5(''),
      'salt' => \md5(time()),
      'status' => User::STATUS_ACTIVE,
    );
    $data = $this->_getDtoArray($dto, $defaults);
    return $data;
  }

  /**
   * Returns an array with changes to make given a delete request
   * This is a database update, not delete.
   * To handle a proper DELETE FROM override delete()
   */
  protected function _delete (IDataTransferObject $dto) {
    if (!isset ($dto->id)) {
      return FALSE;
    }
    $data = $this->_getDtoArray($dto);
    $data['status'] = User::STATUS_DELETED;
    return $data;
  }

  /**
   * Returns an array with changes to upadte given a DTO
   * @param IDataTransferObject $dto
   * @return unknown_type
   */
  protected function _update (IDataTransferObject $dto) {
    if (!isset ($dto->id)) {
      return FALSE;
    }
    $data = $this->_getDtoArray($dto);
    return $data;
  }


  /**
   * Implementation of IDataAccess custom function: findByUsername
   * Searches for all the rows that match the given username
   */
  public function findByUsername ($username) {
    $username = (string) $username;
    $select = $this->_dbR->select()
      ->from($this->_tableName)
      ->where('username=?', $username);
    $tableResult = $this->_dbR->fetchAll($select);
    $result = \array_map(array($this, '_populateDataTransferObject'), $tableResult);
    return $result;
  }

}