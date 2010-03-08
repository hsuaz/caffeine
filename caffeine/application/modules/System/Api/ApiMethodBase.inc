<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\System\Api;

/**
 * Convenience class for executing an api callback
 * @author IceWolf
 */
abstract class ApiMethodBase {
  protected $_defaultOutputFormatter = '\Ferrox\System\Api\Output\PhpSerializer';
  protected $_output = array('result' => NULL);
  protected $_input = array();
  protected $_config = array();
  protected $_error = array();
  protected $_outputSent = FALSE;
  protected $_application = NULL;

  const ERROR_REQUIRED = 'error_required';


  /**
   * Default Constructor
   * @param $config array|\Zend_Config
   *  Configuration options
   */
  public function __construct ($config) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new Exception ('Invalid Configuration passed in.');
    }
  }


  protected function _getApplication() {
    return $this->_application;
  }


  /**
   * Runs the method call
   * @return array
   *   Results of the call
   */
  public function run() {
    // Validation
    $success = FALSE;
    $this->_init();
    if (!$this->_hasError()) {
      $success = $this->_process();
    }
    $this->_setOutput('status', $success);
    return $this->getOutput();
  }


  /**
   * Returns the output with errors mixed in.
   * @return array
   */
  protected function getOutput () {
    if ($this->_hasError() ) {
      $this->_output['error'] = $this->_error;
    }
    return $this->_output;
  }

  /**
   * Processes the request
   * @return boolean
   *   Success
   */
  abstract protected function _process();


  /**
   * Return the error state of this object
   * @return boolean
   */
  protected function _hasError() {
    return (count($this->_error) > 0);
  }


  /**
   * Initialization
   */
  protected function _init() {
    $this->_parseConfiguration();
    $this->_validateRequirements();
    $this->_validateInput();
  }


  /**
   * Parse configurations
   */
  protected function _parseConfiguration () {
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }

    if (\array_key_exists('request', $this->_config)) {
      $this->_input = $this->_config['request'];
    }

    if (\array_key_exists('defaults', $this->_config['routeOptions'])) {
      $this->_populateDefaults($this->_config['defaults']);
    }
  }


  /**
   * @todo: make an interface
   * @param $value
   */
  protected function setApplication ($value) {
    $this->_application = $value;
  }


  /**
   * Sets an output value
   * @param $key string | array
   * @param $value mixed
   */
  protected function _setOutput ($key, $value=NULL) {
    $this->_setArray($this->_output, $key, $value);
  }

  private function _setArray (&$array, $key, $value=NULL) {
    if (\is_array($key)) {
      foreach ($key as $item_key => $item_value) {
        $array[$item_key] = $item_value;
      }
    } else {
      $array[$key] = $value;
    }
  }

  protected function _setResult ($key, $value=NULL) {
    $this->_setArray($this->_output['result'], $key, $value);
  }

  /**
   * Populate the default request values that aren't provided
   * @param $vars array
   */
  protected function _populateDefaults($vars) {
    foreach ($vars as $key => $value) {
      if (!\array_key_exists($key, $this->_request)) {
        $this->_request[$key] = $value;
      }
    }
  }


  protected function _setError ($code, $message) {
    $error = array(
      'code' => $code,
      'message' => $message,
    );
    $this->_error[] = $error;
  }


  protected function _getRequiredParams () {
    return array();
  }


  protected function _validateInput () {
    $success = TRUE;
    foreach ($this->_input as $key => $value) {
      $method = 'validate' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $valid = $this->$method($value);
        $success = ($success && $value);
      }
    }
    return $success;
  }


  /**
   * Validates the required parameters are present
   * @return bool
   *   Success
   */
  protected function _validateRequirements () {
    $success = TRUE;
    $required = $this->_getRequiredParams();
    foreach ($required as $key) {
      if (!isset($this->_input[$key]) || empty($this->_input[$key])) {
        $this->_setError(static::ERROR_REQUIRED, sprintf('Required parameter "%1$s" missing.', $key));
        $success = FALSE;
      }
    }
    return $success;
  }
}