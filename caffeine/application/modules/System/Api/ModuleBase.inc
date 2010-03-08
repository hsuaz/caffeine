<?php
declare(encoding="UTF-8");
namespace Ferrox\Caffeine\System\Api;


/**
 * Base Module Class
 * @author IceWolf
 *
 */
abstract class ModuleBase implements IModule {
  const VERSION = "0.0";
  const NAME ="Module Base";
  const DESCRIPTION = "Abstract Module";
  const ID = "module_base";

  /**
   * Api Instance
   * @var mixed
   */
  static protected $_api = NULL;


  public function getName() {
    return static::NAME;
  }


  /**
   * Returns the version of the module
   * @return string
   */
  public function getVersion () {
    return static::VERSION;
  }


  /**
   * Return the description
   * @return string
   */
  public function getDescription () {
    return static::DESCRIPTION;
  }


  /**
   * Returns the programmatic id
   * @return string
   */
  public function getId () {
    return static::ID;
  }


  /**
   * Default constructior
   * @param $config array|\Zend_Config
   *   Configuration
   */
  public function __construct($config = array()) {
    $this->setConfig($config);
    $delayedInit = FALSE;
    if (\array_key_exists('delayedInit', $this->_config)) {
      $delayedInit = $this->_config['delayedInit'];
    }
    if (!$delayedInit) {
      $this->_init();
    }
  }

  public function setConfig ($config) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new Exception('Invalid configuration. Expecting array for Zend_Config.');
    }
  }


  public function init () {
    $this->_init();
  }


  protected function _initConfig() {
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }
  }

  /**
   * Initialization
   */
  protected function _init() {
    $this->_initConfig();
    $this->_initApi();
  }


  /**
   * API Initialization
   * @return unknown_type
   */
  abstract protected function _initApi();


  /**
   * Returns the internal use api
   * @return mixed
   */
  public function getApi () {
    return static::$_api;
  }


  /**
   * Returns the Internal API Definition
   * @return array
   */
  abstract public function routingInternalApi ();


  /**
   * Return the External API definition
   * @return array
   */
  abstract public function routingExternalApi ();


  /**
   * Load the routes this module implements
   * @param $section string
   *    Section to load
   * @return array
   *   Routing information
   */
  public function loadRoutes ($section) {
    $method = 'routing' . ucfirst($section);
    if (\method_exists($this, $method)) {
      return $this->$method();
    }
  }
}
