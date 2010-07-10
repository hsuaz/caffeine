<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User;
use
  \Ferrox\Caffeine\User\_module as _module,
  \Ferrox\Caffeine\System\Api as Api;

class ApiMethodBase
  extends
    Api\ApiMethodBase
{
  protected function _triggerEvent ($eventName, $params = array()) {
    $this->_event->trigger(_module::ID, $eventName, $this, $params);
  }
}
