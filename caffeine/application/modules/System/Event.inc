<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\System;

class Event implements IEvent {
  // Array of handled Events by event
  // [EventTrigger] => array(Modules Handling)
  protected $_staticCache = NULL;

  protected function getApplication() {
    return \Ferrox\Caffeine\Application::getInstance();
  }

  protected function buildCache() {
    if ($this->_staticCache !== NULL) {
      return;
    }

    $application = $this->getApplication();
    $activeModules = $application->activeModules();
    $hookInfo = array();
    foreach($activeModules as $module) {
      $rawInfo = $module->getHandledEvents();
      $moduleId = $module->getId();
      foreach ($rawInfo as $info) {
        $key = $info['module'] . '::' . $info['name'];
        $hookInfo[$key][$moduleId] = $info;
      }
    }
    $this->_staticCache = $hookInfo;
  }

  protected function getTargets($moduleId, $name) {
    $this->buildCache();
    $key = $moduleId . '::' . $name;
    $results = array();
    if (\array_key_exists($key, $this->_staticCache)) {
      $handlers = $this->_staticCache[$key];
      $application = $this->getApplication();
      foreach ($handlers as $moduleId => $info) {
        $results[] = array($application->$moduleId(), $info['method']);
      }
    }
    return $results;
  }

  public function trigger ($moduleId, $name, $obj, $params) {
    $targetModules = $this->getTargets($moduleId, $name);
    foreach ($targetModules as $callable) {
      $callable[0]->$callable[1]($obj, $params);
      // \call_user_func_array($callable, array($obj) + $params);
    }
  }

}