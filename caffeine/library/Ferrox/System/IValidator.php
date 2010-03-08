<?php
declare(encoding='UTF-8');
namespace Ferrox\System;

interface IValidator {
  public function isValid ($value);
}