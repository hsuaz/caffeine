<?php
namespace Ferrox\Caffeine\System;

class Router {
  static function loadRoutes($section, $config=array()) {
    if ($config instanceof \Zend_Config) {
      $config = $config->toArray();
    } elseif (is_array($config)) {
    } else {
      throw new Exception('Config must be instnace of Zend_Config or array');
    }

    // Allow for Dependance Injection of the application
    if (\array_key_exists('application', $config)) {
      $application = $config['application'];
    } else {
      $application = \Ferrox\Caffeine\Application::getInstance();
    }


    $routes = array();
    foreach ($application->activeModules() as $moduleHook) {
      $newRoutes = $moduleHook->loadRoutes($section);
      if (\is_array($newRoutes)) {
        $routes += $newRoutes;
      }
    }
    return $routes;
  }
}