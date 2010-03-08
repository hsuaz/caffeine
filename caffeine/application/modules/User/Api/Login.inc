<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User\Api;
use
  \Ferrox\Caffeine\System\Api as Api,
  \Ferrox\Caffeine\User\User as User;

class Login
  extends
    Api\ApiMethodBase
{
  /**
   * Error: User not found
   * @var string
   */
  const ERROR_USER_NOT_FOUND = 'user_not_found';


  /**
   * Error: Bad password
   * @var string
   */
  const ERROR_NO_PASSWORD = 'invalid_password';


  /**
   * Required Parameters
   * @return array
   */
  protected function _getRequiredParams () {
    return array(
      'username',
      'passwordHash',
    );
  }


  /**
   * Process
   * @return boolean
   *  Success status
   */
  protected function _process () {
    $username = $this->_input['username'];
    $passwordHash = $this->_input['passwordHash'];

    $user = $this->_getApplication()->user();
    $result = $user->findByUsername($username);
    if (!$result) {
      $this->_setError(static::ERROR_USER_NOT_FOUND, 'Unknown username or password');
      return FALSE;
    }
    $validPassword = $this->_validatePasswordHash($user, $passwordHash);
    $user->cyclePasswordSalt();
    if (!$validPassword) {
      $this->_setError(static::ERROR_NO_PASSWORD, 'Unknown username or password');
      return FALSE;
    }

    // Valid Username and Password
    // User Login Hooks go here
    // Set up the session data
    $sid = $this->_initializeSession($user);
    $this->_setResult('sid', $sid);
    return TRUE;
  }

  protected function _initializeSession (User $user) {
    \Zend_Session::start();
    \Zend_Session::regenerateId();
    $sid = \Zend_Session::getId();
    $systemSession = new \Zend_Session_Namespace('system');
    $systemSession->user = $user->toArray();
    return $sid;
   }

  protected function _validatePasswordHash (User $user, $challenge) {
    $salt = $user->passwordSalt;
    $passwordHash = $user->passwordHash;
    $targetHash = sha1($passwordHash . $salt);
    return ($challenge === $targetHash);
  }
}