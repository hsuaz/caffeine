<?php
/**
 * Sample API Method handling.
 * Intermodule Interaction
 */
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\Example\Api;
use
  \Ferrox\Caffeine\System\Api as Api;

class GetLatest
  extends
    Api\ApiMethodBase
{

  const ERROR_INVALID_COUNT_TYPE = 'invalid_count_type';
  const ERROR_INVALID_COUNT = 'count_out_of_range';

  /**
   * Required Parameters
   * If the parameters given here are not present in the request, then
   * the call will fail before the process function is called.
   * @return array
   */
  // protected function _getRequiredParams ()
  //  {
  //    return array(
  //      'username',
  //      'passwordHash',
  //    );
  //  }


  /**
   * Validation functions
   * If a validation function matches a key in the input, it will run automatically
   * on initialization.
   * Should any validation function fail, the process will not run, and the
   * error will be returned.
   */
  protected function validateCount ($value) {
    if (\is_numeric($value)) {
      $this->_setError(ERROR_INVALID_COUNT, 'Count must be numeric');
      return FALSE;
    }

    // Could put the count validation here to enforce a valid range
    // But, instead, we'll just correct the range in process in this example.
    return TRUE;
  }


  // @TODO: Create Preprocess functions to process validated input before
  //        passing them to the process function.

  // @TODO: Create Defaults function.
  //        Planned to run on init, before validation.
  //        Will be in the following format
  // protected function _getDefaults() {
  //   return array(
  //     'count' => 10,
  //     'locale' => '',
  //   );
  // }
  //

  /**
   * Process
   * @return boolean
   *  Success status
   */
  protected function _process () {
    // Because the parameters are not required, they are checked in here.
    $count = (int) ((\array_key_exists('count', $this->_input)) ? $this->_input['count'] : 10);
    $locales = (\array_key_exists('locales', $this->_input)) ? $this->_input['locales'] : '';


    if ($count <= 0) {
      $count = 0;
    }
    if ($count > 10) {
      $count = 10;
    }

    $locales = \explode(',', $locales);
    $locales = \array_filter($locales);


    /**
     * Returning the concrete instance of the local module class
     * This should be set up by the application handler class
     * @var unknown_type
     */
    $news = $this->_getApplication()->example();
    $results = $news->getLatest($count, $locales);


    /**
     * Intermodule Call
     * Obtain the API for the 'system_user' module.
     * @var unknown_type
     */
    $userApi = $this->_getApplication()->getApi('system_user');
    $users = array();
    foreach ($results as $id => $item) {
      $user = $userApi->findById($item['author']);
      $results[$id]['author']  = array(
        'id' => $user['user']['id'],
        'username' => $user['user']['username'],
      );
    }


    /**
     * Sets the 'news' key on the result to the value given here.
     */
    $this->_setResult('news', $results);

    return (!empty($results));
  }
}