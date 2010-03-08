<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User\Api;
use \Ferrox\Caffeine\System\Api as Api;

class FindById
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
      'uid',
    );
  }


  /**
   * Process
   * @return boolean
   *  Success status
   */
  protected function _process () {
    $userId = $this->_input['uid'];

    $user = $this->_getApplication()->user();
    $foundUser = $user->find($userId);
    if (!$foundUser) {
      $this->_setError(static::ERROR_USER_NOT_FOUND, 'User not found');
      return FALSE;
    }

    $this->_setResult('user', $user->toArray());
    return TRUE;
  }
}