<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\System;

interface IEvent {
  public function trigger ($moduleId, $name, $obj, $params);
}