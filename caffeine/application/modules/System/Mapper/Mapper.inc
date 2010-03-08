<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\System\Mapper;
use
  Ferrox\Caffeine\System\DataAccess\IDataTransferObject,
  Ferrox\Caffeine\User\Exception as Exception;
/**
 * Base Mapper Class
 * @author IceWolf
 *
 */
abstract class Mapper
  implements
    \Ferrox\Caffeine\System\Mapper\IMapper,
    \Ferrox\Caffeine\System\Mapper\IFinder
{
  /**
   * The table name for this mapper instance
   * @var string
   */
  protected $_tableName = 'dual';


  /**
   * The primary key for the table
   * @var string
   */
  protected $_tablePrimaryKey = 'id';


  /**
   * Database Instance
   * @var Zend_Db_Abstract
   */
  protected $_dbR = NULL;

  protected $_dbW = NULL;

  /**
   * Configuration options
   * @var array
   */
  protected $_config = array();


  /**
   * Default Constructor
   * @param array $config
   * @return unknown_type
   */
  public function __construct ($config = array()) {
    if (!is_array($config)) {
      if ($config instanceof Zend_Config) {
        $config = $config->toArray();
      } else {
        throw new Exception('Mapper parameters must be in an array or a Zend_Config object');
      }
    }
    $this->_dbR = array_key_exists('databaseRead', $config) ? $config['databaseRead'] : NULL;
    $this->_dbW = array_key_exists('databaseWrite', $config) ? $config['databaseWrite'] : NULL;
    $this->_event = array_key_exists('event', $config)  ? $config['event'] : NULL;
    $this->_log = array_key_exists('logger', $config)  ? $config['logger'] : NULL;
    if (empty($this->_dbW) || empty($this->_dbR)) {
      throw new Exception('Database instance(s) required to be set.');
    }
  }


  /**
   * Throws an event
   * @param $name
   *  string  Event Name
   * @param $params
   *  array Event Parameters
   * @return
   *  array Event Results
   */
  public function throwEvent ($name, $params = array()) {
    if (!isset ($this->_event)) {
      return;
    }

    throw new Exception('Unimplmented');
  }


  abstract protected function _insert (IDataTransferObject $dto);

  /**
   * Creates a row in the database backend
   * @param IDataTransferObject $dto
   * @return int Last Inserted Id
   */
  public function insert (IDataTransferObject $dto) {
    $create = $this->_insert($dto);

    if (!$create) {
      // TODO LOG THE ERROR
      return FALSE;
    }

    $this->_dbW->beginTransaction();

    try {
      $rowsAffected = $this->_dbW->insert($this->_tableName, $create);
    } catch (\Zend_Db_Statement_Exception $e ) {
      // TODO: LOG THE ERROR
      var_dump($e);
      $this->_dbW->rollBack();
      return FALSE;
    }

    if (0 == $rowsAffected) {
      $this->_dbW->rollBack();
      return FALSE;
    }

    // Get the ID of the created entry
    $id = $this->_dbW->lastInsertId();
    $dto->id = $id;
    $results = $this->throwEvent('create', array($dto));
    $this->_dbW->commit();
    return $id;
  }


  abstract protected function _update (IDataTransferObject $dto);

  /**
   * Updates a row in the database backend
   * @param IDataTransferObject $dto
   * @return boolean Successful update
   */
  public function update (IDataTransferObject $dto) {
    $update = $this->_update($dto);
    if (!$update) {
      return FALSE;
    }
    $this->_dbW->beginTransaction();

    $results = $this->throwEvent('update', array($dto));
    $where = array(
      "{$this->_tablePrimaryKey} = ?" => $dto->id,
    );

    try {
      $rowsAffected = $this->_dbW->update($this->_tableName, $update, $where);
    } catch (\Zend_Db_Statement_Exception $e) {
      // TODO: Log the Error
      $this->_dbW->rollBack();
      return FALSE;
    }

    if (0 == $rowsAffected) {
      $this->_dbW->rollBack();
      return FALSE;
    }

    $this->_dbW->commit();
    return TRUE;
  }


  abstract protected function _delete (IDataTransferObject $dto);

  /**
   * Deletes a row from the database (soft)
   * @param IDataTransferObject $dto
   * @return boolean Successful Update
   */
  public function delete (IDataTransferObject $dto) {
    $delete = $this->_delete($dto);
    if (!$delete) {
      return FALSE;
    }
    $this->_dbW->beginTransaction();

    $results = $this->throwEvent('delete', array($dto));
    $where = array(
      "{$this->_tablePrimaryKey} = ?" => $dto->id,
    );

    try {
      $rowsAffected = $this->_dbW->update($this->_tableName, $delete, $where);
    } catch (\Zend_Db_Statement_Exception $e) {
      //TODO: Log
      $this->_dbW->rollBack();
      return FALSE;
    }

    if (0 == $rowsAffected) {
      $this->_dbW->rollBack();
      return FALSE;
    }

    $this->_dbW->commit();
    return TRUE;
  }


  /**
   * Returns a row from the database by the given Id
   * @param IDataTransferObject $dto
   * @return IDataTransferObject
   *  Row Result
   */
  public function retrieve (IDataTransferObject $dto) {
    return $this->find ($dto->id);
  }


  /**
   * Returns the validator for the primary key value
   */
  protected function _validatePrimaryKey ($value) {
    return is_numeric($value) && ($value > 0);
  }


  /**
   * Returns a row from the database by the given Id
   * @param $id int Id to retrieve
   * @return IDataTransferObject
   */
  public function find($id) {
    if (!$this->_validatePrimaryKey($id)) {
      throw new Exception ('Invalid key format');
    }
    $select = $this->_dbR->select()
      ->from($this->_tableName)
      ->where("{$this->_tablePrimaryKey} = ?", $id);
    $tableResult = $this->_dbR->fetchAll($select);
    $result = \array_map(array($this,'_populateDataTransferObject'), $tableResult);
    return $result;
  }

  abstract protected function _populateDataTransferObject ($databaseRow);
}
