<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\User;
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
  protected $_tableName = 'users';
  protected $_tablePrimaryKey = 'user_id';

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
    foreach ($defaults as $key=>$value) {
      if (!\array_key_exists($key, $data)) {
        $data[$key] = $value;
      }
    }
    return $data;
  }

  /**
   * Datamapper for the create function
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

  protected function _delete (IDataTransferObject $dto) {
    if (!isset ($dto->id)) {
      return FALSE;
    }
    $data = $this->_getDtoArray($dto);
    $data['status'] = User::STATUS_DELETED;
    return $data;
  }

  /**
   *
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