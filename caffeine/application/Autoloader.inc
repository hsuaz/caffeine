<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine;

class Autoloader
  implements
    \Zend_Loader_Autoloader_Interface

{
  /**
   * Autoloader
   * @param $class string
   *  Classname (Absolute)
   */
  public function autoload($class) {
    $path = 'application/modules/';
    $parts = \explode('\\', $class);
    $extension = '.inc';
    $parts = \implode('/', \array_splice($parts, 2));
    $filename = $path . $parts . $extension;
    require_once($filename);
  }
}