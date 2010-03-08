<?php
declare(encoding="UTF-8");
namespace Ferrox\Caffeine\System\Api;


/**
 * Internal Dispatche
 * Primary use is to call an internal class to process a command directly.
 * @author IceWolf
 *
 */
class InternalDispatcher
  implements
    \Ferrox\System\Api\IDispatcher
{
  /**
   * Configuration
   * @var array
   */
  protected $_config = array();


  /**
   * Default Constructior
   * @param $config Zend_Config | array
   */
  public function __construct ($config = array()) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new Exception('Invalid configuration.');
    }
    $this->_init();
  }


  /**
   * Initialization
   */
  protected function _init () {
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }
    if (!\array_key_exists('application', $this->_config)) {
      throw new \Ferrox\Caffeine\System\Exception('Application not provided.');
    }
  }


  /**
   * Executes the method handler
   * @param $options array
   *  Routing options
   * @param $route string
   *  Matched route
   * @param $request array
   *  Incoming request
   */
  protected function _execute ($options, $route, $request) {
    $className = $options['class'];

    // PHP BUG:
    if ($className[0] == '\\') {
      $className = substr($className, 1);
    } else {
      $className = __NAMESPACE__ . '\\' . $className;
    }

    $config = array(
      'route' => $route,
      'request' => $request,
      'routeOptions' => $options,
      'application' => $this->_config['application'],
    );
    $class = new $className($config);
    $result = $class->run();
    return $result;
  }


  /**
   * Dispatch the request to the proper router
   * @param $route string
   *  Matched Route
   * @param $options array
   *  Routing OPtions
   * @param $request array
   *  Request Variables
   * @return array
   *  Result of the call
   */
  public function dispatch ($route, $options, $request) {
    $result = array();
    try {
      $result += $this->_execute($options, $route, $request);
    } catch (\Exception $e) {
      $result['success'] = FALSE;
      $result['error'] = 'Unknown Error';
    }
    return $result;
  }
}