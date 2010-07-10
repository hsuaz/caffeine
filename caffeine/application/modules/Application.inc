<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine;

/**
 * Application Handler
 * @author IceWolf
 *
 */
class Application {
  /**
   * Configuration
   * @var array
   */
  protected $_config = array();
  protected $_systemConfig = array();
  protected $_moduleAlias = array();

  protected $_objectCache = array();
  protected $_databaseCache = array();
  protected $_initialized = FALSE;

  protected $_modules = array();

  protected $_eventHandler = NULL;

  /**
   * Sets the configuration file
   * @param $config \Zend_Config|array
   */
  public function setConfig ($config) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new System\Exception('Configuration file must be an array.');
    }

    $this->_systemConfig = array();
    $this->_moduleAlias = array();
    if (\array_key_exists('system', $this->_config)) {
      if (\array_key_exists('config', $this->_config['system'])) {
        $this->_systemConfig = $this->_config['system']['config'];
      }
      if (\array_key_exists('module', $this->_config['system'])) {
        if (\array_key_exists('alias', $this->_config['system']['module'])) {
          $this->_moduleAlias = $this->_config['system']['module']['alias'];
        }
      }
    }
  }


  /**
   * Initializes the application data
   */
  public function init() {
    if ($this->_eventHandler === NULL) {
      $this->_initDefaultEventHandler();
    }
    $this->_loadModules();
  }


  //
  //  Module Related Functionality
  //  @todo Should probably factor out
  //


  /**
   * Loads all the installed modules and sets up the configuration data.
   */
  protected function _loadModules() {
    $installedModules = $this->_config['system']['module']['installed'];
    $enabledModules = $this->_config['system']['module']['enabled'];
    foreach ($installedModules as $className) {
      // Initialize the information centrally
      $moduleClass = $className . '\_module';
      $instance = new $moduleClass(array('delayedInit' => TRUE));
      $module = array(
        'name' => $instance->getName(),
        'description' => $instance->getDescription(),
        'id' => $instance->getId(),
        'instance' => $instance,
        'enabled' => in_array($instance->getId(), $enabledModules),
      );

      // Check for duplicates
      if (\array_key_exists($module['id'], $this->_modules)) {
        throw new Exception(sprintf('Duplication module ID found in installed modules: %1$s', $module['id']));
      }


      // Module initialization code
      if (\array_key_exists($module['id'], $this->_config)
          && \array_key_exists('__moduleInit', $this->_config[$module['id']]))
      {
        $instance->setConfig($this->_config[$module['id']]['__moduleInit']);
      }
      $instance->init();


      $this->_modules[$module['id']] = $module;
    }
  }


  /**
   * Return a list of the active module definition instances
   * @return array
   */
  public function activeModules() {
    static $instances = NULL;
    if (!empty($instances)) {
      return $instances;
    }

    $instances = array();
    foreach ($this->_modules as $id => $info) {
      if ($info['enabled']) {
        $instances[$id] = $info['instance'];
      }
    }

    return $instances;
  }


  public function getApi($moduleName) {
    $api = NULL;
    $systemName = $this->_getSystemName($moduleName);
    if (\array_key_exists($systemName, $this->_modules)) {
      $info = $this->_modules[$systemName];
      if ($info['enabled']) {
        $instance = $info['instance'];
        $api = $instance->getApi();
      }
    }

    return $api;
  }


  /**
   * Return the Database Connection by Name
   * @param $connectionName string
   *  Database Name
   * @return \Zend_Db
   */
  protected function _getDatabaseConnection ($connectionName) {
    if (!\array_key_exists($connectionName, $this->_config['database'])) {
      throw new System\Exception('Unknown Database Instance');
    }
    $dbConfig = $this->_config['database'][$connectionName];

    try {
      $db = \Zend_Db::factory(new \Zend_Config($dbConfig));
      $db->getConnection();
    } catch (\Zend_Db_Adapter_Exception $e) {
      throw new System\Exception('Database not initialized.');
    }
    return $db;
  }



  /**
   * Return the system name resolving any aliases
   * @param $name string
   * @return string
   */
  protected function _getSystemName ($name) {
    if (\array_key_exists($name, $this->_moduleAlias)) {
      return $this->_moduleAlias[$name];
    }
    return $name;
  }


  /**
   * Method Invoker
   * @param $method string
   * @param $params array
   */
  public function __call($key, $params) {
    $systemName = $this->_getSystemName($key);
    $method = 'get_' . $systemName;
    $instance = NULL;
    if (\method_exists($this, $method)) {
      $config = array();
      $moduleConfig = array();
      if (\array_key_exists($systemName, $this->_systemConfig)) {
        $config = $this->_systemConfig[$systemName];
      }
      if (\array_key_exists($systemName, $this->_config)) {
        $moduleConfig = $this->_config[$systemName];
      }
      $instance = $this->$method($config, $moduleConfig);
    } else {
      \trigger_error(\sprintf('Invalid Method Call %1$s', $key), E_USER_NOTICE);
    }
    return $instance;
  }


  protected function setEventHandler (\Ferrox\Caffeine\System\IEvent $event) {
    $this->_eventHandler = $event;
  }


  protected function _initDefaultEventHandler () {
    $this->_eventHandler = new \Ferrox\Caffeine\System\Event($this);
    return $this->_eventHandler;
  }

  protected function get_system_user ($applicationConfig, $moduleConfig) {
    $dataAccess = new $applicationConfig['dataAccess'](
        array(
          'databaseRead' => $this->_getDatabaseConnection($applicationConfig['databaseRead']),
          'databaseWrite' => $this->_getDatabaseConnection($applicationConfig['databaseWrite']),
        )
      );
    $moduleConfig['dataAccess'] = $dataAccess;
    $moduleConfig['eventHandler'] = $this->_eventHandler;
    $user = new User\User($moduleConfig);
    return $user;
  }

  protected function get_news ($applicationConfig, $moduleConfig) {
    $dataAccess = new $applicationConfig['dataAccess'](
        array(
          'databaseRead' => $this->_getDatabaseConnection($applicationConfig['databaseRead']),
          'databaseWrite' => $this->_getDatabaseConnection($applicationConfig['databaseWrite']),
        )
      );
    $moduleConfig['dataAccess'] = $dataAccess;
    $news= new News\News($moduleConfig);
    return $news;
  }

  protected function get_user_profile ($applicationConfig, $moduleConfig) {
    $dataAccess = new $applicationConfig['dataAccess'](
        array(
          'databaseRead' => $this->_getDatabaseConnection($applicationConfig['databaseRead']),
          'databaseWrite' => $this->_getDatabaseConnection($applicationConfig['databaseWrite']),
        )
      );
    $moduleConfig['dataAccess'] = $dataAccess;
    $profile = new Profile\Profile($moduleConfig);
    return $profile;
  }

}