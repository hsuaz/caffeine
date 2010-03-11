<?php
declare(encoding='UTF-8');
namespace Ferrox\Filter;
class NormalizeUtf8
  implements
    \Zend_Filter_Interface
{
  public function filter($value) {
    if (\mb_check_encoding($value, 'UTF-8')) {
      return '';
    }
    $value = \Normalizer::normalize($value, \Normalizer::FORM_D);
    return $value;
  }
}