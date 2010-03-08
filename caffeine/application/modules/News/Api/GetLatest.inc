<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\News\Api;
use
  \Ferrox\Caffeine\System\Api as Api,
  \Ferrox\Caffeine\User\User as User;

class GetLatest
  extends
    Api\ApiMethodBase
{
  /**
   * Process
   * @return boolean
   *  Success status
   */
  protected function _process () {
    $count = (int) ((\array_key_exists('count', $this->_input)) ? $this->_input['count'] : 10);
    if ($count <= 0) {
      $count = 0;
    }
    if ($count > 10) {
      $count = 10;
    }

    $locales = (\array_key_exists('locales', $this->_input)) ? $this->_input['locales'] : '';
    $locales = explode(',', $locales);
    $locales = array_filter($locales);


    $news = $this->_getApplication()->news();
    $results = $news->getLatest($count, $locales);

    $userApi = $this->_getApplication()->getApi('system_user');
    $users = array();
    foreach ($results as $id => $item) {
      $user = $userApi->findById($item['author']);
      $results[$id]['author']  = array(
        'id' => $user['user']['id'],
        'username' => $user['user']['username'],
      );
    }
    $this->_setResult('news', $results);

    return (!empty($results));
  }
}