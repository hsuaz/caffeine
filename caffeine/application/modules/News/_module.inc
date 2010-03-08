<?php
namespace Ferrox\Caffeine\News;



class _module
  extends
    \Ferrox\Caffeine\System\Api\ModuleBaseDynamicApi
{
  const NAME = 'News Module';
  const VERSION = '1.0';
  const DESCRIPTION = 'News Management';
  const ID = 'news';
  static protected $_api = NULL;


  /**
   * Intetrnal API
   * @return array
   */
  public function routingInternalApi () {
    $classNamespace = '\\' . __NAMESPACE__ . '\Api';
    $api = array (
      'news/getLatest'                 => array(
        'class' => $classNamespace . '\GetLatest',
        'cache' => FALSE,
        'authenticate' => FALSE,
        'api' => array(
          'name' => 'login',
          'parameters' => array(
            'count' => NULL,
            'locales' => NULL,
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
      'news/getLatest' => array(
        ),
    );
    return $api;
  }
}
