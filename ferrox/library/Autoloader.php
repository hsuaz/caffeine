<?php
class Autoloader
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
    $parts = \implode('/', $parts);
    $filename = $parts . $extension;
    require_once($filename);
  }
}