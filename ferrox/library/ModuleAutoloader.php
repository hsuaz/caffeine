<?php
class ModuleAutoloader
  implements
    Zend_Loader_Autoloader_Interface

{
  /**
   * Autoloader
   * @param $class string
   *  Classname (Absolute)
   */
  public function autoload($class) {
    $parts = \explode('_', $class);
    $extension = '.php';
    $moduleName = $parts[1];
    $parts = \implode('/', $parts);
    $filename = APPLICATION_PATH . "/modules/$moduleName/Module.php";
    require_once($filename);
  }
}