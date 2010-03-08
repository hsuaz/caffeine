<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User\Api;
use \Ferrox\Caffeine\System\Api as Api;

class CyclePasswordSalt
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
    $uid = $this->_input['uid'];

    $user = $this->_getApplication()->user();
    $result = $user->find($uid);
    if (!$result) {
      $this->_setError(static::ERROR_USER_NOT_FOUND, 'Unknown username or password');
      return FALSE;
    }

    $result = $user->cyclePasswordSalt();
    return $result;
  }
}