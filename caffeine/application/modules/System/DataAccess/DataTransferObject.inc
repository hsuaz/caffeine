<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\System\DataAccess;

class DataTransferObject implements
    IDataTransferObject
{
  private $_fields = array();
  public function __construct (array $init = array()) {
    foreach ($init as $key => $value) {
      $this->$key = $value;
    }
  }

  public function __get($key) {
    if (array_key_exists($key, $this->_fields)) {
      return $this->_fields[$key];
    }
    trigger_error("Undefined field $key.", E_USER_NOTICE);
  }

  public function __set($key, $value) {
    $this->_fields[$key] = $value;
  }


  public function __isset($key) {
    return isset($this->_fields[$key]);
  }


  public function toArray () {
    return $this->_fields;
  }
}
