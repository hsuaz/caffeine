<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\System\DataAccess;

class DataTransferObject implements
    IDataTransferObject,
    \Iterator
{
  private $_fields = array();
  public function __construct (array $init = array()) {
    foreach ($init as $key => $value) {
      $this->$key = $value;
    }
  }


  public function rewind () { return reset($this->_fields); }
  public function current () { return current($this->_fields); }
  public function next () { return next($this->_fields); }
  public function key () { return key($this->_fields); }
  public function valid () { return (NULL === $this->key()); }

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
