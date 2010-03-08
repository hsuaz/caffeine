<?php
namespace Ferrox\Caffeine\System;
use
    \Ferrox\System\Net\HttpStatus as HttpStatus;

/**
 * Api class
 * @author IceWolf
 *
 */
class Api {
  const E_UNKNOWN = 'api_unknown';
  const E_UNKNOWN_METHOD = 'unknown_method';

  /**
   * Output Formatter Classname
   * @var string
   */
  protected $_defaultOutputFormatter = '\Ferrox\System\Api\Output\PhpSerializer';


  /**
   * Validators to run
   * @var array
   */
  protected $_validators = array();


  /**
   * Configuration
   * @var array
   */
  protected $_config = array();


  /**
   * Pending Messages
   * @var array
   */
  protected $_messages = array();


  /**
   * Output Data
   * @var array
   */
  protected $_output = array();


  /**
   * Incoming request data
   * @var array
   */
  protected $_request = array();


  /**
   * API Dispatcher
   * @var \Ferrox\System\Api\IDispatcher
   */
  protected $_dispatcher = NULL;


  /**
   * Parrot the data in the request
   * @var boolean
   */
  protected $_parrot = FALSE;


  /**
   * Assign the config variable
   * @param $config \Zend_Config | array
   */
  protected function _setConfig ($config) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new Exception('Configuration file must be an array.');
    }
  }


  /**
   * Set request parroting
   * @param $value bool
   *  Parroting Status
   */
  protected function setParrot($value) {
    $this->_parrot = (bool) $value;
  }


  /**
   * Set the dispatcher instance
   * @param $dispatcher
   */
  protected function setDispatcher($dispatcher) {
    if (!($dispatcher instanceof \Ferrox\System\Api\IDispatcher)) {
      throw new Exception ('Not a valid Dispatcher');
    }
    $this->_dispatcher = $dispatcher;
  }


  /**
   * Initialization functions
   */
  protected function _init() {
    $this->_request = $_REQUEST;
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }
    $this->_ensureDefaults($this->_config['request']['default']);

    // Requirements
    if ($this->_dispatcher === NULL) {
      throw new Exception ('No Dispatcher Configured.');
    }
  }


  /**
   * Populate the default request values that aren't provided
   * @param $vars array
   */
  protected function _ensureDefaults($vars) {
    foreach ($vars as $key => $value) {
      if (!\array_key_exists($key, $this->_request)) {
        $this->_request[$key] = $value;
      }
    }
  }


  /**
   * Adds a validator to a given key
   * @param $key string
   *  Request string to validate on
   * @param $validator Ferrox\Caffeine\System\IValidator
   */
  public function addValidator ($key, $validator) {
    if (!\array_key_exists($key, $this->_validators)) {
      $this->_validators[$key] = array();
    }

    if (!($validator instanceof Ferrox\System\IValidator)) {
      throw new Exception ('Invalid validator passed in.');
    }

    $this->_validators[$key] = $validator;
  }


  /**
   * Add an error message to the instance
   * @param $messages array
   *   array in the format of [errorcode] => [message]
   * @return unknown_type
   */
  protected function _addErrors($messages = array()) {
    if (!\array_key_exists('error', $this->_output)){
      $this->_output['error'] = array();
    }

    if (!\is_array($messages)) {
      throw new Exception('Invalid Message.');
    }

    foreach ($messages as $code => $message) {
      $this->_output['error'][] = array(
        'code' => $code,
        'message' => $message,
      );
    }
  }


  /**
   * An error has been logged
   * @return boolean
   */
  protected function _hasError() {
    return (\array_key_exists('error', $this->_output));
  }


  /**
   * Runs all the validation routines
   * @return unknown_type
   */
  protected function _runValidators () {
    foreach ($this->_validators as $key => $validator) {
      $value = \array_key_exists($key, $_REQUEST)? $_REQUEST[$key] : NULL;
      $success = $success && $validator->isValid($value);
      if (!$success) {
        $this->_addErrors($validator->getMessages());
      }
    }
  }


  /**
   * Default Constructor
   * @param $config \Zend_Config|array
   *  Configuration options
   */
  public function __construct ($config = array()) {
    $this->_setConfig($config);
    $this->_init();
  }


  /**
   * Send the response back to the client
   */
  protected function _sendOutput() {
    // Default
    $className = $this->_defaultOutputFormatter;
    if (\array_key_exists($this->_request['output'], $this->_config['formatter'])) {
      $className = $this->_config['formatter'][$this->_request['output']];
    }
    // PHP BUG:
    if ($className[0] == '\\') {
      $className = substr($className, 1);
    } else {
      $className = __NAMESPACE__ . '\\' . $className;
    }

    $output = new $className();
    if ($className instanceof \Ferrox\System\Api\Output\IOutputFormatter) {
      throw Exception('Invalid output implementation.');
    }
    echo $output->send($this->_output);
  }


  /**
   * Dispatch the request to the appropriate handler
   */
  protected function _dispatch () {
    $target = $_SERVER['PATH_INFO'];

    $routes = $this->_config['routes'];
    if (\array_key_exists($target, $routes)) {
      $output = $this->_dispatcher->dispatch($target, $routes[$target], $this->_request);
      if (\array_key_exists('__httpStatus', $output)) {
        HttpStatus::send($output['__httpStatus']);
        unset($output['__httpStatus']);
      }

      if (\is_array($output)) {
        $this->_output = \array_merge($this->_output, $output);
      }
    }
    // Unknown Method, Bail.
    else {
      HttpStatus::send(HttpStatus::HTTP_BAD_REQUEST);
      $this->_addErrors(array(self::E_UNKNOWN_METHOD => 'Unknown API Call'));
    }
  }


  /**
   * Execute the request
   */
  public function _execute() {
    // Run the validation
    $this->_runValidators();

    // If there is an error, do not process the request at all.
    if ($this->_hasError()) {
      return;
    }

    $this->_dispatch();
  }


  /**
   * Execute the request
   * @return unknown_type
   */
  public function run (){
    $this->_output['status'] = TRUE;
    try
    {
      $this->_execute();
    }
    // Api Exceptions
    // Will contain a code and error message to relay back to the user
    catch (Api\Exception $e)
    {
      $this->_addErrors($e->getError());
    }
    // General Exceptions
    // Adjust the header as werr
    catch (\Exception $e)
    {
      HttpStatus::send(HttpStatus::HTTP_SERVER_ERROR);
      $this->_addErrors(array(self::E_UNKNOWN, 'An unknown error occurred.'));
    }

    // If there are any errors, this cannot be successful
    if ($this->_hasError()) {
      $this->_output['status'] = FALSE;
    }

    // Check for parroting of input data
    if ($this->_parrot) {
      $this->_output['parrot'] = $this->_request;
    }
    $this->_sendOutput();
    exit;
  }
}