<?php
declare(encoding="UTF-8");
namespace Ferrox\Caffeine\System\Api;


/**
 * Base Module Class
 * @author IceWolf
 *
 */
abstract class ModuleBaseDynamicApi extends ModuleBase {
  protected $_apiDispatcher = NULL;

  /**
   * Api Dispatcher
   */
  protected function setApiDispatcher ($config) {
    $className = $config['_class'];
    $options = $config;
    unset($options['_class']);
    $dispatcher = new $className($options);
    $this->_apiDispatcher = $dispatcher;
  }


  /**
   * Initialize API
   * @return \Ferrox\System\Api\DynamicApi
   */
  protected function _initApi() {
    $routes = $this->loadRoutes('internalApi');
    $config = array(
      'routes' => $routes,
      'dispatcher' => $this->_apiDispatcher,
    );
    static::$_api = new \Ferrox\System\Api\DynamicApi ($config);
  }
}
