<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\DeveloperAccess;

class _module
  extends
    \Ferrox\Caffeine\System\Api\ModuleBaseDynamicApi
{
  const NAME = "Developer Access";
  const VERSION = '1.0';
  const DESCRIPTION = 'Developer Access';
  const ID = 'developer_access';


  /**
   * API Interface
   * @var \Ferrox\System\Api\DynamicApi
   */
  static protected $_api = NULL;


  /**
   * Intetrnal API
   * @return array
   */
  public function routingInternalApi () {
    $classNamespace = '\\' . __NAMESPACE__ . '\Api';
    $api = array (
      'user/find/username' => array(
        'class' => $classNamespace . '\Find',
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
   * External API
   * @return array
   */
  public function routingExternalApi () {
    $api = array(
      'devaccess/' => array(
        ),
    );
    return $api;
  }
}
