<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User\Api;
use \Ferrox\Caffeine\System\Api as Api;

class FindByUsername
  extends
    Api\ApiMethodBase
{
  /**
   * Error: User not found
   * @var string
   */
  const ERROR_USER_NOT_FOUND = 'user_not_found';




  /**
   * Required Parameters
   * @return array
   */
  protected function _getRequiredParams () {
    return array(
      'username',
    );
  }


  /**
   * Process
   * @return boolean
   *  Success status
   */
  protected function _process () {
    $username = $this->_input['username'];

    $user = $this->_getApplication()->user();
    $foundUser = $user->findByUsername($username);
    if (!$foundUser) {
      $this->_setError(static::ERROR_USER_NOT_FOUND, 'User not found');
      return FALSE;
    }

    $this->_setResult('user', $user->toArray());
    return TRUE;
  }
}