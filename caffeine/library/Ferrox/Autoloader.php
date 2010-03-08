<?php
namespace Ferrox;

class Autoloader
  implements
    \Zend_Loader_Autoloader_Interface

{
  public function autoload($class) {
    $path = 'library/';
    $extension = '.php';
    $split_key = '\\';
    // If the class isn't namespaced, use PEAR style
    if (FALSE === strpos($class, '\\')) {
      $split_key = '_';
    }
    $parts = str_replace($split_key, DIRECTORY_SEPARATOR, $class);
    $filename = $path . $parts . $extension;
    require_once($filename);
  }
}