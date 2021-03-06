<?php
declare(encoding='UTF-8');
define('CAFFEINE_CONFIG_PATH', realpath('../application/config'));
define('CAFFEINE_APPLICATION_PATH', realpath('../application'));
define('CAFFEINE_LIBRARY_PATH', realpath('../library'));

require_once ('./Bootstrap.php');
header("content-type: text/plain");

$bootstrap = new Bootstrap(CAFFEINE_CONFIG_PATH . '/internalApi.ini', 'development');
$applicationConfig = new \Zend_Config_Ini(CAFFEINE_CONFIG_PATH . '/caffeine.ini', 'development');
$bootstrap->setApplicationConfig($applicationConfig);
$bootstrap->run();
$config = $bootstrap->getConfig();


$dispatchConfig = $config->get('dispatcher', array());
if (!is_array($dispatchConfig)) {
  $dispatchConfig = $dispatchConfig->toArray();
}
$dispatchConfig['application'] = $bootstrap->getApplication();
$dispatcher = new \Ferrox\Caffeine\System\Api\ExternalDispatcher($dispatchConfig);

$routerConfig = $config->toArray();
$routerConfig['application'] = $bootstrap->getApplication();
$externalApi = $config->toArray();
$externalApi['dispatcher'] = $dispatcher;
$externalApi['routes'] = \Ferrox\Caffeine\System\Router::loadRoutes('externalApi', $routerConfig);

$api = new \Ferrox\Caffeine\System\Api($externalApi);
$api->run();
