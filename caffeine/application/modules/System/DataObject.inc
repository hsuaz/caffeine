<?php
namespace Ferrox\Caffeine\System;

class DataObject
  implements \Serializable
{
  /**
   * fields set in the object
   * @var array
   */
  protected $_fields = array();

  public function __construct() {
  }


  public function toArray() {
    return $this->_fields;
  }


  public function serialize () {
    return serialize(array('_fields' => $this->_fields));
  }

  public function unserialize($serialized) {
    $data = unserialize($serialized);
    $this->_fields = $data['_fields'];
  }

  /**
   * Unset function
   *
   */
  public function __unset($key) {
    unset($this->_fields[$key]);
  }


  /**
   * Get function
   * @param $key string
   * @return mixed
   */
  public function __get($key) {
    if (\array_key_exists($key, $this->_fields)) {
      return $this->_fields[$key];
    }
    trigger_error("Undefined field $key.", E_USER_NOTICE);
  }


  /**
   *
   * @param $key
   * @param $value
   * @return NULL
   */
  public function __set($key, $value) {
    $fixedKey = \ucfirst($key);
    $method = 'preprocess' . $fixedKey;
    if (\method_exists($this, $method)) {
      $value = $this->$method($value);
    }

    $this->_fields[$key] = $value;
  }


  /**
   *
   * @param $key
   * @return boolean
   */
  public function __isset($key) {
    return isset($this->_fields[$key]);
  }


  public function reset() {
    $this->_fields = array();
  }
}