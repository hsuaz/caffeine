<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User\Api;
use
  \Ferrox\Caffeine\System\Api as Api,
  \Ferrox\Caffeine\User\User as User;


class Logout
  extends Api\ApiMethodBase
{
  protected function _getRequiredParams () {
    return array('sid');
  }

  protected function _process () {
    $session_id = $this->_input('sid');
    \Zend_Session::setId($session_id);
    \Zend_Session::start();
    \Zend_Session::destroy(FALSE, FALSE);
    return TRUE;
  }
}