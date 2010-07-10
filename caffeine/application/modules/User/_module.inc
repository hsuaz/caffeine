<?php
namespace Ferrox\Caffeine\User;



class _module
  extends
    \Ferrox\Caffeine\System\Api\ModuleBaseDynamicApi
{
  const NAME = 'User Module';
  const VERSION = '1.0';
  const DESCRIPTION = 'User Management';
  const ID = 'system_user';
  static protected $_api = NULL;


  /**
   * Intetrnal API
   * @return array
   */
  public function routingInternalApi () {
    $classNamespace = '\\' . __NAMESPACE__ . '\Api';
    $api = array (
      'user/login'                 => array(
        'class' => $classNamespace . '\Login',
        'cache' => FALSE,
        'authenticate' => FALSE,
        'api' => array(
          'name' => 'login',
          'parameters' => array(
            'username' => NULL,
            'password' => NULL,
          ),
        ),
      ),
      'user/login/getPasswordSalt' => array(
        'class' => $classNamespace . '\GetPasswordSalt',
        'cache' => FALSE,
        'authenticate' => FALSE,
        'api' => array(
          'name' => 'getPasswordSalt',
          'parameters' => array(
            'username' => NULL,
          ),
        ),
      ),
      'user/logout'                => array(
        'class' => $classNamespace . '\Logout',
        'cache' => FALSE,
        'api' => array(
          'name' => 'logout',
          'parameters' => array(),
        ),
      ),
      'user/cyclePasswordSalt'    => array(
        'class' => $classNamespace . '\CyclePasswordSalt',
        'cache' => FALSE,
        'api' => array(
          'name' => 'cyclePasswordSalt',
          'parameters' => array(
            'uid' => NULL,
          ),
        ),
      ),
      'user/find/id' => array(
        'class' => $classNamespace . '\FindById',
        'api' => array(
          'name' => 'findById',
          'parameters' => array(
            'uid' => NULL,
          ),
        ),
      ),
      'user/find/username' => array(
        'class' => $classNamespace . '\FindByUsername',
        'api' => array(
          'name' => 'findByUsername',
          'parameters' => array(
            'username' => NULL,
          ),
        ),
      ),
//      'user/register/getRequiredParams' => array(
//      ),
      'user/register/getFields' => array(
        'class' => $classNamespace . '\RegisterFields',
        'api' => array(
          'name' => 'registerFields',
          'parameters' => array(
          ),
        ),
      ),
      'user/register' => array(
        'class' => $classNamespace . '\Register',
        'api' => array(
          'name' => 'register',
          'parameters' => array(
            'username' => NULL,
            'email' => NULL,
            'agree_tos' => NULL,
          ),
        ),
      ),
    );
    return $api;
  }


  /**
   * External API
   * @todo: Change this to add better event handling so that
   *      data going out can be filtered for security.
   * @return array
   */
  public function routingExternalApi () {
    $api = array(
      'user/login' => array(
        'cache' => FALSE,
        ),
      'user/login/getPasswordSalt' => array(
        'cache' => FALSE,
        ),
      'user/logout' => array(
        'cache' => FALSE,
        ),
      'user/find/username' => array(
        'filterResult' => '\\' . __NAMESPACE__ . '\ExternalApi\UserFilter'
        ),
      'user/getInfo' => array(
        ),
      'user/register/getFields' => array(

        ),
      'user/register' => array (
        'cache' => FALSE,
        ),
    );
    return $api;
  }
}
