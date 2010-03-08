<?php
declare(encoding='UTF-8');
namespace Ferrox\System\Api;

/*
 * Generate a resource list of the sort:
 * moduleId =>
 *  methodName =>
 *   options
 */
class DynamicApi {
  /**
   * Routing Information
   * @var array
   */
  protected $_routingInformation = array();


  /**
   * Dispatcher
   * @var \Ferrox\System\Api\IDispatcher
   */
  protected $_dispatcher = NULL;


  /**
   * Configuration
   * @var array
   */
  protected $_config = array();


  protected $_lastResponse = NULL;

  /**
   * Default Constructor
   * @param $config array
   */
  public function __construct ($config) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new Exception('Config expected to be instance of Zend_Config or arary');
    }
    $this->_init();
  }


  /**
   * Initialization
   */
  protected function _init() {
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }
    if (!($this->_dispatcher instanceof \Ferrox\System\Api\IDispatcher)) {
      throw new Exception('No dispatcher provided.');
    }
  }


  /**
   * Set the Dispatcher
   * @param $dispatcher IDispatcher
   * @return unknown_type
   */
  protected function setDispatcher(\Ferrox\System\Api\IDispatcher $dispatcher) {
    $this->_dispatcher = $dispatcher;
  }


  /*
   * Set the routs
   */
  protected function setRoutes ($routes) {
    if (!\is_array($routes)) {
      \trigger_error('Routeing configuration must be an array.', E_USER_NOTICE);
      return;
    }
    foreach ($routes as $path => $routeInfo) {
      if (!array_key_exists('api', $routeInfo)) {
        continue;
      }
      $apiInfo = $routeInfo['api'];
      unset($routeInfo['api']);
      $routeConfig['api'] = $apiInfo;
      $routeConfig['api']['target'] = $path;
      $routeConfig['options'] = $routeInfo;
      $this->_routingInformation[$apiInfo['name']] = $routeConfig;
    }
  }

  /**
   * Default method dispatcher
   * @param $route array
   *   target route
   * @param $params array
   *   Parameters
   * @return array
   *   Results
   */
  protected function _call($route, $params) {
    $this->_lastResponse = NULL;
    $output = $this->_dispatcher->dispatch($route['api']['target'], $route['options'], $params);
    $this->_lastResponse = $output;
    if (!$output['status']) {
      return NULL;
    }
    return $output['result'];
  }


  public function getLastError() {
    if (!\is_array($this->_lastResponse)) {
      trigger_error('No request made', E_USER_NOTICE);
      return NULL;
    }

    if (\array_key_exists('error', $this->_lastResponse)) {
      return $this->_lastResponse;
    }
    return array();
  }
  /**
   *
   * @param $paramDef
   * @param $params
   * @return unknown_type
   */
  protected function _mapInput ($paramDef, $params) {
    $result = array();
    foreach ($paramDef as $name => $default) {
      $value = \array_shift($params);
      if ($value !== NULL) {
        $result[$name] = $value;
        continue;
      }

      if ($default === NULL) {
        // Missing Required Parameter
        trigger_error(sprintf('Insufficient parameters passed in, expecting %1$d found %2$d', count($paramDef), count($params)), E_USER_ERROR);
      }
      $result[$name] = $default;
    }
    return $result;
  }


  /**
   * Defualt method invoker
   * @param $name
   * @param $params
   * @return array
   */
  public function __call($name, $params) {
    if (\array_key_exists($name, $this->_routingInformation)) {
      $route = $this->_routingInformation[$name];
      $params = $this->_mapInput($route['api']['parameters'], $params);
      return $this->_call($route, $params);
    } else {
      trigger_error(sprintf('Function %1$s does not exist.', $name), E_USER_ERROR);
    }
  }
}