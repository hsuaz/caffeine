<?php
/**
 * Sample Module
 * Reference
 */

/**
 * File Encoding
 */
declare(encoding='UTF-8');

/**
 * Namepsace
 *
 */
namespace Ferrox\Caffeine\User;



/**
 * Module Interface Class
 * The behaviour of the module is statically defined here.
 *
 */
class _module
  extends
    \Ferrox\Caffeine\System\Api\ModuleBaseDynamicApi
{
  /**
   * Friendly name of the module
   * @note Not currently in use
   * @var string
   */
  const NAME = 'User Module';

  /**
   * Version of the module.
   * This will trigger updates to run when changed
   * @note Not currently in use
   * @var string
   */
  const VERSION = '1.0';

  /**
   * Description
   * @note Not currently in use
   * @var string
   */
  const DESCRIPTION = 'User Management';

  /**
   * Unique ID for the module
   * This name defines how the module will be referenced in code, and in
   * the configuration .ini files that refer to it.
   *
   * Format should be kept lowercase only [a-z\-]
   * @var string
   */
  const ID = 'system_user';


  /**
   * Local API cache
   * This must be declared static for proper behaviour of the default API class
   *
   * @var object
   */
  static protected $_api = NULL;


  /**
   * Internal API
   * This defines the Internal API Routing information
   * The format of the return value is as follows:
   *
   * The route exposed to match
   * [route] => array(
   *  'class'  :  The classname that will execute this method.
   *              Must be of type Api\ApiMethodBase
   *              Must be fully qualified
   *              REQUIRED.
   *              TYPE: string
   *
   *  'cache'  :  Whether this call is cacheable
   *              @todo Implement
   *              OPTIONAL: DEFAULT=TRUE
   *              TYPE: boolean
   *
   *  'authenticate' : UNUSED, may remove
   *              OPTIONAL: DEFAULT=FALSE
   *              TYPE: boolean
   *
   *  'api'    :  DynamicApi Configuration
   *              OPTIONAL: DEFAULT=array()
   *              This is used by the DynamicApi class to generate an api
   *              function call.
   *              TYPE: array
   *     'name'     Method Name
   *                This defines the name to be used in the api call
   *                ie:  $api->name()
   *                REQUIRED
   *                TYPE: string
   *     'parameters'  Parameters to the call
   *                   REQUIRED, MAY BE "ARRAY()"
   *                   @todo Implement default to account for this missing
   *                   TYPE: array
   *         'name' => 'default'
   *                      Parameter name and Defaults
   *                      NULL default implies REQUIRED parameter
   *
   * Example:
   *   The User/Login route below defines the Dynamic API
   *   $api->login($username, $password)
   *
   *
   * );
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
    );
    return $api;
  }


  /**
   * External API Definition
   * This defines the methods exposed to the outside world.
   *
   * Currently, this merely passes through the request to the internal api
   * route of the same name.
   *
   * Format:
   *  [route] => array( options )
   *    'cache'         :  Should this call be cached at the external layer
   *                       OPTIONAL DEFAULT=false
   *                       TYPE boolean
   *                       @todo Implement
   *
   *    'filterResult'  :  Class Filter to run after executing the route
   *                       Must define a public filter() callback method
   *
   *                       @todo Change to array
   *
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
      'user/findById' => array(),
      'user/find/username' => array(
        'filterResult' => '\\' . __NAMESPACE__ . '\ExternalApi\UserFilter'
        ),
      'user/getInfo' => array(
        ),
    );
    return $api;
  }



  // Inherited Methods
  // The functions below are not needed to be overriden in a module
  // Avoid doing so.

  /**
   * Methods Inherited by ModuleBaseDynamicApi
   */

  /**
   * Api Dispatcher Instance
   * @var IDispatcher
   */
  protected $_apiDispatcher = NULL;

  /**
   * Api Dispatcher
   * Sets the API Dispatcher, called by $config key apiDispatcher
   */
  protected function setApiDispatcher ($config);

  /**
   * Initialize API
   * Initialises the DynamicApi interface
   * @return \Ferrox\System\Api\DynamicApi
   */
  protected function _initApi();



  /**
   * Methods Inherited by ModuleBase
   */

  /**
   * Return the module name
   * Returns the NAME constant
   * @return string
   */
  public function getName();


  /**
   * Return the Version
   * Returns the VERSION constant
   * @return string
   */
  public function getVersion ();


  /**
   * Return the Module Description
   * Return the DESCRIPTION constant
   * @return string
   */
  public function getDescription ();


  /**
   * Return the Module ID
   * Returns the ID constant
   * @return string
   */
  public function getId ();


  /**
   * Calls all configuration options
   * This calls a method setKey($value) if the config key exists
   * @return unknown_type
   */
  protected function _initConfig();

  /**
   * Initialization
   * Calls _initConfig, _initApi
   */
  protected function _init();

  /**
   * API Initialization
   * @return unknown_type
   */
  abstract protected function _initApi();


  /**
   * Returns the internal use api
   * Returns the static $_api instance
   * @return object
   */
  public function getApi ();


  /**
   * Returns the Internal API Definition
   * @return array
   */
  abstract public function routingInternalApi ();


  /**
   * Return the External API definition
   * @return array
   */
  abstract public function routingExternalApi ();


  /**
   * Loads the routs that this module defines based on the section name given
   * Currently looks for the method "routingSectionName()"
   * @param $section string
   *    Section to load
   * @return array
   *   Routing information
   */
  public function loadRoutes ($section);
}
